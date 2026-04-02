#ifndef UTILS_H
#define UTILS_H

#include <string>

// Create a test binary file to load into ROM
void createTestBin(const std::string& filename);

// Basic CHIP8 module test, loads and prints ROM
void chipTest();

// Basic platform test, displays a checkerboard pattern
void platformTest();

#endif
