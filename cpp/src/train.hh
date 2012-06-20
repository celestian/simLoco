#ifndef _RAIL_ROUTE_H_
#define _RAIL_ROUTE_H_


#include <list>


class RailRoute
{

	private:

		struct RouteMark {
			RouteMark(double pos, double spl, double slope) : position(pos), speedlimit(spl), slope(slope) {}

			const double position;
			const double speedlimit;
			const double slope;
		};

		std::list < RouteMark > route;


	public:

		explicit RailRoute (const char *filename);

		double getSpeedLimit (double position);
		double getSlope (double position);

};

#endif // _RAIL_ROUTE_H_
