// Michael Warnock ID: 16120478
// CS 404 
// Prof: Van De Leifvoort
// Sneaky Path Project
// Due 26 November 2016
//********************************************************************************************************
#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <iomanip>
using namespace std;

fstream LoadFile(); //returns a file handle for a formatted input text file.
int** CreateMatrix(int initialvalue, int size);
void PrintMatrix(int** matrix, int size, string message=""); //Prints a 2D matrix
void PrintMatrix(float** matrix, int size, string message=""); //Prints avg matrix
void PrintMatrix(int*** matrix, int size, string message = ""); //Prints a 3d Matrix
void FloydWarshall(int** &edgeMat, int** &pathMat); //Finds all pairs shortest distance
void DeriveShortestPaths(int** &pathMat, int*** &shortestPaths);
void DeriveTotalFlow(int*** &shortestPaths, int** &initialFlow, int** &totalFlow, int** initialPath);
void MinMaxAvgSneakyPath(int*** allPaths, int** flowPerEdge, int** &minmat, int** &maxmat, float** &avgmat);