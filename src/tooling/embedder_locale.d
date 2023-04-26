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

// TODO: Rewrite in C

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
    foreach (ref string str; strings)
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
        string desc = format("%s: %s", toName(), primary);
        foreach (item; locales) {
            desc ~= "," ~ item;
        }
        return desc;
    }

    string toName() const
    {
        return format("LOC_%u", toHash());
    }

    string toCode()
    {
        string code = "// " ~ primary ~ "\n";
        code ~= format("const char %s[] = \"%s", toName(), primary);
        foreach (string loc; locales)
        {
            code ~= "|" ~ loc;
        }
        code ~= "\";\n";
        return code;
    }

    string toHeaderCode()
    {
        string code = "// " ~ primary ~ "\n";
        code ~= format("extern const char %s[];", toName());
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

string generateSentenceArray(ref Sentence[ulong] sentences)
{
    string code = "const char** LOC_ALL = {\n";
    foreach (sent; sentences)
    {
        code ~= format("    &%s,\n",sent.toName());
    }
    code ~= "};";
    return code;
}

void generateLocaleHeaderFile(ref Sentence[ulong] sentences, string inputFolder, string outputFile)
{
    string fileContent = "#include \"basalt.h\"\n";
    fileContent ~= "// Automatically generated, do not edit by hand!\n\n";
    foreach (sent; sentences)
    {
        fileContent ~= sent.toHeaderCode() ~ "\n\n";
    }
    fileContent ~= "\nextern const char** LOC_ALL;\n\n";
    logDebug(fileContent);
    std.file.write(outputFile, fileContent);
    writeln("Wrote header code to " ~ outputFile);
}

void generateLocaleCodeFile(ref Sentence[ulong] sentences, string inputFolder, string outputFile)
{
    string fileContent = "#include \"basalt.h\"\n";
    fileContent ~= "// Automatically generated, do not edit by hand!\n\n";
    foreach (sent; sentences)
    {
        fileContent ~= sent.toCode() ~ "\n";
    }
    fileContent ~= generateSentenceArray(sentences) ~ "\n";
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
                Sentence[ulong] sentences = loadSentencesFromFolder(inputFolder);
                generateLocaleCodeFile(sentences, inputFolder, destFile ~ ".c");
                generateLocaleHeaderFile(sentences, inputFolder, destFile ~ ".h");
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
