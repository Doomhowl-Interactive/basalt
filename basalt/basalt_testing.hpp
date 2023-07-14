#pragma once
#include <string>

#include "basalt_utils.hpp"

// =====================================
// Small over-engineered test framework
// =====================================

void EndTest(std::string name, std::string description, bool succeeded);

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