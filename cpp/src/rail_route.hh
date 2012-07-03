#ifndef _RAIL_ROUTE_H_
#define _RAIL_ROUTE_H_

#include <vector>
#include <tuple>

class RailRoute
{

	private:

		struct RouteMark {
			RouteMark(double pos, double spl, double slope) : position(pos), speedlimit(spl), slope(slope) {}

			double position;
			double speedlimit;
			double slope;
		};

		std::vector<RouteMark> route;
		std::vector<RouteMark>::size_type actual_section;

	public:

		explicit RailRoute (const char *filename);

		double getSpeedLimit (double position);
		double getSlope (double position);
		
		bool isValidSection ();
		void resetSection () { actual_section = 0; }
		std::tuple <double, double, double> getSection(); /* Warning: This function changes the inner state of object like side effect! */

		void writeProfile ();		
		
};

#endif // _RAIL_ROUTE_H_
