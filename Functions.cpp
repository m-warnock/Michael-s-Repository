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
		cout << "Please enter a properly formatted file you would like to load (default is 'Graph.txt'):" << endl;
		cin >> fileName;
		graphFile.open(fileName);

		if (graphFile.fail()) {
			cout << "There was a problem with the file." << endl;
		}
		else goodFile = true;
	}
	cout << "The file was successfully loaded." << endl;
	return graphFile;
}


void PrintMatrix(int** matrix, int size) {
	cout << endl;
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			cout << matrix[i][j] << " ";
			if (j == size - 1)
				cout << endl;
		}
	}
}

void PrintMatrix(int*** matrix, int size) {
	int k;
	for (int i = 0; i < size; i++) {
		cout << endl;
		for (int j = 0; j < size; j++) {
			k = 0;
			cout << "(";
			while (matrix[i][j][k] > 0) {
				if (k != 0)
					cout << " ";
				cout << matrix[i][j][k];
				k++;
			}
			cout << ")   ";
		}
	}
	cout << endl;
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
					}
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


void DeriveTotalFlow(int*** &shortestPaths, int** &initialFlow, int** &totalFlow){
	//iterate through all of the flow routes one edge at a time to accumulate total flow.
	int from, to, k, singleEdgeFlow;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			k = 0;
			while (shortestPaths[i][j][k] && shortestPaths[i][j][k + 1] > 0) {
				from = shortestPaths[i][j][k];
				to = shortestPaths[i][j][k+1];
				singleEdgeFlow = initialFlow[i][j]; 
				if (totalFlow[from - 1][to - 1] == -1)
					totalFlow[from - 1][to - 1]++;
				totalFlow[from - 1][to - 1] += singleEdgeFlow; //account for off by 1 indexing
				k++;
			}
		}
	}
}
