#include "basalt.h"
#include "basalt_plat.h"

// =====================================
// Small over-engineered test framework
// =====================================

#define TEST(N) void Test##N() { const char* NAME = #N;
#define END EndTest(NAME, "", true); } // succeeds

func void EndTest(const char* name, const char* description, bool succeeded)
{
    const char* padding = PadStringRight(name, '.', 50);
    const char* result = succeeded ? COLTEXT(32, "PASSED"):COLTEXT(31, "FAILED");
    INFO("%s %s", padding, result);

    if (!succeeded)
    {
        ERR("Failed at --> %s", description);
        ERR("Cannot proceed as unit tests failed!");
        exit(EXIT_FAILURE);
    }
}

#define CHECK(X,I) if (!(X)) { EndTest(NAME, I, false); return; } // fails

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
} END;

TEST(Math)
{
    CHECK(MIN(5,3) == 3, "Minimum");
    CHECK(MAX(5,3) == 5, "Maximum");
} END;

// NOTE: I'm terrible at calculating hexes and binary,
// so this is a little sandbox to make sure I don't go insane.
TEST(Colors)
{
    Color alpha = 0x000000FF;
    CHECK(alpha == RGBA(0,0,0,255),"alpha");

    Color red = 0xFF0000FF;
    CHECK(red == RGB(255,0,0),"red");

    Color green = 0x00FF00FF;
    CHECK(green == RGB(0,255,0), "green");

    Color blue = 0x0000FFFF;
    CHECK(blue == RGB(0,0,255), "blue");

    Color left = RGBA(255,50,50,255);
    CHECK((left & 0x000000FF) == 0x000000FF, "compare alpha");

    Color purple = RGBA(51, 51, 153, 255);
    CHECK(purple == 0x333399FF, "purple color");
} END;

TEST(StringPadding)
{
    {
        const char* pad = PadStringRight("Hello world!",'.',20);
        const char* expected = "Hello world!........";
        CHECK(strcmp(pad, expected) == 0, "PadStringRight with dots");
    }
    {
        const char* pad = PadStringRight("Hello world!",' ',20);
        const char* expected = "Hello world!        ";
        CHECK(strcmp(pad, expected) == 0, "PadStringRight with spaces");
    }
} END;

platfunc void UnitTest()
{
    INFO("Doing unit tests");
    TestByteReading();
    TestStringPadding();
    TestMath();
    TestColors();
}
