#include "train.hh"
#include <cmath>
#include <tuple>
#include <cstdio>

using namespace std;


Train::Train(RailRoute &r): route(r) {

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

double Train::adhesion (double velocity) {

	return (7500.0/(fabs(velocity)+44.0) + 161.0)*0.001;
}

double Train::resistForce (double velocity) {

	return (pow(velocity,2.0)*rho_a + velocity*rho_b + rho_c)*9.81;
}

double Train::engineForce (double velocity) {

	double U = velocity;
	if (velocity < 20.0) U = 20.0;

	for (int i=5; i > 0; i--) {
		if ((velocity > 20.0) and (velocity > maxspeed/i))
			U = maxspeed/i;
	}

	return (3600 * power * adhesion(velocity))/U;
}

double Train::brakeForce (double velocity) {

	return (adhesion(velocity)/0.15) * wcount * 20000;
}

double Train::eqMotion (double velocity) {
		
	double engine, brake, resist;
	
	if (mode == 1) {
		engine = engineForce(velocity);
		brake = 0.0;
	}
	if (mode == -1) {
		engine = 0.0;
		brake = brakeForce(velocity);	
	}	
	resist  = resistForce(velocity);// + risistDragForce(route, position);
	
	return (engine - brake - resist) / (1000 * (2.0 * weight + rho_d));
}

double Train::deltaVelocity (double vA, double dt) {
		
	double rkA, rkB, rkC, rkD;
			
	rkA = eqMotion (vA);
	rkB = eqMotion (vA + (0.5 * rkA));
	rkC = eqMotion (vA + (0.5 * rkB));
	rkD = eqMotion (vA + rkC);

	return (rkA + 2.0*rkB + 2.0*rkC + rkD)*dt/6.0;
}

void Train::motion (double speedA, double speedB, double distance) {

	double dt = 0.25;
	double xA = 0.0;

	if (speedA == speedB)
		drive.push_back ( DriveMark(0, position + distance, speedA) );
	
	if(speedA < speedB) {
		double vA = speedA;
		double vB;
		double xB;
		while ((xA < distance) and (vA < speedB)) {
			vB = vA + 3.6 * deltaVelocity(vA, dt);//1, route, position + xA)	# [km/h]
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
		drive.push_back( DriveMark(0.0, position + distance, speedB) );

		while ((xA < distance) and (vA < speedA)) {
			vB = vA + 3.6 * deltaVelocity(vA, -dt);//, vA, -1, route, position + distance - xA)	# [km/h]
			xB = (dt*vB)/3.6 + xA;															//# [m]
			vA = vB;
			xA = xB	;
			drive.push_back(DriveMark(0, position + distance - xA, vB) );
		}
	}
}

double Train::deltaSpeedDistance (double speedA, double speedB) {
		
	double dt = 1;
	double vA = speedA;
	double xA = 0.0; 
	double xB = 0.0;
	double vB;

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
		vB = vA + 3.6 * deltaVelocity(vA, dt);//, vA, mode, route, position)	# [km/h]
		xB = (dt*vB)/3.6 + xA;//											# [m]
		vA = vB;
		xA = xB;
	}

	return xB;
}

tuple < double, double > Train::getOutlookA (double speedA, double speedB, double distance){

	double result_distance;
	
	double speedlimit = route.getSpeedLimit(position);

	if (deltaSpeedDistance(speedA, speedB) < distance) {
		double i = 0;
		double suff_distance = 0.0;
		double speedC = speedA + i;		


		while ((suff_distance < distance) and (speedC <= speedlimit)) {
			i += 0.2;
			suff_distance = 0.0;
			speedC = speedA + i;
			result_distance = deltaSpeedDistance(speedA, speedC);
			suff_distance += deltaSpeedDistance(speedC, speedB);
			suff_distance += result_distance;
		}
		
		return tuple < double, double > {(speedC-0.2),  result_distance};
	}

	return tuple < double, double > {speedA, 0.0};	
}

double Train::funct (double CC, double speedA, double speedB, double distance) {

	double currentA = deltaSpeedDistance(speedA, CC);
	double currentB = deltaSpeedDistance(CC, speedB);

	return distance - currentA - currentB;
}

tuple < double, double > Train::getOutlookB (double speedA, double speedB, double distance) {

	double C, fC;
	double speedlimit = route.getSpeedLimit(position);

	if (deltaSpeedDistance(speedA, speedB) < distance) {

		double A = speedA;
		double B = speedlimit;
		double fA = funct(A, speedA, speedB, distance);
		double fB = funct(B, speedA, speedB, distance);

		if (fA * fB < 0) {
			for (int i=0; i < 200; i++) {

				C = (A + B)/2;
				fC = funct(C, speedA, speedB, distance);
				fA = funct(A, speedA, speedB, distance);
				fB = funct(B, speedA, speedB, distance);

				if (fA * fC > 0.0) {
					A = C;
				}
				if (fB * fC > 0.0) {
					B = C;
				}
				if (fC == 0.0) {
					return tuple < double, double > {C, deltaSpeedDistance(speedA, C)};
				}
				if (fabs(A - B) < 0.2) {
					break;
				}
			}
			return tuple < double, double > {(C),  deltaSpeedDistance(speedA, C)};
		} else {
			return tuple < double, double > {speedA, distance - deltaSpeedDistance(speedA, speedB)};
		}
	}
	return tuple < double, double > {speedA, 0.0};
}

void Train::run () {

	position = 0.0;
	double speed = 0.0;
	list <DriveMark> passage;


//	list<RailRoute::RouteMark>::const_iterator iter = route.begin();
//	list<RailRoute::RouteMark>::const_iterator iter_end = route.end();


//		void getSection();

	int i = 1;
	route.resetSection();
	while (route.isValidSection()) {
	
		printf("%d\n", i++);
		route.nextSection();
	}
/*	
	--iter_end; iter != route.end(); ++iter) {
		double distance = (iter + 1)->distance - position;
		double speedA = iter->speedlimit;
		double speedB = (iter + 1)->speedlimit;

		if (speedA <= speedB) {
			passage += motion(speed, speedA, distance);
			position += distance;
			speed = passage.end()->speed;
		}
		else if (speedA > speedB) {
		
			if (distance == 500) {
				if (funct == 'A') {
					speedC, tmpdistance = getOutlookA(speed, speedB, distance);
				} else {
					speedC, tmpdistance = getOutlookB(speed, speedB, distance);
			} else {
				speedC, tmpdistance = getOutlookA(speed, speedB, distance);
			}
			passage += motion(speed,speedC, tmpdistance);
			position += tmpdistance;
			speed = passage[-1].speed;

			passage += motion(speed,speedB,distance-tmpdistance);
			position += distance - tmpdistance;
			speed = passage[-1].speed;
		}
	}

	iter = route.begin();
	iter_end = route.end();
	for (--iter_end; iter != route.end(); ++iter) {
		delta_v = (passage[i+1].speed - passage[i].speed) / 2.0 + passage[i].speed;
		delta_distance = passage[i+1].distance - passage[i].distance;
		passage[i+1].time = math.fabs(2 * delta_distance / delta_v) + passage[i].time;
	}
*/
}

//   get<2>(bar) = 100;    --------------------------------------------------------

