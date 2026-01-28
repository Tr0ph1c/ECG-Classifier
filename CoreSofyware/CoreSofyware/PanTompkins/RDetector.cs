using System;

namespace CoreSofyware.PanTompkins;
public class RDetector : IDisposable
{
    private IntPtr _handle;
    private bool _disposed = false;
    private readonly double _samplingFrequency;
    private readonly SamplesBuffer _buffer;

    /// <summary>
    /// Create Pan-Tompkins QRS detector
    /// </summary>
    /// <param name="samplingFrequencyHz">ECG sampling frequency (50-1000 Hz)</param>
    /// <exception cref="ArgumentException">Invalid sampling frequency</exception>
    /// <exception cref="OutOfMemoryException">Native allocation failed</exception>
    public RDetector(double samplingFrequencyHz)
    {
        if (samplingFrequencyHz < 50.0 || samplingFrequencyHz > 1000.0)
            throw new ArgumentException("Sampling frequency must be between 50-1000 Hz", nameof(samplingFrequencyHz));

        _handle = NativeECGDetector.ECGDetector_Create(samplingFrequencyHz);
        if (_handle == IntPtr.Zero)
            throw new OutOfMemoryException("Failed to create ECG detector (native allocation failed)");

        _samplingFrequency = samplingFrequencyHz;
        
        _buffer = new SamplesBuffer((int) samplingFrequencyHz / 2);
    }

    /// <summary>
    /// Process a single ECG sample
    /// </summary>
    /// <param name="sample">Raw ECG voltage sample</param>
    /// <returns>
    ///   >=0 : Detected QRS peak index (relative to sample stream start)
    ///   -1  : Not ready yet (initialization phase - normal)
    /// </returns>
    /// <exception cref="ObjectDisposedException">Detector has been disposed</exception>
    public int ProcessSample(double sample)
    {
        ThrowIfDisposed();
        _buffer.Add(sample);
        int result = NativeECGDetector.ECGDetector_ProcessSample(_handle, sample);
        
        if (result == -2)
            throw new InvalidOperationException("ECG detector internal error (invalid handle or exception)");
        
        return (result == -1)? -1 : result - _buffer.FindIndexOfMax(); // -1 = not ready yet (normal during warmup)
    }

    /// <summary>
    /// Reset detector state (e.g., after signal loss or lead change)
    /// </summary>
    public void Reset()
    {
        ThrowIfDisposed();
        NativeECGDetector.ECGDetector_Reset(_handle);
    }

    public double SamplingFrequency => _samplingFrequency;

    public void Dispose()
    {
        if (!_disposed && _handle != IntPtr.Zero)
        {
            NativeECGDetector.ECGDetector_Destroy(_handle);
            _handle = IntPtr.Zero;
            _disposed = true;
        }
        GC.SuppressFinalize(this);
    }

    ~RDetector() => Dispose();

    private void ThrowIfDisposed()
    {
        if (_disposed)
            throw new ObjectDisposedException(nameof(RDetector));
    }
}