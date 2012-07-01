#include "drive.hh"
#include <cmath>
#include <vector>
#include <tuple>
#include <cstdio>


using namespace std;


Drive::Drive(RailRoute &r): route(r) {

	maxspeed = 140;
	power = 2000;
	edb = 1700;
	weight = 155.5;
	lenght = 79.2;
	wcount = 3;
	rho_a = 0.030624;
	rho_b = 0.07424;
	rho_c = 1.35;
	rho_d = 14.973;
	
}

double Drive::engineForce (double velocity) {

	double U = velocity;
	if (velocity < 20.0) U = 20.0;

	for (int i=5; i > 0; i--) {
		if ((velocity > 20.0) and (velocity > maxspeed/i))
			U = maxspeed/i;
	}

	return (3600 * power * adhesion(velocity))/U;
}

double Drive::eqMotion (double velocity, double mode, double position) {
		
	double engine, brake, resist;
	
	if (mode == 1) {
		engine = engineForce(velocity);
		brake = 0.0;
	}
	if (mode == -1) {
		engine = 0.0;
		brake = brakeForce(velocity);	
	}	
	resist  = resistForce(velocity)+ resistSlopeForce(position);
	
	return (engine - brake - resist) / (1000 * (2.0 * weight + rho_d));
}

double Drive::deltaVelocity (double vA, double dt, double mode, double position) {
		
	double rkA, rkB, rkC, rkD;
			
	rkA = eqMotion (vA, mode, position);
	rkB = eqMotion (vA + (0.5 * rkA), mode, position);
	rkC = eqMotion (vA + (0.5 * rkB), mode, position);
	rkD = eqMotion (vA + rkC, mode, position);

	return (rkA + 2.0*rkB + 2.0*rkC + rkD)*dt/6.0;
}

void Drive::motion (double speedA, double speedB, double distance, double position) {

	double dt = 0.25;
	double xA = 0.0;

	if (speedA == speedB)
		drive.push_back ( DriveMark(0, position + distance, speedA) );
	
	if(speedA < speedB) {
		double vA = speedA;
		double vB;
		double xB;
		while ((xA < distance) and (vA < speedB)) {
			vB = vA + 3.6 * deltaVelocity(vA, dt, 1, position + xA);//1, route, )	# [km/h]
			xB = (dt*vB)/3.6 + xA;												//# [m]
			vA = vB;
			xA = xB	;
			drive.push_back( DriveMark(0, position + xB, vB) );
		}
	}
	
	if (speedA > speedB) {	
		double vA = speedB;
		double vB;
		double xB;
		vector<DriveMark>::iterator curr_pos = drive.end();
		drive.push_back( DriveMark(0.0, position + distance, speedB) );
		
		while ((xA < distance) and (vA < speedA)) {
			vB = vA + 3.6 * deltaVelocity(vA, -dt, -1, position + distance - xA);//, vA, -1, route, )	# [km/h]
			xB = (dt*vB)/3.6 + xA;															//# [m]
			vA = vB;
			xA = xB	;
			
			curr_pos = drive.insert (curr_pos, DriveMark(0, position + distance - xA, vB));
		}
	}
}

double Drive::deltaSpeedDistance (double speedA, double speedB, double position) {
		
	double dt = 1;
	double vA = speedA;
	double xA = 0.0; 
	double xB = 0.0;
	double vB;
	double mode;

	if (speedA > speedB) {
		mode = -1;
	}
	else if (speedA < speedB) {
		mode = 1;
	}
	else {
		return 0;	
	}

	while ( (mode * (vA - speedB)) < 0.0 ) {
		vB = vA + 3.6 * deltaVelocity(vA, dt, mode, position);//, vA, mode, route)	# [km/h]
		xB = (dt*vB)/3.6 + xA;//											# [m]
		vA = vB;
		xA = xB;
	}

	return xB;
}

tuple <double, double> Drive::getOutlookA (double speedA, double speedB, double distance, double position){

	double result_distance;
	double pos = position;
	double speedlimit = route.getSpeedLimit(pos);

	if (deltaSpeedDistance(speedA, speedB, pos) < distance) {
		double i = 0;
		double suff_distance = 0.0;
		double speedC = speedA + i;		


		while ((suff_distance < distance) and (speedC <= speedlimit)) {
			i += 0.2;
			suff_distance = 0.0;
			speedC = speedA + i;
			result_distance = deltaSpeedDistance(speedA, speedC, pos);
			suff_distance += deltaSpeedDistance(speedC, speedB, pos);
			suff_distance += result_distance;
		}
		
		return tuple <double, double> {(speedC-0.2),  result_distance};
	}

	return tuple <double, double> {speedA, 0.0};	
}

double Drive::funct (double CC, double speedA, double speedB, double distance, double position) {

	double currentA = deltaSpeedDistance(speedA, CC, position);
	double currentB = deltaSpeedDistance(CC, speedB, position);

	return distance - currentA - currentB;
}

tuple <double, double> Drive::getOutlookB (double speedA, double speedB, double distance, double position) {

	double C, fC;
	double pos = distance/2.0 + position;
	double speedlimit = route.getSpeedLimit(pos);

	if (deltaSpeedDistance(speedA, speedB, pos) < distance) {

		double A = speedA;
		double B = speedlimit;
		double fA = funct(A, speedA, speedB, distance, pos);
		double fB = funct(B, speedA, speedB, distance, pos);

		if (fA * fB < 0) {
			for (int i=0; i < 200; i++) {

				C = (A + B)/2;
				fC = funct(C, speedA, speedB, distance, pos);
				fA = funct(A, speedA, speedB, distance, pos);
				fB = funct(B, speedA, speedB, distance, pos);

				if (fA * fC > 0.0) {
					A = C;
				}
				if (fB * fC > 0.0) {
					B = C;
				}
				if (fC == 0.0) {
					return tuple <double, double> {C, deltaSpeedDistance(speedA, C, pos)};
				}
				if (fabs(A - B) < 0.2) {
					break;
				}
			}
			return tuple <double, double> {(C),  deltaSpeedDistance(speedA, C, pos)};
		} else {
			return tuple <double, double> {speedA, distance - deltaSpeedDistance(speedA, speedB, pos)};
		}
	}
	return tuple <double, double> {speedA, 0.0};
}

void Drive::run () {

	double position = 0.0;
	double speed = 0.0;
	tuple <double, double, double> section;
	tuple <double, double> look;

	double speedC, tmpdistance, delta_v, delta_distance;

	route.resetSection();
	while (route.isValidSection()) {
	
		section = route.getSection();
		
		double distance = get<0>(section); 
		double speedA = get<1>(section); 
		double speedB = get<2>(section); 

		if (speedA <= speedB) {
			motion(speed, speedA, distance, position);
			position += distance;
			speed = drive.back().speed;
		}
		else if (speedA > speedB) {
			look = getOutlookA(speed, speedB, distance, position);
			speedC = get<0>(look); 
			tmpdistance = get<1>(look); 

			motion(speed,speedC, tmpdistance, position);
			position += tmpdistance;
			speed = drive.back().speed;

			motion(speed,speedB,distance-tmpdistance, position);
			position += distance - tmpdistance;
			speed = drive.back().speed;
		}
		route.nextSection();
	}

	vector<DriveMark>::size_type i;
	for (i=0; i < (drive.size() - 1); ++i) {
		delta_v = (drive[i+1].speed - drive[i].speed) / 2.0 + drive[i].speed;
		delta_distance = drive[i+1].distance - drive[i].distance;
		drive[i+1].time = fabs(2 * delta_distance / delta_v) + drive[i].time;
	}

}

void Drive::writeResults () {

	FILE * file;
	vector<DriveMark>::size_type i;
		
	file = fopen("results/sample_A.drive", "w");
	for (i=0; i < drive.size(); ++i) {
		fprintf (file, "%e\t%e\t%e\n", drive[i].time, drive[i].distance/1000.0, drive[i]. speed);	
	}
	fclose (file);
}
