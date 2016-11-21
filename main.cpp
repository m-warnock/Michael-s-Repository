#include "definitions.h"

int N; //Global N = Matrix Size
int A; //Global A = Starting Node
int B; //Global B = Terminal Node

int main() {

	//Load in files and create initial Matrices
	//********************************************************************************

	fstream fileHandle = LoadFile();
	string line, matrixIndicator, comma;
	int node1, node2, weight;
	stringstream ss;
	

	//Read first line in data file
	getline(fileHandle, line);
	ss << line;
	ss >> N >> comma >> A >> comma >> B;
	
	//create an empty Matrix for the Edge and flow
	//(Dynamic allocation)
	//since called only once, there is no reason to functionalize a createMatrix function.
	//It is also more efficient to create 3 matricies in one pass than 3 passes with 1 matrix each.
	int** EdgeMatrix = new int*[N];
	int** FlowMatrix = new int*[N];
	int** PathMatrix = new int*[N];
	int** AllPairsShortestPath = new int*[N];
	
	for (int i = 0; i < N; i++) {
		EdgeMatrix[i] = new int[N];
		FlowMatrix[i] = new int[N];
		PathMatrix[i] = new int[N];
		AllPairsShortestPath[i] = new int[N];
		
		// initialize to zero matrix;
		for (int j = 0; j < N; j++) {
			EdgeMatrix[i][j]= AllPairsShortestPath[i][j] = 216000;
			FlowMatrix[i][j] = 0;
			PathMatrix[i][j] = -1;
		}
		EdgeMatrix[i][i] = AllPairsShortestPath[i][i] = 0;
	}

	ss.clear(); //Ready stringstream for next line

	//Read data from the rest of the file and load it into the matrix
	while (getline(fileHandle, line)) {
		if (line != "") { //Skips empty lines
			ss << line;
			ss >> matrixIndicator >> node1 >> comma >> node2 >> comma >> weight;
			matrixIndicator.pop_back(); //remove the comma
			
			// Load data for the Edge or Flow Matrix
			if (matrixIndicator == "E") {
				EdgeMatrix[node1 - 1][node2 - 1] = weight;
				AllPairsShortestPath[node1 - 1][node2 - 1] = weight;

				//Initial state path matrix
				PathMatrix[node1 - 1][node2 - 1] = node1;
			}
			if (matrixIndicator == "F") {
				FlowMatrix[node1 - 1][node2 - 1] = weight;
				
			}
			ss.clear(); //clear stringstream
		}
	}

	fileHandle.close();
	//PrintMatrix(EdgeMatrix, N);
	PrintMatrix(PathMatrix, N);
	//PrintMatrix(FlowMatrix, N);
//***************************************************************************************
//Floyd-Warshall
	for (int k = 0; k < N; k++) {
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				if (AllPairsShortestPath[i][j] > AllPairsShortestPath[i][k] + AllPairsShortestPath[k][j]) {
					AllPairsShortestPath[i][j] = AllPairsShortestPath[i][k] + AllPairsShortestPath[k][j];
					PathMatrix[i][j] = k;
				}
			}
		}
	}
	//PrintMatrix(AllPairsShortestPath, N);
	PrintMatrix(PathMatrix, N);
	return 0;
}

