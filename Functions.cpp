#include "definitions.h"

fstream LoadFile() {
	fstream graphFile;
	string fileName;
	bool goodFile = false;

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