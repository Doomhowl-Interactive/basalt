#include "basalt.h"
#include "bullet_common.h"

// TODO: DRY
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

func void EndTest(const char* name, const char* description, bool succeeded)
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

TEST(Transformations)
{
    {  // set size and center entity
        Entity e = { 0 };
        SetEntitySize(&e, 5, 3);
        SetEntityCenter(&e, 10, 10);

        CHECK(e.bounds.x == 10.f - 2.5f, "SetEntityCenter (X)");
        CHECK(e.bounds.y == 10.f - 1.5f, "SetEntityCenter (Y)");
        CHECK(e.bounds.width == 5, "SetEntityCenter (Width)");
        CHECK(e.bounds.height == 3, "SetEntityCenter (Height)");
    }

    {  // reverse order
        Entity e = { 0 };
        SetEntityCenter(&e, 10, 10);
        SetEntitySize(&e, 5, 3);

        CHECK(e.bounds.x == 10.f - 2.5f, "Reverse SetEntityCenter (X)");
        CHECK(e.bounds.y == 10.f - 1.5f, "Reverse SetEntityCenter (Y)");
        CHECK(e.bounds.width == 5, "Reverse SetEntityCenter (Width)");
        CHECK(e.bounds.height == 3, "Reverse SetEntityCenter (Height)");
    }

    {  // multiple operations
        Entity e = { 0 };
        SetEntityCenter(&e, 10, 10);
        SetEntitySize(&e, 5, 3);
        SetEntitySize(&e, 10, 5);
        SetEntityCenter(&e, 5, 5);

        CHECK(e.bounds.x == 5.f - 5.f, "Multiple SetEntityCenter (X)");
        CHECK(e.bounds.y == 5.f - 2.5f, "Multiple SetEntityCenter (Y)");
        CHECK(e.bounds.width == 10, "Multiple SetEntityCenter (Width)");
        CHECK(e.bounds.height == 5, "Multiple SetEntityCenter (Height)");
    }
}
END;

TEST(EntityFlags)
{
    Scene scene = { 0 };

    Entity* e = CreateEntity(&scene);
    InitPlayer(e, (Vec2){ 10, 10 });

    Entity* e2 = CreateEntity(&scene);
    InitBullet(e2, GetBulletPattern(0), (Vec2){ 10, 10 }, (Vec2){ 0, 0 });

    CHECK(EntityHasFlag(e, FLAG_PLAYER), "Player has entity flag");
    CHECK(EntityHasFlag(e2, FLAG_BULLET), "Bullet has bullet flag");

    Entity* e3 = CreateEntity(&scene);
    e3->flags = FLAG_BULLET | FLAG_ENEMY;
    CHECK(EntityHasFlag(e3, FLAG_BULLET & FLAG_ENEMY), "Bullet has bullet flag");
}
END;

BULLET void UnitTestBullet()
{
    INFO("Doing unit tests of bulletgame");
    TestTransformations();
    TestEntityFlags();
}
