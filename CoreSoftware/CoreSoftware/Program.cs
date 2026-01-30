using CoreSoftware.PanTompkins;
using CoreSoftware.DataStream;
using CoreSoftware.DataProcessing;

var builder = WebApplication.CreateBuilder(args);
builder.Services.AddRazorPages();
builder.Services.AddServerSideBlazor();

var app = builder.Build();
app.MapRazorPages();
app.MapBlazorHub();
app.MapStaticAssets();
app.UseStaticFiles();

string textCsv = String.Empty;
for (int i = 1; i < 93; i++)
{
    textCsv += $"sample_{i},";
}

textCsv += "rr_interval\n";

BeatProducer br = new BeatProducer(360d);
br.NewBeatProduced += (object? o, NewBeatProducedEventArgs e) =>
{
    foreach (var s in e.Beat.Samples)
    {
        textCsv += $"{s},";
    }

    textCsv += $"{e.Beat.RrInterval}\n";

};

ProducerConsumer.ReadFile("Tests/100_raw");
ProducerConsumer.Start((s) =>
{ 
    br.ProcessSample(s);
});


app.MapGet("/", () => textCsv);
app.MapFallbackToPage("/EcgStream/{*catchall}", "/EcgStream/Main");
app.Run();
