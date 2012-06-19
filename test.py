#!/bin/env python3.2
# coding=utf-8
#


import os
from time import time
import train.route
import train.drive
import math

def main():

	drive_unit = train.drive.DriveUnit()

	j = 0
	for distance in range (500, 8000,50):
		for i in range(30,130,5):

			j += 1
			
			# --- data/test.route ---
			try:
				with open('data/test.route', 'w') as fh:
					fh.write('{:g} {:g} {:g}\n'.format(0, 30, 0.0))
					fh.write('{:g} {:g} {:g}\n'.format(500, 30 + i, 0.0))
					fh.write('{:g} {:g} {:g}\n'.format(distance + 500, 15, 0.0))
					fh.write('{:g} {:g} {:g}\n'.format(distance + 1000, 0, 0.0))
			except EnvironmentError as err:
				print (err)
		
			route = train.route.RailRoute('data/test.route')
		
			t0 = time()	
			result = drive_unit.run(route, 'A')
			t1 = time() - t0
			result = drive_unit.run(route, 'B')
			t2 = time() - t1

			print ("{:g}\t{:g}\t{:g}\t{:g}\t{:g}".format(j, t1, t2, distance, i))


if __name__ == "__main__":

	main()

