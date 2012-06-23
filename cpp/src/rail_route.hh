#ifndef _RAIL_ROUTE_H_
#define _RAIL_ROUTE_H_

#include <list>
#include <tuple>

class RailRoute
{

	private:

		struct RouteMark {
			RouteMark(double pos, double spl, double slope) : position(pos), speedlimit(spl), slope(slope) {}

			const double position;
			const double speedlimit;
			const double slope;
		};

		std::list<RouteMark> route;
		std::list<RouteMark>::const_iterator curent_route;

	public:

		explicit RailRoute (const char *filename);

		double getSpeedLimit (double position);
		double getSlope (double position);
		
		inline void resetSection () { curent_route = route.begin(); }
		std::tuple <double, double, double> getSection();
		inline void nextSection () { ++curent_route; }
		bool isValidSection ();		
};

#endif // _RAIL_ROUTE_H_
