#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
using namespace std;

fstream LoadFile(); //returns a file handle for a formatted input text file.
void PrintMatrix(int** matrix, int size); //Prints a matrix
