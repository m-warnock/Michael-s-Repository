// Michael Warnock ID: 16120478
// CS 404 
// Prof: Van De Leifvoort
// Sneaky Path Project
// Due 26 November 2016
//********************************************************************************************************
#include "definitions.h"

int N; //Global N = Matrix Size
int A; //Global A = Starting Node
int B; //Global B = Terminal Node

int main() {
	//Load in files and create initial Matrices

	fstream fileHandle = LoadFile();


	// make sure output file is clean
	remove("SneakyPathOutput.txt");
	fstream output;
	output.open("SneakyPathOutput.txt", fstream::app);

	
	//Get ready to read from file
	string line, matrixIndicator, comma;
	int node1, node2, weight;
	stringstream ss;



	//Read first line in data file
	getline(fileHandle, line);
	ss << line;
	ss >> N >> comma >> A >> comma >> B;
	ss.clear(); //Ready stringstream for next line
	output << "Size: " << N << "    From: " << A << "   To: " << B << endl;
	output.close();
	//Dynamically allocate matrices now that N is read in from the file.

	int** InitialEdgeMatrix = CreateMatrix(216000, N); // Edge data read in from file, Matrix E
	int** InitialFlowMatrix = CreateMatrix(0, N); // Flow data read in from file, Matrix F
	int** InitialPathMatrix = CreateMatrix(-1, N); // Derived from edge data each data point represents the node it came from
	int** PathMatrix = CreateMatrix(-1, N);// Duplicate of Initial Path Matrix intended to be manipulated
	int** ShortestEdgeMatrix = CreateMatrix(216000, N); //This is a duplicate of InitialEdgeMatrix that is intended to be manupulated. Also Matrix E
	int** TotalFlowMatrix = CreateMatrix(0, N); // Matrix L
	int** MinSneakyPath = CreateMatrix(0, N); // All pairs minimum path
	int** MaxSneakyPath = CreateMatrix(0, N); // All pairs maximum path
	float** AverageSneakyPath = new float*[N]; // All pairs Maximum path
	int*** AllPairsShortestPaths = new int**[N]; //contains array of hop to hop shortest path from i to j
	int*** AllPairsSneakyPaths = new int**[N]; //contains array of hop to hop least traffic path from i to j

	for (int i = 0; i < N; i++) {
		AverageSneakyPath[i] = new float[N];
		AllPairsShortestPaths[i] = new int*[N];
		AllPairsSneakyPaths[i] = new int*[N];
		
		//Initialize diagonal state for the following matricies
		InitialEdgeMatrix[i][i] = ShortestEdgeMatrix[i][i] = TotalFlowMatrix[i][i] = 0;
		
		// initial state for Avg matrix
		for (int j = 0; j < N; j++) {
			AverageSneakyPath[i][j] = 0;
			//Since the 2 3D matricies have variable length in their 3rd dimension, they will be allocated later.
		}
	}

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
				InitialPathMatrix[node1 - 1][node2 - 1] = node1;
				PathMatrix[node1 - 1][node2 - 1] = node1;
			}
			if (matrixIndicator == "F") {
				InitialFlowMatrix[node1 - 1][node2 - 1] = weight;
			}
			ss.clear(); //clear stringstream
		}
	}

	fileHandle.close();

	PrintMatrix(InitialEdgeMatrix, N, "Matrix E created from the input file where 216000 represents infinity:");
	PrintMatrix(InitialFlowMatrix, N, "Matrix F created from the input file:");
	PrintMatrix(InitialPathMatrix, N, "Initial Path Matrix derived from matrix E where the number represents the previous node (-1 means there is no known path):");

	//***************************************************************************************
	//Floyd-Warshall
	FloydWarshall(ShortestEdgeMatrix, PathMatrix);
	PrintMatrix(ShortestEdgeMatrix, N, "This is the Shortest Edge Matrix. Derived by applying the Floyd-Warshall algorithm on matrix E:");
	PrintMatrix(PathMatrix, N, "The Shortest Path Matrix is also derived by applying Floyd-Warshall on the Initial Path Matrix concurrently with matrix E.");
	//****************************************************************************************
	DeriveShortestPaths(PathMatrix, AllPairsShortestPaths); 
	PrintMatrix(AllPairsShortestPaths, N, "The shortest edge route to go from any source to any destination, derived by tracing the Shortest Path Matrix");
	//****************************************************************************************
	DeriveTotalFlow(AllPairsShortestPaths, InitialFlowMatrix, TotalFlowMatrix, InitialPathMatrix);
	PrintMatrix(TotalFlowMatrix, N, "Matrix L, The total flow on each edge is derived by tracing the previously listed matrix and accumulating path flow from the Initial Flow Matrix F. (216000 still = infinity)");
	// Find Sneaky Path
	PrintMatrix(InitialPathMatrix, N, "This is the Initial Path Matrix again, It is used along side Matrix L and run through Floyd-Warshall.");
	FloydWarshall(TotalFlowMatrix, InitialPathMatrix);
	PrintMatrix(TotalFlowMatrix, N, "The all-pairs-fewest-cars Matrix or Flow Per Edge Matrix.");
	//****************************************************************************************
	PrintMatrix(InitialPathMatrix, N, "The Sneaky Path Matrix(least encountered traffic path), where the number represents the previous node.");
	DeriveShortestPaths(InitialPathMatrix, AllPairsSneakyPaths);
	PrintMatrix(AllPairsSneakyPaths, N, "The All Pairs Sneaky Paths Matrix showing hop to hop. Derived from tracing previous Matrix.");
	//****************************************************************************************
	MinMaxAvgSneakyPath(AllPairsSneakyPaths, TotalFlowMatrix, MinSneakyPath, MaxSneakyPath, AverageSneakyPath);
	PrintMatrix(MinSneakyPath, N, "All pairs minimum traffic edge within the total sneaky path.");
	PrintMatrix(MaxSneakyPath, N, "All pairs maximum traffic edge within the total sneaky path.");
	PrintMatrix(AverageSneakyPath, N, "The average traffic encountered per hop for all sneaky paths.");
	
	
	// output for A to B specifically
	output.open("SneakyPathOutput.txt", fstream::app);
	int i = 0;
	int temp;
	output << endl;
	output << "The sneaky path from " << A << " to " << B << " is: ";
	while (AllPairsSneakyPaths[A-1][B-1][i] > 0) {
		output << AllPairsSneakyPaths[A - 1][B - 1][i] << " ";
		i++;
	}
	output << endl << "The Minimum path flow is: " << MinSneakyPath[A - 1][B - 1];
	output << endl << "The Maximum path flow is: " << MaxSneakyPath[A - 1][B - 1];
	output << endl << "The Average traffic per edge is: " << AverageSneakyPath[A - 1][B - 1];
	output.close(); 
	cout << "Output was placed in SneakyPathOutput.txt in the local directory." << endl << endl;
	cout << "The shortest possible answer is at the very bottom of the file."<< endl;
	system("pause");
	return 0;
}

