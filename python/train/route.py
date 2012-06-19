#!/bin/env python3.2
# coding=utf-8


class RouteMark:
	'''
	Information on rail route
	'''

	def __init__ (self, distance, speedlimit, slope):

		# If speedlimit = 0, it means that the train must stop here. From this 
		# point begins a new train route.

		self.distance = distance		# distance from the beginning of the route [m]
		self.speedlimit = speedlimit	# maximum speed limit [km/h]
		self.slope = slope				# climb/descent route [ppt]


class RailRoute:
	'''
	Route of the passing train (speed limits, climb, descend)
	'''

		
	def __init__ (self, filename):
		
		self.route = []				# Train route (speed limits, climb, descend)
		try:
			with open(filename, 'r') as fh:
				for line in fh:
					data = line.partition(' ')
					distance = float(data[0])
					data = data[2].partition(' ')
					speedlimit = float(data[0])
					slope = float(data[2])
					self.route.append(RouteMark(distance, speedlimit, slope))
		except EnvironmentError as err:
			print (err)


	def getSpeedLimit (self, position):
		'''
		The speed limit at the specified position
		
		IN: position [m]
		OUT: speed [km/h]
		'''

		speed = 0.0
		for r in self.route:
			if (r.distance <= position):
				speed = r.speedlimit

		return speed
	

	def getSlope (self, position):
		'''
		The slope at the specified position
		
		IN: position [m]
		OUT: slope [ppt]
		'''

		slope = 0.0
		for r in self.route:
			if (r.distance <= position):
				slope = r.slope

		return slope

	
	
if __name__ == "__main__":

	import doctest
	doctest.testmod()

