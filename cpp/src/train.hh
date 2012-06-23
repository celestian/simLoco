#ifndef _TRAIN_H_
#define _TRAIN_H_

#include <list>
#include <tuple>
#include "rail_route.hh"

class Train
{

	private:

		struct DriveMark {
			DriveMark(double t, double d, double s) : time(t), distance(d), speed(s) {}

			const double time;			// [s]
			const double distance;		// [m]
			const double speed;		// [km/h]
		};

		RailRoute &route;
		std::list < DriveMark > drive;


	
		double maxspeed;				// [km/h]
		double power;					// [kW]
		double edb;						// [kW]
		double weight;					// [t] 
		double lenght;					// [m]
		double wcount;					// number of wagons

		// ToDo: To determine the parameters directly from the specified train
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

		int mode;
		double position;
	
		double adhesion (double velocity);
		double resistForce (double velocity);
		double engineForce (double velocity);
		double brakeForce (double velocity);
		
		double eqMotion (double velocity);
		double 	deltaVelocity (double vA, double dt);
		void motion (double speedA, double speedB, double distance);
		
		double deltaSpeedDistance (double speedA, double speedB);
		std::tuple < double, double > getOutlookA (double speedA, double speedB, double distance);
		double funct (double CC, double speedA, double speedB, double distance);
		std::tuple < double, double > getOutlookB (double speedA, double speedB, double distance);

	public:

		Train(RailRoute &r);
		void run ();
};

#endif // _TRAIN_H_
