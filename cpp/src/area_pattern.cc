#include "area_pattern.hh"

using namespace std;
using namespace pval;

vector<string> make_pattern (const int pattern) {

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
			line.append ("  {0|e:xxxx|B}-");
			pattern_matrix.push_back (line);
			line.clear();
			line.append ("-{a|e:xxxx|A}--");
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
			line.append ("-{b|e:xxxx|0}  ");
			pattern_matrix.push_back (line);
			line.clear();
			line.append ("--{a|e:xxxx|A}-");
			pattern_matrix.push_back (line);
			break;
		}
	}
	
	return pattern_matrix;
}
