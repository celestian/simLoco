#include <cstdio>
#include <cmath>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <tuple>

#include "area_pattern.hh"


using namespace std;
using namespace pval;


class Track {

	public:
	
		Track (const std::string & descp);
		int getId () { return id; };
		char getType () { return type; };
		int getLenght () { return lenght; };
		void print () {	 printf("%s\t|  %i\t%c\t%i\t%i\t%i\t%i\t%i\t%i\t%i\n", title.c_str(), id, type, lenght, back_left, back_direct, back_right, forward_left, forward_direct, forward_right); };
		
		int back_left;
		int back_direct;
		int back_right;

		int forward_left;
		int forward_direct;
		int forward_right;


	private:
		
		int id;
		char type;
		std::string title;
		int lenght;



};


Track::Track (const string & descp) {
	
	id = stoi(descp.substr(4, 4));
	type = descp.at(2);
	
	if (type == 'r') {
		title = "rail xxxx";
		lenght = 800;
	}
	if (type == 'e') {
		title = "switch xxxx";
		lenght = 0;
	}
	title.replace(title.find("xxxx"), 4, to_string(id) );
	
	back_left = 0;
	back_direct = 0;
	back_right = 0;

	forward_left = 0;
	forward_direct = 0;
	forward_right = 0;
}



int main(int argc, char **argv) {

	vector<string> matrixA, matrixB;
	ifstream file_handler (argv[1]);
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

	while ( isTherePattern (matrixA, LEFT_SWITCHES) ) {
		matrixB = replacePattern (matrixA, LEFT_SWITCHES, LEFT_DECOMP_SWITCHES, junction_number++);
		matrixA = copyMatrix (matrixB);
	}
	
	while ( isTherePattern (matrixA, LEFT_SWITCH_A) ) {
		matrixB = replacePattern (matrixA, LEFT_SWITCH_A, LEFT_DECOMP_SWITCH_A, junction_number++);
		matrixA = copyMatrix (matrixB);
	}
	
	while ( isTherePattern (matrixA, LEFT_DOWN_SWITCHES) ) {
		matrixB = replacePattern (matrixA, LEFT_DOWN_SWITCHES, LEFT_DOWN_DECOMP_SWITCHES, junction_number++);
		matrixA = copyMatrix (matrixB);
	}
	
	while ( isTherePattern (matrixA, LEFT_DOWN_SWITCH_A) ) {
		matrixB = replacePattern (matrixA, LEFT_DOWN_SWITCH_A, LEFT_DOWN_DECOMP_SWITCH_A, junction_number++);
		matrixA = copyMatrix (matrixB);
	}
	
	while ( isTherePattern (matrixA, RIGHT_SWITCHES) ) {
		matrixB = replacePattern (matrixA, RIGHT_SWITCHES, RIGHT_DECOMP_SWITCHES, junction_number++);
		matrixA = copyMatrix (matrixB);
	}
	
	while ( isTherePattern (matrixA, RIGHT_SWITCH_A) ) {
		matrixB = replacePattern (matrixA, RIGHT_SWITCH_A, RIGHT_DECOMP_SWITCH_A, junction_number++);
		matrixA = copyMatrix (matrixB);
	}
	
	matrixB.clear(); // udělat lépe!
	// final touch
	for (auto line : matrixA) {
		while (line.find("  ") != string::npos) {
			line.replace(line.find("  "), 2, " ");
		}
		while (line.find("}-{") != string::npos) {
			line.replace(line.find("}-{"), 3, "}{");
		}
		while (line.find("--") != string::npos) {
			line.replace(line.find("--"), 2, "-");
		}
		while (line.find("-") != string::npos) {
			line.replace(line.find("-"), 1, "{d|r:xxxx|d}");
			line.replace(line.find("xxxx"), 4, to_string(junction_number++)); 
		}		
		
		matrixB.push_back (line);
	}
	matrixA.clear();
	matrixA = copyMatrix (matrixB);
	
	// numbering
	vector<Track> tracks;
	vector<int> ids;

	string hmm;
	for (auto line : matrixA) {
		string::size_type aa, bb, ii;
		ids.clear();
		ids.push_back(0);
		bb = 0;
		while (string::npos != line.find_first_of("{", bb)) {
			aa = line.find_first_of("{", bb);
			bb = line.find_first_of("}", aa);
			hmm = line.substr( aa+1, bb-aa-1);
			Track hmm_track (hmm);

			int isexist = 0;
			for (auto item : tracks)	{
				if (hmm_track.getId() == item.getId()) isexist++;
			}
			if(isexist == 0) tracks.push_back(hmm_track);
			ids.push_back (hmm_track.getId());
		}
		ids.push_back (0);
		// directions
		ii = 0;
		int pid = 1;
		while (ii < line.size()) {
			if (line.at(ii) == '{') {
				for (auto &item : tracks)	{
					if (ids[pid] == item.getId()) {
						if (line.at(ii+1) == 'l') item.back_left = ids[pid-1];
						if (line.at(ii+1) == 'd') item.back_direct = ids[pid-1];
						if (line.at(ii+1) == 'r') item.back_right = ids[pid-1];

						if (line.at(ii+10) == 'l') item.forward_left = ids[pid+1];
						if (line.at(ii+10) == 'd') item.forward_direct = ids[pid+1];
						if (line.at(ii+10) == 'r') item.forward_right = ids[pid+1];
					}
				}
				//aa = line.find("}", ii+4);
				ii += 11;
				++pid;
			}
			++ii;	
		}
	}
/*
	printf("title\t\t|  id\ttype\tlenght\tBL\tBD\tBR\tFL\tFD\tFR\n");
	printf("================|====================================================================\n");
	for(auto item : tracks) {
		item.print();
	}
*/
	
	FILE * file = fopen(argv[2], "w");
	int znak;
	for (auto item : tracks) {
		if (item.getType() == 'r') znak = 1;
		if (item.getType() == 'e') znak = 2;
		
		fprintf (file, "%i\t%i\t%i\t%i\t%i\t%i\t%i\t%i\t%i\n", znak, item.getId(), item.getLenght(), item.back_left, item.back_direct, item.back_right, item.forward_left, item.forward_direct, item.forward_right);
	}
	fclose (file);
	
	
	return 0;
}

