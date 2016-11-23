#include "definitions.h"

int N; //Global N = Matrix Size
int A; //Global A = Starting Node
int B; //Global B = Terminal Node

int main() {
	//Load in files and create initial Matrices

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
	//It is also more efficient to create 5 matricies in one pass than 5 passes with 1 matrix each.
	int** InitialEdgeMatrix = new int*[N];
	int** InitialFlowMatrix = new int*[N];
	int** PathMatrix = new int*[N];
	int** FlowPathMatrix = new int*[N];
	int** ShortestEdgeMatrix = new int*[N];
	int** TotalFlowMatrix = new int*[N];
	int*** AllpairsShortestPath = new int**[N]; //contains array of hop to hop shortest path from i to j
	int*** AllpairsSneakyPath = new int**[N];

	for (int i = 0; i < N; i++) {
		InitialEdgeMatrix[i] = new int[N];
		InitialFlowMatrix[i] = new int[N];
		PathMatrix[i] = new int[N];
		FlowPathMatrix[i] = new int[N];
		ShortestEdgeMatrix[i] = new int[N];
		TotalFlowMatrix[i] = new int[N];
		AllpairsShortestPath[i] = new int*[N];
		AllpairsSneakyPath[i] = new int*[N];

		// initialize to zero matrix;
		for (int j = 0; j < N; j++) {
			InitialEdgeMatrix[i][j] = ShortestEdgeMatrix[i][j] = 216000;
			InitialFlowMatrix[i][j] = 0;
			TotalFlowMatrix[i][j] = -1;
			PathMatrix[i][j] = -1;
			FlowPathMatrix[i][j] = -1;
		}
		InitialEdgeMatrix[i][i] = ShortestEdgeMatrix[i][i] = TotalFlowMatrix[i][i] = 0;
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
				InitialEdgeMatrix[node1 - 1][node2 - 1] = weight;
				ShortestEdgeMatrix[node1 - 1][node2 - 1] = weight;

				//Initial state path matrix
				PathMatrix[node1 - 1][node2 - 1] = node1;
			}
			if (matrixIndicator == "F") {
				InitialFlowMatrix[node1 - 1][node2 - 1] = weight;

			}
			ss.clear(); //clear stringstream
		}
	}

	fileHandle.close();

	PrintMatrix(InitialEdgeMatrix, N);
	PrintMatrix(PathMatrix, N);
	//PrintMatrix(FlowMatrix, N);
//***************************************************************************************
//Floyd-Warshall
	FloydWarshall(ShortestEdgeMatrix, PathMatrix);
	PrintMatrix(ShortestEdgeMatrix, N);
	PrintMatrix(PathMatrix, N);
//****************************************************************************************
	DeriveShortestPaths(PathMatrix, AllpairsShortestPath); 
	PrintMatrix(AllpairsShortestPath, N);
	DeriveTotalFlow(AllpairsShortestPath, InitialFlowMatrix, TotalFlowMatrix);
	
// Find Sneaky Path
	//prepTotalFlow for FloydWarshall
	//inject relative infinity
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			if (TotalFlowMatrix[i][j] == -1)
				TotalFlowMatrix[i][j] = 216000;
		}
	}
	PrintMatrix(TotalFlowMatrix, N);
	FloydWarshall(TotalFlowMatrix, FlowPathMatrix);
	PrintMatrix(FlowPathMatrix, N);
	DeriveShortestPaths(TotalFlowMatrix, AllpairsSneakyPath);
	PrintMatrix(AllpairsSneakyPath, N);
	return 0;
}

