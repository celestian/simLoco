#include "rail_route.hh"

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
				route.emplace_back(RouteMark(position, speedlimit, slope));
			}
		}
		file_handler.close();
	}
	else cout << "Unable to open file";
}

double RailRoute::getSpeedLimit (double position) {

	double speed = 0.0;
	vector<RouteMark>::size_type i;
	
	for (i=0; i < route.size(); ++i) {
		if (route[i].position <= position) {
			speed = route[i].speedlimit;
		}
	}

	return speed;
}

double RailRoute::getSlope (double position) {

	double slope = 0.0;
	vector<RouteMark>::size_type i;
	
	for (i=0; i < route.size(); ++i) {
		if (route[i].position <= position) {
			slope = route[i].slope;
		}
	}

	return slope;
}

bool RailRoute::isValidSection () {
	
	if ( actual_section + 1 < route.size()) {
		return true;
	} else {
		return false;
	}
}

tuple <double, double, double> RailRoute::getSection() {

	double distance, speedA, speedB;
	vector<RouteMark>::size_type i;
	
	for (i = actual_section + 1; i < route.size(); ++i) {
		distance = route[i].position - route[actual_section].position;
		speedA = route[actual_section].speedlimit;
		speedB = route[i].speedlimit;
		if (speedA != speedB) {
			actual_section = i;
			break;
		}
	}
	
	return tuple <double, double, double> {distance, speedA, speedB};
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
