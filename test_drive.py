#!/bin/env python3.2
# coding=utf-8
#


import os
import train.route
import train.drive


def main():

	route = train.route.RailRoute('data/sample_A.route')
	drive_unit = train.drive.DriveUnit()
	result = drive_unit.run(route)

	try:
		with open('results/sample_A.drive', 'w') as fh:
			for d in result:
				fh.write("{:e}\t{:e}\t{:e}\t{:e}\n".format(d.time, d.distance, d.speed, d.acceleration))
	except EnvironmentError as err:
		print (err)

	try:
		with open('results/gnuplot.dat', 'w') as fh:
			fh.write('reset\n')
			fh.write('set terminal "png"\n')
			fh.write('set output "results/sample_A.png"\n')
			fh.write('set title "driving profile"\n')
			fh.write('set xlabel "distance [m]"\n')
			fh.write('set ytics textcolor rgb "green"\n')
			fh.write('set ylabel textcolor rgb "green"\n')
			fh.write('set yrange [0:160]\n')
			fh.write('set ylabel "velocity [km/h]"\n')
			fh.write('set y2tics textcolor rgb "blue"\n')
			fh.write('set y2label textcolor rgb "blue"\n')
			fh.write('set y2range [-20:140]\n')
			fh.write('set y2label "slope [ppt]"\n')
			fh.write('plot "data/sample_A.profil" u 1:2 w l title "speedlimit", "results/sample_A.drive" u 2:3 w l title "actual speed", "data/sample_A.profil" u 1:3 w l axis x1y2 title "slope"\n')
			fh.write('reset\n')
	except EnvironmentError as err:
		print (err)

	os.system('gnuplot results/gnuplot.dat')



if __name__ == "__main__":

	main()

