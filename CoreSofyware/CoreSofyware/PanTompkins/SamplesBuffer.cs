namespace CoreSofyware.PanTompkins;

internal class SamplesBuffer: IDisposable
{
    private IntPtr _handle;
    private bool _disposed;
    
    public SamplesBuffer(int maxSize)
    {
        _handle = NativeSamplsBuffer.SamplesBuffer_Create(maxSize);
        _disposed = false;
    }
    public void Add(double number)
    {
        ThrowIfDisposed();
        int appended = NativeSamplsBuffer.SamplesBuffer_AppendSample(_handle, number);
        if (appended == -1) throw new InvalidOperationException("Append failed or internal error");
        else if (appended == -2) throw new InvalidOperationException("Invalid handle");
    }
    
    public int FindIndexOfMax()
    {
        ThrowIfDisposed();
        return NativeSamplsBuffer.SamplesBuffer_GetRPeakIndex(_handle);
    }
    public void Dispose()
    {
        if (!_disposed && _handle != IntPtr.Zero)
        {
            NativeSamplsBuffer.SamplesBuffer_Destroy(_handle);
            _handle = IntPtr.Zero;
            _disposed = true;
        }
        GC.SuppressFinalize(this);
    }

    ~SamplesBuffer() => Dispose();
    private void ThrowIfDisposed()
    {
        if (_disposed)
            throw new ObjectDisposedException(nameof(RDetector));
    }
}