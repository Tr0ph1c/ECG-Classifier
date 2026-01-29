# Build stage
FROM mcr.microsoft.com/dotnet/sdk:10.0 AS build
WORKDIR /src

COPY CoreSoftware.csproj ./
RUN dotnet restore

COPY . .
RUN dotnet publish -c Release -o /app/publish

# Runtime stage
FROM mcr.microsoft.com/dotnet/aspnet:10.0
WORKDIR /app

COPY --from=build /app/publish .


COPY ./Tests ./Tests

EXPOSE 5050
ENTRYPOINT ["dotnet", "CoreSoftware.dll"]
