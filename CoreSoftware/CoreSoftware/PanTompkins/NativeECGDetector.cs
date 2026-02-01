using System;
using System.Runtime.InteropServices;

namespace CoreSoftware.PanTompkins;

internal static class NativeECGDetector
{
#if WINDOWS
    private const string LibName = "libDetectorPanTompkins";
#elif LINUX
    private const string LibName = "DetectorPanTompkins";
#endif

    [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern IntPtr ECGDetector_Create(double samplingFrequencyHz);

    [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern int ECGDetector_ProcessSample(IntPtr handle, double sample);

    [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void ECGDetector_Reset(IntPtr handle);

    [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void ECGDetector_Destroy(IntPtr handle);
}