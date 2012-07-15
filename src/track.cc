#include "track.hh"
#include <iostream>

using namespace std;


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
