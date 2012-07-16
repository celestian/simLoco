#include <cluttermm.h>
#include <vector>
#include <fstream>
#include <cstdio>
#include "track.hh"
#include "gui_track.hh"

using namespace std;

/*

	DSL-------------------------------------------------------------------------
	
	type 1 kolej:
	id, lenght, 0, <id, 0, 0, id>, 0
	
	
	DSG-------------------------------------------------------------------------

	type 1 kolej:
	id, x, y, x-size
	
	type 1 vyhýbka -< dolů:
	id, x, y
	
*/

int main(int argc, char** argv)
{

	Clutter::init(&argc, &argv);

	// Get the stage and set its size and color:
	const Glib::RefPtr<Clutter::Stage> stage = Clutter::Stage::get_default();
	stage->set_size(400, 200);
	stage->set_color(Clutter::Color(0, 0, 0, 255)); // black

	string descp;
	vector<Track> dsl;
	vector<TrackGUI> dsg;

	if (argc < 3) {
		printf("Error: No input file given!\n");
		printf("usage: dsl dsl_file dsg_file\n");
		return 1;
	}
	
	ifstream file_dsl (argv[1]);
	if (file_dsl.is_open()) {
		while (! file_dsl.eof() ) {
			getline (file_dsl, descp);
			if (descp.size() > 0 ) {
				switch (stoi (descp)) {
					case 1: {
						Rail rail (descp);
						rail.print();
						dsl.push_back(rail);						
						break;				
					}
					case 2: {
						Junction junction (descp);
						junction.print();
						dsl.push_back(junction);						
						break;				
					}
				}
			}
		}
		file_dsl.close();
	} else {
		printf("Error: Unable to open file!\n");
		return 1;	
	}

	ifstream file_dsg (argv[2]);
	if (file_dsg.is_open()) {
		while (! file_dsg.eof() ) {
			getline (file_dsg, descp);
			if (descp.size() > 0 ) {
				switch (stoi (descp)) {
					case 1: {

						int id, xsize;
						float x, y;
						string::size_type pp, pos;
						
						pp = 0; pos = 0;
						id = stoi (descp, &pos);
						pp += pos;
						id = stoi (descp.substr(pp), &pos);
						pp += pos;
						x = stoi (descp.substr(pp), &pos);
						pp += pos;
						y = stoi (descp.substr(pp), &pos);
						pp += pos;
						xsize = stoi (descp.substr(pp), &pos);
						
						
						RailGUI railg (id, x, y, xsize);
						railg.show (stage);
						dsg.push_back (railg);						
						break;				
					}
					case 2: {
					
						int id;
						float x, y;
						string::size_type pp, pos;
						
						pp = 0; pos = 0;
						id = stoi (descp, &pos);
						pp += pos;
						id = stoi (descp.substr(pp), &pos);
						pp += pos;
						x = stoi (descp.substr(pp), &pos);
						pp += pos;
						y = stoi (descp.substr(pp), &pos);
											
						JunctionGUI junctiong (id, x, y);
						junctiong.show (stage);
						dsg.push_back (junctiong);						
						break;				
					}
				}
			}
		}
		file_dsg.close();
	} else {
		printf("Error: Unable to open file!\n");
		return 1;	
	}

	// Show the stage:
	stage->show();

	// Start the main loop, so we can respond to events:
	Clutter::main();

  return 0;
}

