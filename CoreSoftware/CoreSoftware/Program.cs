using CoreSoftware.PanTompkins;
using CoreSoftware.DataStream;

var builder = WebApplication.CreateBuilder(args);
var app = builder.Build();
int last = -1;
RDetector rd = new(360d);
ProducerConsumer.ReadFile("Tests/100_raw");
ProducerConsumer.Start((s) =>
{ 
    int rIndex = rd.ProcessSample(s);
    if (rIndex != -1)
    {
        last = rIndex;
    }
});


app.MapGet("/", () => $"Last r index = {last}\n");

app.Run();
