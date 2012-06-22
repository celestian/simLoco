#include "rail_route.hh"

#include <iostream>
#include <fstream>
#include <string>


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

	cout << "Celkem vytvořeno " << route.size() << endl;



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

