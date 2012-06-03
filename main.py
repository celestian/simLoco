#!/bin/env python3.2
# coding=utf-8
#


import os
import train.route
import train.drive


def main():

	route = train.route.RailRoute('data/sample_A.route')
	drive_unit = train.drive.DriveUnit()


	# There is the simulation core
	result = drive_unit.run(route)
	print ('Travel time is {:g} minute.'.format(result[-1].time/60))


	# We will make some visualization (gnuplot is needed)
	
	# --- results/sample_A.profile ---
	try:
		with open('results/sample_A.profile', 'w') as fh:
			x = 0.0
			while (route.getSpeedLimit(x) > 0.0):
				fh.write('{:e}\t{:e}\t{:e}\n'.format(x, route.getSpeedLimit(x), route.getSlope(x) ))
				x += 1.0
			fh.write('{:e}\t{:e}\t{:e}\n'.format(x, route.getSpeedLimit(x), route.getSlope(x) ))
	except EnvironmentError as err:
		print (err)

	# --- results/sample_A.drive ---
	try:
		with open('results/sample_A.drive', 'w') as fh:
			for d in result:
				fh.write("{:e}\t{:e}\t{:e}\n".format(d.time, d.distance, d.speed))
	except EnvironmentError as err:
		print (err)
	

	# --- results/gnuplot.dat ---
	try:
		with open('results/gnuplot.dat', 'w') as fh:
			fh.write('reset\n')
			fh.write('set terminal "png"\n')
			fh.write('set output "results/sample_A.png"\n')
			fh.write('set title "driving profile"\n')
			fh.write('set xlabel "distance [m]"\n')
			fh.write('set yrange [0:160]\n')
			fh.write('set ylabel "velocity [km/h]"\n')
			fh.write('set y2range [-20:140]\n')
			fh.write('set y2tics 0,10,20\n')
			fh.write('set y2label "slope [ppt]"\n')
			fh.write('plot "results/sample_A.profile" u 1:2 w l title "speedlimit", "results/sample_A.drive" u 2:3 w l title "actual speed", "results/sample_A.profile" u 1:3 w l axis x1y2 title "slope"\n')
			fh.write('reset\n')
	except EnvironmentError as err:
		print (err)

	# !!! PLEASE, comment out this line if you do not gnuplot
	os.system('gnuplot results/gnuplot.dat')



if __name__ == "__main__":

	main()

