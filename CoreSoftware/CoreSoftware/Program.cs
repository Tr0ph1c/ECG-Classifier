using CoreSoftware.PanTompkins;

var builder = WebApplication.CreateBuilder(args);
var app = builder.Build();

RDetector d = new(360d);


app.MapGet("/", () => $"Hello World! {d.ProcessSample(2d)}");

app.Run();
