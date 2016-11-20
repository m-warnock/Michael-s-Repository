#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
using namespace std;

fstream LoadFile(); //returns a file handle for a formatted input file.
int N, A, B; //Global N = Matrix Size
int A; //Global A = Starting Node
int B; //Global B = Terminal Node