#include "rail_route.hh"

#include <list>
#include <tuple>
#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>


using namespace std;


RailRoute::RailRoute (const char* filename) {

	ifstream file_handler (filename);
	string line;
	string::size_type pos1, pos2;

	double position;
	double speedlimit;
	double slope;
	
	if (file_handler.is_open()) {
		while (! file_handler.eof() ) {
			getline (file_handler,line);
			
			if (line.size() > 0) {		
				pos1 = line.find(' ', 0);
				position = stod(line.substr(0,pos1));
				pos2 = line.find(' ', pos1+1);
				speedlimit = stod(line.substr(pos1,pos2-pos1));
				slope = stod(line.substr(pos2, line.size() - pos2));
				
				RouteMark mark (position, speedlimit, slope);
				route.push_back (mark);
			}
		}
		file_handler.close();
	}
	else cout << "Unable to open file";

	curent_route = route.begin();
}

double RailRoute::getSpeedLimit (double position) {

	double speed = 0.0;
	list<RouteMark>::const_iterator iter;
	
	for (iter = route.begin(); iter != route.end(); ++iter) {
		if (iter->position <= position) {
			speed = iter->speedlimit;
		}
	}

	return speed;
}

double RailRoute::getSlope (double position) {

	double slope = 0.0;
	list<RouteMark>::const_iterator iter;
	
	for (iter = route.begin(); iter != route.end(); ++iter) {
		if (iter->position <= position) {
			slope = iter->slope;
		}
	}

	return slope;
}

tuple <double, double, double> RailRoute::getSection() {

	double distance, speedA, speedB;
	std::list<RouteMark>::const_iterator next_iter = curent_route;
	++next_iter;
	
	distance = next_iter->position - curent_route->position;
	speedA = curent_route->speedlimit;
	speedB = next_iter->speedlimit;
	
	return tuple <double, double, double> {distance, speedA, speedB};
}

bool RailRoute::isValidSection () {
	
	std::list<RouteMark>::const_iterator next_iter = curent_route;
	++next_iter;
	
	if ( next_iter == route.end()) {
		return false;
	} else {
		return true;
	}
}

void RailRoute::writeProfile () {

	double x = 0.0;
	FILE * file;
		
	file = fopen("results/sample_A.profile", "w");

	while (getSpeedLimit(x) > 0.0) {
		fprintf(file, "%e\t%e\t%e\n", x/1000.0, getSpeedLimit(x), getSlope(x));
		x += 1.0;
	}
	fprintf(file, "%e\t%e\t%e\n", x/1000.0, getSpeedLimit(x), getSlope(x));
	fclose (file);
}
