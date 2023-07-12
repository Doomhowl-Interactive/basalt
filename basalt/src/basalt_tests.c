#include <stdlib.h>

#include "basalt.h"
#include "basalt_extra.h"
#include "basalt_extra.hpp"

// =====================================
// Small over-engineered test framework
// =====================================

#define TEST(N) \
 void Test##N() \
 { \
  const char* NAME = #N;
#define END \
 EndTest(NAME, "", true); \
 }  // succeeds

static void EndTest(const char* name, const char* description, bool succeeded)
{
    const char* padding = PadStringRight(name, '.', 50);
    const char* result = succeeded ? "PASSED" : "FAILED";
    ConsoleColor color = succeeded ? CGREEN : CRED;
    BasaltPrintColored(color, "TEST  : %s %s", padding, result);

    if (!succeeded) {
        ERR("Failed at --> %s", description);
        ERR("Cannot proceed as unit tests failed!");
        exit(EXIT_FAILURE);
    }
}

#define CHECK(X, I) \
 if (!(X)) { \
  EndTest(NAME, I, false); \
  return; \
 }  // fails

// =====================================

TEST(ByteReading)
{
    // spr block size example
    uchar result[4];
    uint value = 1770;
    result[3] = (uchar)(value >> 24);
    result[2] = (uchar)(value >> 16);
    result[1] = (uchar)(value >> 8);
    result[0] = (uchar)(value >> 0);
    uint val = *((uint*)result);
    CHECK(val == 1770, "Calculate 1770 with bit shifting");
}
END;

TEST(Math)
{
    CHECK(MIN(5, 3) == 3, "Minimum");
    CHECK(MAX(5, 3) == 5, "Maximum");
}
END;

// NOTE: I'm terrible at calculating hexes and binary,
// so this is a little sandbox to make sure I don't go insane.
TEST(Colors)
{
    Color alpha = 0x000000FF;
    CHECK(alpha == RGBA(0, 0, 0, 255), "alpha");

    Color red = 0xFF0000FF;
    CHECK(red == RGB(255, 0, 0), "red");

    Color green = 0x00FF00FF;
    CHECK(green == RGB(0, 255, 0), "green");

    Color blue = 0x0000FFFF;
    CHECK(blue == RGB(0, 0, 255), "blue");

    Color left = RGBA(255, 50, 50, 255);
    CHECK((left & 0x000000FF) == 0x000000FF, "compare alpha");

    Color purple = RGBA(51, 51, 153, 255);
    CHECK(purple == 0x333399FF, "purple color");
}
END;

TEST(StringPadding)
{
    {
        const char* pad = PadStringRight("Hello world!", '.', 20);
        const char* expected = "Hello world!........";
        CHECK(TextIsEqual(pad, expected), "PadStringRight with dots");
    }
    {
        const char* pad = PadStringRight("Hello world!", ' ', 20);
        const char* expected = "Hello world!        ";
        CHECK(TextIsEqual(pad, expected), "PadStringRight with spaces");
    }
}
END;

TEST(FormatText)
{
    const char* greeting = FormatText("%s %s", "Hello", "world!");
    CHECK(TextIsEqual(greeting, "Hello world!"), "String format failed!");

    const char* money = FormatText("I have %d dollars", 50);
    CHECK(TextIsEqual(money, "I have 50 dollars"), "String format failed!");

    for (int i = 0; i < 50; i++) {
        const char* money2 = FormatText("I have %d dollars", 50 + i);
        char expected[100];
        snprintf(expected, 100, "I have %d dollars", 50 + i);
        CHECK(TextIsEqual(money2, expected), "Looping string format failed!");
    }
}
END;

TEST(ToUppercase)
{
    const char* text = "testTEXT";
    const char* upper = ToUppercase(text);
    CHECK(TextIsEqual(upper, "TESTTEXT"), "ToUppercase");
}
END;

TEST(ToLowercase)
{
    const char* text = "testTEXT";
    const char* lower = ToLowercase(text);
    CHECK(TextIsEqual(lower, "testtext"), "ToLowercase");
}
END;

TEST(StripText)
{
    const char* text = "             Hello, space cadet!       ";
    char* buffer = CloneText(text);
    StripText(buffer);
    CHECK(TextIsEqual(buffer, "Hello, space cadet!"), "Strip text");
    free(buffer);
}
END;

TEST(ExtractDialogLines)
{
    // split line according to backslash
    // strip all whitespace
    const char* dialog = "Hello world!\\\
                          How are you today?";

    StringArray arr = ExtractDialogLines(dialog);
    CHECK(arr.count == 2, "String array is 2");
    CHECK(strcmp(arr.strings[0], "Hello world!") == 0, "Check first line");
    CHECK(strcmp(arr.strings[1], "How are you today?") == 0, "Check second line");
}
END;

TEST(ExtractDialogKeywords)
{
    const char* line = "{teacher|excited}Greetings traveler, are you up for the challenge to reach "
                       "the mountain top?";
    StringArray keywords = ExtractDialogKeywords(line);
    CHECK(keywords.count == 2, "There are two keywords");
    CHECK(TextIsEqual(keywords.strings[0], "teacher"), "Check first keyword");
    CHECK(TextIsEqual(keywords.strings[1], "excited"), "Check second keyword");
}
END;

BASALT void UnitTest()
{
    INFO("Doing unit tests");
    TestByteReading();
    TestStringPadding();
    TestMath();
    TestColors();
    TestFormatText();
    TestToLowercase();
    TestToUppercase();
    TestStripText();
    TestExtractDialogLines();
    TestExtractDialogKeywords();
}