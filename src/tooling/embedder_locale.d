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
    ulong hash;

    this(string csvLine)
    {
        locales = csvLine.split(";");
        hash = computeHash();
    }

    string primary()
    {
        if (locales.length > 0)
        {
            return locales[0];
        }
        else
        {
            throw new Error("Sentence does not have any text.");
        }
    }

    string toString()
    {
        string first = locales.empty ? "(Nothing)" : locales[0];
        return format("%ul: %s", hash, first);
    }

    bool opEquals(R)(const R other) const
    {
        return hash == other.hash;
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

    private auto computeHash()
    {
        if (locales.length > 0)
        {
            string first = primary();
            return typeid(first).getHash(&first);
        }
        else
        {
            throw new Error("Sentence does not have any text.");
        }
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
            string msg = format("Duplicate sentence at line %d: %s",i,s.primary());
            throw new Error(msg);
        }
        result[s.hash] = s;
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
