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

void stripAll(ref string[] strings)
{
    foreach (string str; strings)
    {
        str = str.strip();
    }
}

struct Sentence
{
    string[] locales;
    string primary;

    this(string csvLine)
    {
        string[] cols = csvLine.split(";");
        if (cols.length == 0)
        {
            throw new Error("Invalid CSV line " ~ csvLine);
        }
        stripAll(cols);
        this(cols);
    }

    this(string[] locales)
    {
        this.primary = locales[0];
        this.locales = locales[1 .. $];
    }

    string toString() const
    {
        string desc = format("%ul: %s", toHash(), primary);
        foreach (item; locales) {
            desc ~= "," ~ item;
        }
        return desc;
    }

    string toCode()
    {
        string code = "// " ~ primary ~ "\n";
        code ~= format("const char* LOC_%u[] = { \"%s\"", toHash(), primary);
        foreach (string loc; locales)
        {
            code ~= ", \"" ~ loc ~ "\"";
        }
        code ~= ", NULL };\n";
        return code;
    }

    size_t toHash() const @safe nothrow
    {
        return typeid(primary).getHash(&primary);
    }

    bool opEquals(R)(const R other) const
    {
        return toHash() == other.toHash();
    }
}

Sentence[ulong] loadSentencesFromFile(string file)
{
    logDebug("Processing " ~ file);
    string csvContent = readText(file);

    Sentence[ulong] result;
    string[] lines = splitLines(csvContent);
    for (auto i = 0; i < lines.length; i++)
    {
        string line = lines[i];
        auto s = Sentence(line);
        result[s.toHash()] = s;
        logDebug(s.toString);
    }
    return result;
}

Sentence[ulong] loadSentencesFromFolder(string input)
{
    if (input.empty())
    {
        throw new Error("Input folder '" ~ input ~ "' doesn't exist!");
    }

    Sentence[ulong] locales;
    foreach (file; dirEntries(input, "*.csv", SpanMode.depth))
    {
        foreach (sent; loadSentencesFromFile(file))
        {
            auto hash = sent.toHash();
            if (hash in locales)
            {
                throw new Error("Duplicate sentence " ~ sent.primary);
            }
            locales[hash] = sent;
        }
    }
    return locales;
}

void generateLocaleCodeFile(string inputFolder, string outputFile)
{
    string fileContent = "#include \"basalt.h\"\n\n";
    Sentence[ulong] sentences = loadSentencesFromFolder(inputFolder);
    foreach (sent; sentences)
    {
        fileContent ~= sent.toCode() ~ "\n";
    }
    fileContent ~= "\n";
    logDebug(fileContent);
    std.file.write(outputFile, fileContent);
    writeln("Wrote code to " ~ outputFile);
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
            "input|i", &inputFolder,
            "output|o", &destFile,
            "verbose|v", &isVerbose,
            "benchmark|b", &benchmark
        );
    }
    catch (Exception ex)
    {
        stderr.writeln("Invalid arguments passed!");
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
                generateLocaleCodeFile(inputFolder, destFile);
            }
            catch (Exception exc)
            {
                stderr.writeln("Failed to process locales!");
                stderr.writeln(exc.msg);
                return EXIT_FAILURE;
            }
        }

    }
    return EXIT_SUCCESS;
}
