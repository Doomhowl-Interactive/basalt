module tooling.embedder_locale;

import std.getopt;
import std.stdio;
import std.path;
import std.file;
import std.string;
import std.range;
import std.regex;
import std.algorithm;

import core.stdc.stdlib;

static bool isVerbose = false;
void logDebug(string msg)
{
    if (isVerbose)
    {
        writeln(msg);
    }
}

struct Sentence
{
    string[] locales;
    string primary;

    this(string csvLine)
    {
        auto cols = csvLine.split(";");
        if (cols.length == 0){
            throw new Error("Invalid CSV line " ~ csvLine);
        }
        primary = cols[0];
        locales = cols;
    }

    string toString() const
    {
        return format("%ul: %s", toHash(), primary);
    }

    size_t toHash() const @safe nothrow
    {
        return typeid(primary).getHash(&primary);
    }

    bool opEquals(R)(const R other) const
    {
        return toHash() == other.toHash();
    }

    bool containedIn(Sentence[ulong] iter)
    {
        foreach (Sentence o; iter)
        {
            if (o == this)
            {
                return true;
            }
        }
        return false;
    }
}

Sentence[ulong] loadSentences(string csv)
{
    Sentence[ulong] result;
    string[] lines = splitLines(csv);
    for (auto i = 0; i < lines.length; i++)
    {
        string line = lines[i];
        auto s = Sentence(line);
        if (s.containedIn(result))
        {
            string msg = format("Duplicate sentence at line %d: %s",i,s.primary);
            throw new Error(msg);
        }
        result[s.toHash()] = s;
    }
    return result;
}

void processLocale(string file)
{
    logDebug("Processing " ~ file);
    string csvContent = readText(file);

    auto sentences = loadSentences(csvContent);
    foreach (Sentence key; sentences)
    {
        writeln(key.toString);
    }
}

void processLocales(string input, string output)
{
    if (input.empty())
    {
        throw new Error("Input folder '" ~ input ~ "' doesn't exist!");
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
        stderr.writeln( "Invalid arguments passed!");
        return EXIT_FAILURE;
    }

    if (helpInformation.helpWanted)
    {
        defaultGetoptPrinter("Some information about the program.",
            helpInformation.options);
    }
    else
    {
        if (inputFolder.empty() || destFile.empty())
        {
            stderr.writeln("No inputFolder or destination file passed!");
        }
        else
        {
            try
            {
                processLocales(inputFolder, destFile);
            }
            catch (Exception exc)
            {
                stderr.writeln( exc.msg);
                return EXIT_FAILURE;
            }
        }

    }
    return EXIT_SUCCESS;
}
