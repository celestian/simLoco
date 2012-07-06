#include "area.hh"
#include "area_pattern.hh"
#include <cmath>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <tuple>

using namespace std;
using namespace pval;


void printMatrix (const vector<string> & matrix) {

	for (auto line : matrix) {
		std::cout << line << endl;
	}

}

vector<string> copyMatrix (const vector<string> & matrixA) {

	vector<string> matrixB;

	for (string line : matrixA) {
		matrixB.push_back (line);
	}

	return matrixB;
}


bool isTherePattern (const vector<string> & matrix, const int pattern_id) {

	vector<string>::size_type mr, ii;
	string::size_type mc, jj;
	int res;
	
	vector<string> pattern = make_pattern (pattern_id);
	vector<string>::size_type pr = pattern.size(); // ToDo: This could be a public method of class pattern
	string::size_type pc = pattern[0].size();  // ToDo: This could be a public method of class pattern
	
	for(mr=0; mr < (matrix.size() - pr); ++mr) {
		for(mc=0; mc < (matrix[mr].size() - pc); ++mc) {
		////	
			for(res=0, ii=0; ii < pr; ++ii) {
				for(jj=0; jj < pc; ++jj) {
				////
					if (pattern[ii][jj] != ' '){
						if (pattern[ii][jj] != matrix[mr + ii][mc + jj]) {
							res++;
						}
					}										
				////
				}
			}
		////
		if (res == 0) return true;
		}
	}
	
	return false;
}

tuple<vector<string>::size_type, string::size_type>  findPatternXY (const vector<string> & matrix, const int pattern_id) {

	vector<string>::size_type mr, ii;
	string::size_type mc, jj;
	int res;
	
	vector<string> pattern = make_pattern (pattern_id);
	vector<string>::size_type pr = pattern.size(); // ToDo: This could be a public method of class pattern
	string::size_type pc = pattern[0].size();  // ToDo: This could be a public method of class pattern
	
	for(mr=0; mr < (matrix.size() - pr); ++mr) {
		for(mc=0; mc < (matrix[mr].size() - pc); ++mc) {
		////	
			for(res=0, ii=0; ii < pr; ++ii) {
				for(jj=0; jj < pc; ++jj) {
				////
					if (pattern[ii][jj] != ' '){
						if (pattern[ii][jj] != matrix[mr + ii][mc + jj]) {
							res++;
						}
					}										
				////
				}
			}
		////
		if (res == 0) return tuple<int, int> {mr, mc};
		}
	}
	
	return tuple<int, int> {-1, -1};
}

vector<string> replacePattern (const vector<string> & matrixA, const int pattern_id_A, const int pattern_id_B, int number) {

	vector<string> matrixB, patternA, patternB, patternC;
	string line;
	tuple<vector<string>::size_type, string::size_type> pattern_pos;
	vector<string>::size_type prB;
	string::size_type pcA, pcB, i, j;

	pattern_pos = findPatternXY (matrixA, pattern_id_A);
	patternA = make_pattern (pattern_id_A);
	patternB = make_pattern (pattern_id_B);

	// vector<string>::size_type prA = patternA.size();
	pcA = patternA[0].size();

	prB = patternB.size();
	pcB = patternB[0].size();
	
	j = 0;
	for (auto line : patternB) {
		if (line.find("xxxx") != string::npos) {
			line.replace(line.find("xxxx"), 4, to_string(number) );
		}			
		patternC.push_back (line);
	}
	patternB = copyMatrix (patternC);
			
	for (i=0; i < matrixA.size(); ++i) {
		line.clear();		
		
		line = matrixA[i];
		if ((get<0>(pattern_pos) <= i) and (j < prB)) {
			line.erase(get<1>(pattern_pos), pcA);
			line.insert(get<1>(pattern_pos), patternB[j++]);
		} else {
			line.insert(get<1>(pattern_pos) + pcA, pcB - pcA, line[get<1>(pattern_pos)]);
		}
		matrixB.push_back (line);
	}

	return matrixB;
}

vector<string> addWhiteSpace (const vector<string> & matrixA) {

	vector<string> matrixB;
	string end;
	string::size_type number = 0;
	
	for (string line : matrixA) {
		number = fmax (number, line.size());
	}

	for (string line : matrixA) {
		string lineB = line;
		if (line.size() < number)
			lineB.insert (line.size(), number - line.size(), ' ');
		matrixB.push_back (lineB);
	}

	end.insert (0, number, ' ');
	matrixB.push_back (end);
	matrixB.push_back (end);

	return matrixB;
}

Area::Area (const char* filename) {

	vector<string> matrixA, matrixB;
	ifstream file_handler (filename);
	string line;
	int junction_number = 1000;
	
	// loading
	if (file_handler.is_open()) {
		while (! file_handler.eof() ) {
			getline (file_handler,line);
			matrixA.push_back(line);
		}
		file_handler.close();
	}
	else cout << "Unable to open file";

	// analyzing
	matrixB = addWhiteSpace (matrixA);
	matrixA = copyMatrix (matrixB);

	printMatrix (matrixA);

	while ( isTherePattern (matrixA, LEFT_SWITCHES) ) {
		matrixB = replacePattern (matrixA, LEFT_SWITCHES, LEFT_DECOMP_SWITCHES, junction_number++);
		matrixA = copyMatrix (matrixB);
		
		printMatrix (matrixA);	
	}

	while ( isTherePattern (matrixA, LEFT_SWITCH_A) ) {
		matrixB = replacePattern (matrixA, LEFT_SWITCH_A, LEFT_DECOMP_SWITCH_A, junction_number++);
		matrixA = copyMatrix (matrixB);
		
		printMatrix (matrixA);
	}
	
	while ( isTherePattern (matrixA, RIGHT_SWITCHES) ) {
		matrixB = replacePattern (matrixA, RIGHT_SWITCHES, RIGHT_DECOMP_SWITCHES, junction_number++);
		matrixA = copyMatrix (matrixB);
		
		for (string line : matrixA) {
			std::cout << line << endl;
		}	
	}

	while ( isTherePattern (matrixA, RIGHT_SWITCH_A) ) {
		matrixB = replacePattern (matrixA, RIGHT_SWITCH_A, RIGHT_DECOMP_SWITCH_A, junction_number++);
		matrixA = copyMatrix (matrixB);
		
		for (string line : matrixA) {
			std::cout << line << endl;
		}	
	}

	printMatrix (matrixA);
}

