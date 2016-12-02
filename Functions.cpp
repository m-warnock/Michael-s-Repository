// Michael Warnock ID: 16120478
// CS 404 
// Prof: Van De Leifvoort
// Sneaky Path Project
// Due 26 November 2016
//********************************************************************************************************
#include "definitions.h"

extern int N;
extern int A;
extern int B;


fstream LoadFile() {
	fstream graphFile;
	string fileName;
	bool goodFile = false;

	//Prompt for file and check validity
	while (!goodFile) {
		cout << "Please enter a properly formatted file you would like to load:" << endl;
		cin >> fileName;
		graphFile.open(fileName);

		if (graphFile.fail()) {
			cout << "There was a problem with the file." << endl;
		}
		else goodFile = true;
	}
	cout << "The file was successfully loaded." << endl << endl;
	return graphFile;
}

//creates an int matrix
int** CreateMatrix(int initialvalue, int size) {
	int** Matrix = new int*[size];
	for (int i = 0; i < size; i++) {
		Matrix[i] = new int[size];
		for (int j = 0; j < size; j++) {
			Matrix[i][j] = initialvalue;
		}
	}
	return Matrix;
}
//print a 2D matrix to console
void PrintMatrix(int** matrix, int size, string message) {
	ofstream myfile;
	myfile.open("SneakyPathOutput.txt", ofstream::app);
	myfile << endl;
	myfile << message << endl;
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			myfile << matrix[i][j] << "  ";
			if (j == size - 1)
				myfile << endl;
		}
	}
	myfile.close();
}


//print float matrix
void PrintMatrix(float** matrix, int size, string message) {
	ofstream myfile;
	myfile.open("SneakyPathOutput.txt", ofstream::app);
	myfile << endl;
	myfile << message << endl;
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			myfile << setprecision(2)<< fixed << matrix[i][j] << "  ";
			if (j == size - 1)
				myfile << endl;
		}
	}
	myfile.close();
}


//print a 3D matrix to console
void PrintMatrix(int*** matrix, int size, string message) {
	ofstream myfile;
	myfile.open("SneakyPathOutput.txt", ofstream::app);
	myfile << endl << message;
	int k;
	for (int i = 0; i < size; i++) {
		myfile << endl;
		for (int j = 0; j < size; j++) {
			k = 0;
			myfile << "(";
			while (matrix[i][j][k] > 0) {
				if (k != 0)
					myfile << " ";
				myfile << matrix[i][j][k];
				k++;
			}
			myfile << ")   ";
		}
	}
	myfile << endl;
	myfile.close();
}

void FloydWarshall(int** &edgeMat, int** &pathMat) {
// Creates all pairs shortest distance Matrix and derives a path matrix from it
	for (int k = 0; k < N; k++) {
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				if (edgeMat[i][j] > edgeMat[i][k] + edgeMat[k][j]) {
					edgeMat[i][j] = edgeMat[i][k] + edgeMat[k][j];
					pathMat[i][j] = pathMat[k][j];
				}
			}
		}
	}
}


void DeriveShortestPaths(int** &pathMat, int*** &shortestPaths) {
	//Use Path Matrix to create all pairs shortest path 3d matrix
	vector<int> tmp;
	int k, z;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			if (pathMat[i][j] != -1) {
				z = j;
				
				do {//watch out for the off by ones! the matrix is displayed with 1 as its lowest matrix.
					//but is obviously indexed starting at 0 in memory
					if (tmp.empty())
						tmp.push_back(z + 1);

					if (pathMat[i][z] == i + 1) {
						tmp.push_back(pathMat[i][z]);
						break;
					}//The path is placed on the vector in reverse order.
					tmp.push_back(pathMat[i][z]);
					z = pathMat[i][z]-1;
					if(pathMat[i][z] == i + 1)
						tmp.push_back(pathMat[i][z]);
				} while (pathMat[i][z] != i + 1);
			
				//Since this 3d array holds no data, it must first be dynamically allocated.
				//Doing it here also saves alot of memory; the vector helped with that problem (rather than having an array of size N).
				shortestPaths[i][j] = new int[tmp.size()];
				k = 0;
				while (!tmp.empty()) {
					shortestPaths[i][j][k] = tmp.back();
					tmp.pop_back();
					k++;
				}
			}
			else {
				shortestPaths[i][j] = new int[1];
				shortestPaths[i][j][0] = i + 1;
			}
		}
	}
}


void DeriveTotalFlow(int*** &shortestPaths, int** &initialFlow, int** &totalFlow, int** initialPath){
	//iterate through all of the flow routes one edge at a time to accumulate total flow.
	//the initial path is used to determine where the NA's go on the totalFlow matrix and place infinity (216000) in that position
	int from, to, k, singleEdgeFlow;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			k = 0;
			//since the arrays vary in size, the k + 1th index (look ahead 1) must be checked to see if that position has been initialized as uninitialized data is large negative numbers
			while (shortestPaths[i][j][k] && shortestPaths[i][j][k + 1] > 0) {
				from = shortestPaths[i][j][k];
				to = shortestPaths[i][j][k+1];
				singleEdgeFlow = initialFlow[i][j]; 
				if (totalFlow[from - 1][to - 1] == -1)
					totalFlow[from - 1][to - 1]++;
				totalFlow[from - 1][to - 1] += singleEdgeFlow; //account for off by 1 indexing
				k++;
			}
			if (initialPath[i][j] == -1 && i != j) {
				totalFlow[i][j] = 216000;
			}
		}
	}
}


void MinMaxAvgSneakyPath(int*** allPaths, int** flowPerEdge, int** &minmat, int** &maxmat, float** &avgmat) {
	int min, max, k;
	float avg;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			k = 0;
			min = flowPerEdge[i][j];
			max = 0;
			avg = 0.0;
			while(allPaths[i][j][k+1] > 0) {

				if (flowPerEdge[allPaths[i][j][k] - 1][allPaths[i][j][k+1] - 1] < min)
					min = flowPerEdge[allPaths[i][j][k] - 1][allPaths[i][j][k + 1] -1];
				if (flowPerEdge[allPaths[i][j][k] - 1][allPaths[i][j][k + 1] - 1] > max)
					max = flowPerEdge[allPaths[i][j][k] - 1][allPaths[i][j][k + 1] - 1];
				avg += flowPerEdge[allPaths[i][j][k] - 1][allPaths[i][j][k + 1] - 1];
				k++;
			}

			minmat[i][j] = min;
			maxmat[i][j] = max;
			if (avg != 0)
				avgmat[i][j] = avg / k;
			else
				avgmat[i][j] = 0;
		}
	}
}
