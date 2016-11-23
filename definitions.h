#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
using namespace std;

fstream LoadFile(); //returns a file handle for a formatted input text file.
void PrintMatrix(int** matrix, int size); //Prints a 2D matrix
void PrintMatrix(int*** matrix, int size); //Prints a 3d Matrix
void FloydWarshall(int** &edgeMat, int** &pathMat); //Finds all pairs shortest distance
void DeriveShortestPaths(int** &pathMat, int*** &shortestPaths);
void DeriveTotalFlow(int*** &shortestPaths, int** &initialFlow, int** &totalFlow);