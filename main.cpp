#include "definitions.h"


int main() {
	fstream fileHandle = LoadFile();
	string line, token, matrixIndicator, comma;
	stringstream ss;
	int node1, node2, weight;

	//account for first line in data file.
	getline(fileHandle, line);
	ss << line;
	ss >> N >> comma >> A >> comma >> B;
	ss.clear();

	//Write code to create an empty Matrix (Probably a function)
	//Dynamic allocation


	//Read data from the file and load it into the matrix
	while (getline(fileHandle, line)) {
		ss << line;
		ss >> matrixIndicator >> node1 >> comma >> node2 >> comma >> weight;
		matrixIndicator.pop_back(); //remove the comma
		// matrix loading code goes here.
		
		ss.clear();
	}

	
		
	fileHandle.close();
		

	return 0;
}

