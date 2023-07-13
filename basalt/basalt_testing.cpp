#include <stdlib.h>
#include <string>

#include "basalt_testing.hpp"
#include "basalt_console.hpp"
#include "basalt_graphics.hpp"

namespace basalt {

using namespace std;

void EndTest(string name, string description, bool succeeded)
{
    string padding = PadStringRight(name, '.', 50);
    string result = succeeded ? "PASSED" : "FAILED";
    ConsoleColor color = succeeded ? CGREEN : CRED;
    BasaltPrintColored(color, "TEST  : %s %s", padding, result);

    if (!succeeded) {
        ERR("Failed at --> %s", description);
        ERR("Cannot proceed as unit tests failed!");
        exit(EXIT_FAILURE);
    }
}

TEST(ByteReading)
{
    // spr block size example
    unsigned char result[4];
    unsigned int value = 1770;
    result[3] = (unsigned char)(value >> 24);
    result[2] = (unsigned char)(value >> 16);
    result[1] = (unsigned char)(value >> 8);
    result[0] = (unsigned char)(value >> 0);
    unsigned int val = *((unsigned int*)result);
    CHECK(val == 1770, "Calculate 1770 with bit shifting");
}
END;

TEST(Math)
{
    CHECK(min(5, 3) == 3, "Minimum");
    CHECK(max(5, 3) == 5, "Maximum");
}
END;

// NOTE: I'm terrible at calculating hexes and binary,
// so this is a little sandbox to make sure I don't go insane.
TEST(Colors)
{
    Color alpha = 0x000000FF;
    CHECK(alpha == MakeRGB(0, 0, 0, 255), "alpha");

    Color red = 0xFF0000FF;
    CHECK(red == MakeRGB(255, 0, 0), "red");

    Color green = 0x00FF00FF;
    CHECK(green == MakeRGB(0, 255, 0), "green");

    Color blue = 0x0000FFFF;
    CHECK(blue == MakeRGB(0, 0, 255), "blue");

    Color left = MakeRGB(255, 50, 50, 255);
    CHECK((left & 0x000000FF) == 0x000000FF, "compare alpha");

    Color purple = MakeRGB(51, 51, 153, 255);
    CHECK(purple == 0x333399FF, "purple color");
}
END;

TEST(StringPadding)
{
    {
        auto pad = PadStringRight("Hello world!", '.', 20);
        auto expected = "Hello world!........";
        CHECK(pad == expected, "PadStringRight with dots");
    }
    {
        auto pad = PadStringRight("Hello world!", ' ', 20);
        auto expected = "Hello world!        ";
        CHECK(pad == expected, "PadStringRight with spaces");
    }
}
END;

TEST(FormatText)
{
    auto greeting = FormatText("%s %s", "Hello", "world!");
    CHECK(greeting == "Hello world!", "String format failed!");

    auto money = FormatText("I have %d dollars", 50);
    CHECK(money == "I have 50 dollars", "String format failed!");

    for (int i = 0; i < 50; i++) {
        auto money2 = FormatText("I have %d dollars", 50 + i);
        char expected[100];
        snprintf(expected, 100, "I have %d dollars", 50 + i);
        CHECK(money2 == expected, "Looping string format failed!");
    }
}
END;

TEST(StripText)
{
    string text = "             Hello, space cadet!       ";
    string strip = StripText(text);
    CHECK(strip == "Hello, space cadet!", "Strip text");
}
END;
//
// TEST(ExtractDialogLines)
//{
//    // split line according to backslash
//    // strip all whitespace
//    const char* dialog = "Hello world!\\\
//                          How are you today?";
//
//    StringArray arr = DialogSequence(dialog);
//    CHECK(arr.count == 2, "String array is 2");
//    CHECK(strcmp(arr.strings[0], "Hello world!") == 0, "Check first line");
//    CHECK(strcmp(arr.strings[1], "How are you today?") == 0, "Check second line");
//}
// END;
//
// TEST(ExtractDialogKeywords)
//{
//    const char* line = "{teacher|excited}Greetings traveler, are you up for the challenge to reach
//    "
//                       "the mountain top?";
//    StringArray keywords = ExtractDialogKeywords(line);
//    CHECK(keywords.count == 2, "There are two keywords");
//    CHECK(TextIsEqual(keywords.strings[0], "teacher"), "Check first keyword");
//    CHECK(TextIsEqual(keywords.strings[1], "excited"), "Check second keyword");
//}
// END;

void RunUnitTests()
{
    INFO("Doing unit tests");
    TestByteReading();
    TestStringPadding();
    TestMath();
    TestColors();
    TestFormatText();
    TestStripText();
}

}  // namespace basalt