#ifndef _DRIVE_H_
#define _DRIVE_H_

#include <cmath>
#include <vector>
#include <tuple>
#include "rail_route.hh"


class Drive
{

	public:

		explicit Drive(RailRoute &r);
		void run ();
		void writeResults ();


	private:

		struct DriveMark {
			DriveMark(double t, double d, double s) : time(t), distance(d), speed(s) {}

			double time;				// [s]
			double distance;			// [m]
			double speed;				// [km/h]
		};

		RailRoute &route;
		std::vector <DriveMark> drive;

		double maxspeed;				// [km/h]
		double power;					// [kW]
		double edb;						// [kW]
		double weight;					// [t] 
		double lenght;					// [m]
		double wcount;					// number of wagons

		// ToDo: To determine the parameters directly from the specified Drive
		// (need database)

		//	vehicle		weight [t]	d		a		b		c
		//	----------------------------------------------------
		//	CD 471		62.7		0,15	
		//	CD 071		45.5		0,06	0,00033	0,0008	1,35	
		//	CD 971		47.3		0,06	0,00033	0,0008	1,35

		double rho_a;
		double rho_b;
		double rho_c;
		double rho_d;
	
		inline double adhesion (double velocity) { return (7500.0/(fabs(velocity)+44.0) + 161.0)*0.001; }
		inline double resistForce (double velocity) { return (pow(velocity,2.0)*rho_a + velocity*rho_b + rho_c)*9.81; }
		inline double resistSlopeForce (double position) { return weight * 9.81 * route.getSlope(position); }
		double engineForce (double velocity);
		inline double brakeForce (double velocity) { return (adhesion(velocity)/0.15) * wcount * 20000; }
		
		double eqMotion (double velocity, double mode, double position);
		double 	deltaVelocity (double vA, double dt, double mode, double position);
		void motion (double speedA, double speedB, double distance, double position);
		
		double deltaSpeedDistance (double speedA, double speedB, double position);
		std::tuple <double, double> getOutlookA (double speedA, double speedB, double distance, double position);
		double funct (double CC, double speedA, double speedB, double distance, double position);
		std::tuple <double, double> getOutlookB (double speedA, double speedB, double distance, double position);

};

#endif // _DRIVE_H_
