#pragma once
#include <string>

#include "basalt_utils.hpp"

namespace basalt {
using namespace std;

// =====================================
// Small over-engineered test framework
// =====================================

void EndTest(string name, string description, bool succeeded);

#define TEST(N) \
 void Test##N() \
 { \
  const char* NAME = #N;
#define END \
 EndTest(NAME, "", true); \
 }  // succeeds

#define CHECK(X, I) \
 if (!(X)) { \
  EndTest(NAME, I, false); \
  return; \
 }  // fails

// =====================================

void RunUnitTests();

}  // namespace basalt