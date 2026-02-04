
using System.Diagnostics;

namespace CoreSoftware.DataStream;

static class ProducerConsumer
{
    private static FileStream dataStream;
    private static StreamReader dataReader;
    private static int fullRecIndex = 0;
    private static float[] fullRecording = new float[1048576];
    readonly public static Object locker = new();

    // NaN means not assigned any other number is a valid value
    private const int SHAREDBUFFERSIZE = 4096;
    private static int bufferIndex = 0;
    private static float[] buffer = new float[SHAREDBUFFERSIZE];
    private static Semaphore emptySem = new(SHAREDBUFFERSIZE, SHAREDBUFFERSIZE);
    private static Semaphore fullSem = new(0, SHAREDBUFFERSIZE);

    private static bool isRunning = false;
    readonly private static double writingHz = 360d;
    readonly private static double writingDelaySec = 1 / writingHz;
    
    public static void ReadFile(string path)
    {
        End();
        dataStream = File.OpenRead(path);
        dataReader = new(dataStream);
    }
    
    public static void Start(Action<float> ConsumeItem)
    {
        Console.WriteLine($"Starting data stream");
        isRunning = true;
        Array.Fill(buffer, float.NaN);

        ReadRecording();

        Thread producer = new(Produce);
        Thread consumer = new(() => Consume(ConsumeItem));
        producer.Start();
        consumer.Start();
    }

    public static void End()
    {
        emptySem.Release();
        fullSem.Release();
        isRunning = false;
    }

    private static void ReadRecording()
    {
        for (int i = 0; !dataReader.EndOfStream; ++i)
        {
            fullRecording[i] = Convert.ToSingle(dataReader.ReadLine());
        }

        dataReader.Close();
        dataStream.Close();
    }

    private static void Consume(Action<float> ConsumeItem)
    {
        while (isRunning)
        {
            fullSem.WaitOne();
            for (int i = 0; i < bufferIndex; i++)
            {
                ConsumeItem(buffer[i]);
                buffer[i] = float.NaN;
            }
            bufferIndex = 0;
            emptySem.Release();
        }
    }

    // Windows, by default, has a clock interrupt rate that ticks every ~15.6 ms.
    // A Sleep() call can only complete on one of these ticks. If you request
    // a sleep of 1 ms, it will likely wait until the next tick occurs.
    private static void Produce()
    {
        while (isRunning)
        {
            emptySem.WaitOne();
            buffer[bufferIndex++] = fullRecording[Math.Min(fullRecording.Length - 1, fullRecIndex++)];
            fullSem.Release();

            PreciseSleep(writingDelaySec);
        }
    }

    // double precision thread blocking function
    private static void PreciseSleep(double durationSeconds)
    {
        var durationTicks = Math.Round(durationSeconds * Stopwatch.Frequency);
        var sw = Stopwatch.StartNew();

        while (sw.ElapsedTicks < durationTicks)
        {

        }
    }
    
}

