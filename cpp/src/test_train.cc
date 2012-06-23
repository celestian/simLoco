#include "rail_route.hh"
#include "train.hh"


int main(int argc, char **argv) {
	
	const char *file = "data/sample_A.route";
	
	RailRoute Route(file);
	Train vlak(Route);
	vlak.run();

	return 0;
}
