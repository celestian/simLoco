#include "track.hh"
#include <cstdio>

using namespace std;


Track::Track (const string & descp) {

	string::size_type pp, pos;
	pp = 0; pos = 0;
	type = stoi (descp, &pos);
	pp += pos;
	id = stoi (descp.substr(pp), &pos);
	pp += pos;
	lenght = stoi (descp.substr(pp), &pos);
	pp += pos;
	back_left = stoi (descp.substr(pp), &pos);
	pp += pos;
	back_direct = stoi (descp.substr(pp), &pos);
	pp += pos;
	back_right = stoi (descp.substr(pp), &pos);
	pp += pos;
	forward_left = stoi (descp.substr(pp), &pos);
	pp += pos;
	forward_direct = stoi (descp.substr(pp), &pos);
	pp += pos;
	forward_right = stoi (descp.substr(pp), &pos);
}

void Rail::print() {
	
	printf(": RAIL #%i --------------------\n", id);
	printf(": back: %i\tforward: %i\n", back_direct, forward_direct);
	printf(": lenght: %i [m]\n\n", lenght);
}


void Junction::print() {

	printf(": SWITCH #%i ------------------\n", id);
	printf(": back: %i\tforward: %i\n", back_direct, forward_direct);
	if (forward_right != 0) printf(": switch: %i (forward left)\n\n", forward_right);
}


Rail::Rail (const string & descp) : Track (descp) {

	;

}

Junction::Junction (const string & descp) : Track (descp) {

	;

}
