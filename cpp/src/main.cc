#include "rail_route.hh"
#include "drive.hh"
#include <cstdio>
#include <cstdlib>
 

int main(int argc, char **argv) {

	RailRoute route("data/sample_A.route");
	route.writeProfile ();
	Drive vlak(route);

	vlak.run();
	vlak.writeResults();

	FILE* file;
	file = fopen ("results/gnuplot.dat", "w");	
	fprintf(file, "reset\n");
	fprintf(file, "set terminal \"png\"\n");
	fprintf(file, "set output \"results/sample_A.png\"\n");
	fprintf(file, "set title \"driving profile\"\n");
	fprintf(file, "set xlabel \"distance [km]\"\n");
	fprintf(file, "set yrange [0:200]\n");
	fprintf(file, "set ytics 0,20\n");
	fprintf(file, "set ylabel \"velocity [km/h]\"\n");
	fprintf(file, "set y2range [-20:180]\n");
	fprintf(file, "set y2tics 0,10,20\n");
	fprintf(file, "set y2label \"slope [ppt]\"\n");
	fprintf(file, "plot \"results/sample_A.profile\" u 1:2 w l title \"speedlimit\", \"results/sample_A.drive\" u 2:3 w l title \"actual speed\", \"results/sample_A.profile\" u 1:3 w l axis x1y2 title \"slope\"\n");
	fprintf(file, "reset\n");
	fclose (file);

	system ("gnuplot results/gnuplot.dat");

	return 0;
}
