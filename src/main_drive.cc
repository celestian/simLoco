#include "rail_route.hh"
#include "drive.hh"
#include <cstdio>
#include <cstdlib>
 
/*
#include <iostream>
#include <chrono>
#include <ctime>
*/

int main(int argc, char **argv) {

	RailRoute route("data/sample_A.route");
	route.writeProfile ();
	Drive vlak(route);

/*
	int i;
	std::chrono::time_point<std::chrono::system_clock> start, end;
    start = std::chrono::system_clock::now();
	for (i=0; i<100; i++) vlak.run();
    end = std::chrono::system_clock::now();
    int elapsed_seconds = std::chrono::duration_cast<std::chrono::milliseconds>
                             (end-start).count();
    std::time_t end_time = std::chrono::system_clock::to_time_t(end);
    std::cout << "100 times runA: elapsed time: " << elapsed_seconds << " ms\n";

	vlak.writeResults();

    start = std::chrono::system_clock::now();
	for (i=0; i<100; i++) vlak.runB();
    end = std::chrono::system_clock::now();
    elapsed_seconds = std::chrono::duration_cast<std::chrono::milliseconds>
                             (end-start).count();
    end_time = std::chrono::system_clock::to_time_t(end);
    std::cout << "100 times runB: elapsed time: " << elapsed_seconds << " ms\n";
*/

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
