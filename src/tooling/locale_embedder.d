import std.getopt;
import std.stdio;
import std.path;
import std.file;
import std.range;
import core.stdc.stdlib;

static bool isVerbose = false;
void logDebug(string msg)
{
    if (isVerbose)
    {
        writeln(msg);
    }
}

void processLocale(string file)
{
    logDebug("Processing " ~ file);
}

void processLocales(string input, string output)
{
    if (input.empty())
    {
        throw new Error("Input folder doesn't exist!");
    }

    auto files = dirEntries(input, "*.csv", SpanMode.depth);
    foreach (file; files)
    {
        processLocale(file);
    }
}

int main(string[] args)
{
    string inputFolder = "";
    string destFile = "";
    bool benchmark = false;

    GetoptResult helpInformation;
    try
    {
        helpInformation = getopt(
            args,
            "input", &inputFolder,
            "output", &destFile,
            "verbose", &isVerbose,
            "benchmark", &benchmark
        );
    }
    catch (Exception ex)
    {
        writeln(stderr, "Invalid arguments passed!");
        return EXIT_FAILURE;
    }

    if (helpInformation.helpWanted)
    {
        defaultGetoptPrinter("Some information about the program.",
            helpInformation.options);
    }
    else
    {
        try
        {
            processLocales(inputFolder, destFile);
        }
        catch (Exception exc)
        {
            writeln(stderr, exc.msg);
            return EXIT_FAILURE;
        }

    }
    return EXIT_SUCCESS;
}
