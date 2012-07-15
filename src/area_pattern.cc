#include "area_pattern.hh"

#include <iostream>
#include <cmath>

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

vector<string> makePattern (const int pattern) {

	vector<string> pattern_matrix;
	string line;

	switch (pattern) {
		case LEFT_SWITCHES: {
			line.clear();
			line.append ("  /-");
			pattern_matrix.push_back (line);
			line.clear();
			line.append (" /--");
			pattern_matrix.push_back (line);
			line.clear();
			line.append ("/---");
			pattern_matrix.push_back (line);
			break;		
		}
		case LEFT_DECOMP_SWITCHES: {
			line.clear();
			line.append ("    /-");
			pattern_matrix.push_back (line);
			line.clear();
			line.append (" /-/--");
			pattern_matrix.push_back (line);
			line.clear();
			line.append ("/-----");
			pattern_matrix.push_back (line);
			break;
		}
		case LEFT_SWITCH_A: {
			line.clear();
			line.append ("  /-");
			pattern_matrix.push_back (line);
			line.clear();
			line.append ("-/--");
			pattern_matrix.push_back (line);
			break;
		}
		case LEFT_DECOMP_SWITCH_A: {
			line.clear();
			line.append ("  {0|e:xxxx|l}-");
			pattern_matrix.push_back (line);
			line.clear();
			line.append ("-{d|e:xxxx|d}--");
			pattern_matrix.push_back (line);
			break;
		}

		case RIGHT_SWITCHES: {
			line.clear(); 
			line.append ("-\\  ");
			pattern_matrix.push_back (line);
			line.clear();
			line.append ("--\\ ");
			pattern_matrix.push_back (line);
			line.clear();
			line.append ("---\\");
			pattern_matrix.push_back (line);
			break;		
		}
		case RIGHT_DECOMP_SWITCHES: {
			line.clear();
			line.append ("-\\    ");
			pattern_matrix.push_back (line);
			line.clear();
			line.append ("--\\-\\ ");
			pattern_matrix.push_back (line);
			line.clear();
			line.append ("-----\\");
			pattern_matrix.push_back (line);
			break;
		}
		case RIGHT_SWITCH_A: {
			line.clear();
			line.append ("-\\  ");
			pattern_matrix.push_back (line);
			line.clear();
			line.append ("--\\-");
			pattern_matrix.push_back (line);
			break;
		}
		case RIGHT_DECOMP_SWITCH_A: {
			line.clear();
			line.append ("-{l|e:xxxx|0}  ");
			pattern_matrix.push_back (line);
			line.clear();
			line.append ("--{d|e:xxxx|d}-");
			pattern_matrix.push_back (line);
			break;
		}

		case LEFT_DOWN_SWITCHES: {
			line.clear();
			line.append ("---/-");
			pattern_matrix.push_back (line);
			line.clear();
			line.append ("--/  ");
			pattern_matrix.push_back (line);
			line.clear();
			line.append ("-/   ");
			pattern_matrix.push_back (line);
			break;		
		}
		case LEFT_DOWN_DECOMP_SWITCHES: {
			line.clear();
			line.append ("-----/");
			pattern_matrix.push_back (line);
			line.clear();
			line.append ("--/-/ ");
			pattern_matrix.push_back (line);
			line.clear();
			line.append ("-/    ");
			pattern_matrix.push_back (line);
			break;
		}
		case LEFT_DOWN_SWITCH_A: {
			line.clear();
			line.append ("--/-");
			pattern_matrix.push_back (line);
			line.clear();
			line.append ("-/  ");
			pattern_matrix.push_back (line);
			break;
		}
		case LEFT_DOWN_DECOMP_SWITCH_A: {
			line.clear();
			line.append ("--{d|e:xxxx|d}-");
			pattern_matrix.push_back (line);
			line.clear();
			line.append ("-{r|e:xxxx|0}  ");
			pattern_matrix.push_back (line);
			break;
		}

	}
	
	return pattern_matrix;
}

bool isTherePattern (const vector<string> & matrix, const int pattern_id) {

	vector<string>::size_type mr, ii;
	string::size_type mc, jj;
	int res;
	
	vector<string> pattern = makePattern (pattern_id);
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
	
	vector<string> pattern = makePattern (pattern_id);
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
	patternA = makePattern (pattern_id_A);
	patternB = makePattern (pattern_id_B);

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

