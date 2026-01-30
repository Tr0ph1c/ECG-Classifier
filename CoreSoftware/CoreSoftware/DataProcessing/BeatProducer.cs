using CoreSoftware.PanTompkins;
using CoreSoftware.Models;

namespace CoreSoftware.DataProcessing;

public class BeatProducer
{
    private static readonly int BeatSize = 91;
    private int? NewR;
    private int? OldR;
    private long ProcessedSamples;
    private CircularBuffer<double> SamplesBuffer;
    public event EventHandler<NewBeatProducedEventArgs> NewBeatProduced;
    private RDetector Detector;
    private int IterationsToProduce;

    public BeatProducer(double fs)
    {
        SamplesBuffer = new CircularBuffer<double>(BeatSize);
        Detector = new RDetector(fs);
        NewR = null;
        OldR = null;
        ProcessedSamples = 0;
        IterationsToProduce = -1;
    }

    public void ProcessSample(double sample)
    {
        ProcessedSamples++;
        SamplesBuffer.Write(sample);
        if (IterationsToProduce != -1)
        {
            if (IterationsToProduce == 0 && OldR != null)
            {
                ProduceNewBeat();
                IterationsToProduce = -1;
            }
            else
            {
                IterationsToProduce--;
            }
        }
        
        int rIndex = Detector.ProcessSample(sample);
        if (rIndex != -1)
        {
            AddNewR(rIndex);
            if (OldR != null)
            {
                IterationsToProduce = (int)(ProcessedSamples - rIndex);
            }
        }
    }
    private void AddNewR(int r)
    {
        OldR = NewR;
        NewR = r;
    }

    private void ProduceNewBeat()
    {
        Beat beat = new Beat();
        beat.Samples = SamplesBuffer.ToArray();
        beat.RrInterval = NewR - OldR;
        NewBeatProduced.Invoke(null, new NewBeatProducedEventArgs(beat));
    }

}
public class NewBeatProducedEventArgs : EventArgs
{
    public Beat Beat { get; }

    public NewBeatProducedEventArgs(Beat beat)
    {
        Beat =  beat;
    }
}