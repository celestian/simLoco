#!/bin/env python3.2
# coding=utf-8


import train.route


def main():

	route = train.route.RailRoute('path.dat')

	for s in range(0,6000):
		print ('{:g}\t{:g}\t{:g}'.format(s, route.getSpeedLimit(s), route.getSlope(s) ))

#	./test_route.py > data.dat
#
#	gnuplot> reset
#	gnuplot> set terminal "png"
#	gnuplot> set output "profile.png"
#	gnuplot> set title "speed_limit (position), slope (position)"
#	gnuplot> plot "data.dat" u 1:2 w l, "data.dat" u 1:3 w l
#	gnuplot> reset


if __name__ == "__main__" :

	main()

