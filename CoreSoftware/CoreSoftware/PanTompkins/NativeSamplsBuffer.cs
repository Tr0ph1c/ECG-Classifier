using System.Runtime.InteropServices;

namespace CoreSoftware.PanTompkins;

internal static class NativeSamplsBuffer
{
#if WINDOWS
    private const string LibName = "libDetectorPanTompkins";
#elif LINUX
    private const string LibName = "DetectorPanTompkins";
#endif

    [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern IntPtr SamplesBuffer_Create(int maxSize);

    [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern int SamplesBuffer_AppendSample(IntPtr handle, double sample);

    [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern int SamplesBuffer_GetRPeakIndex(IntPtr handle);

    [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void SamplesBuffer_Destroy(IntPtr handle);

}