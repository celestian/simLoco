#include "rail_route.hh"
#include "drive.hh"


int main(int argc, char **argv) {
	
	const char *file = "data/sample_A.route";
	
	RailRoute Route(file);
	Drive vlak(Route);
	vlak.run();

	return 0;
}
