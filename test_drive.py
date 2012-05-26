#!/bin/env python3.2
# coding=utf-8


import train.route
import train.drive


def main():


	route = train.route.RailRoute('path.dat')
	drive_unit = train.drive.DriveUnit()
	drive_unit.run(route)

#	./test_drive.py > drive.dat
#
#	gnuplot> reset
#	gnuplot> set terminal "png"
#	gnuplot> set output "driving.png"
#	gnuplot> set title "velocity (distance) with speedlimit and slope"
#	gnuplot> plot "data.dat" u 1:2 w l, "data.dat" u 1:3 w l, "drive.dat" u 2:3 w l
#	gnuplot> reset


#	Old Test
#-------------------------------------------------------------------------------
#	# Static test train drive properties	
#		
#	print("# v[km/h]\tadhesion[-]\tdrag[N]\tengine_power[N]\tconst_throttle[-]")
#	for velocity in range(0,161):
#		adh = drive_unit.adhesion(velocity)
#		drg = drive_unit.dragForce(velocity)
#		eng = drive_unit.engineForce(velocity)
#		uef = drive_unit.uniformEngineForce (velocity)
#		print ("{:e}\t{:e}\t{:e}\t{:e}\t{:e}".format(velocity, adh, drg, eng, uef))	

#	# gnuplot> set title "drag/engine (velocity)"
#	# gnuplot> plot "data.dat" u 1:($3/$4)


if __name__ == "__main__" :

	main()

