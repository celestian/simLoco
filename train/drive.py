#!/bin/env python3.2
# coding=utf-8


import math
import train.route as Route


class DriveMark:
	'''
	Information on the driving
	'''

	def __init__ (self, time, distance, speed, acceleration):

		self.time = time					# [s]
		self.distance = distance			# [m]
		self.speed = speed					# [km/h]
		self.acceleration = acceleration	# [m/s^2]


class DriveUnit():
	'''
	Elementary physics needed to ride the train
	'''

	def __init__ (self):

		self.maxspeed = 140		# [km/h]
		self.power = 2000		# [kW]
		self.edb = 1700			# [kW]
		self.weight = 155.5		# [t] 
		self.lenght = 79.2		# [m]
		self.wcount = 3			# number of wagons

		# ToDo: To determine the parameters directly from the specified train
		# (need database)

		#	vehicle		weight [t]	d		a		b		c
		#	----------------------------------------------------
		#	CD 471		62.7		0,15	
		#	CD 071		45.5		0,06	0,00033	0,0008	1,35	
		#	CD 971		47.3		0,06	0,00033	0,0008	1,35

		self.rho_a = 0.030624
		self.rho_b = 0.07424
		self.rho_c = 1.35
		self.rho_d = 14.973
		
		
#-------------------------------------------------------------------------------
#	F O R C E S

	def adhesion (self, velocity):
		'''
		Estimate of adhesion 
		http://homen.vsb.cz/~s1i95/mvd/mvd_tah.htm
	
		IN: velocity [km/h]
		OUT: adhesion coefficient [-]
		'''
		# Kother
#		return (9000.0/(math.fabs(velocity)+42.0) + 116.0)*0.001

		# Curtius-Kniffler
		return (7500.0/(math.fabs(velocity)+44.0) + 161.0)*0.001

		
	def dragForce (self, velocity):
		'''
		Drag of vehicles during moving
		
		IN: velocity [km/h]
		OUT: drag force [N]
		'''

		return (math.pow(velocity,2.0)*self.rho_a + velocity*self.rho_b + self.rho_c)*9.81


	def slopeDragForce (self, position):
		'''
		Drag force arising from the uphill / downhill
		
		IN: position m]
		OUT: slope drag force [N]
		'''
		if (position == -1):
			return 0.0
		else:
			return self.weight * 9.81 * self.route.getSlope(position)
	
		
	def engineForce (self, velocity):
		'''
		Engine force
		
		IN: velocity [km/h]
		OUT: engine force [N]
		'''
		
		# Have you ever tried to divide by zero? (Well the case of velocity = 0
		# would be a different formula, but at the moment it is unknown.)
		U = velocity
		if (velocity < 20.0): U = 20.0

		return (3600 * self.power * self.adhesion(velocity))/U


	def uniformEngineForce (self, velocity):
		'''
		Throttle value for steady movement (acceleration = 0 m/s^2)
		
		IN: velocity [km/h]
		OUT: throttle_coefficient [-]
		'''
		
		# ToDo: So far only calculated for flat terrain! (slope = 0 ppt)

		return self.dragForce(velocity)/self.engineForce(velocity)


	def brakeForce (self, velocity):
	
		#aa = self.adhesion(velocity) * 9810 * self.weight
		bb = (self.adhesion(velocity) * self.wcount * 200000)
		return bb


#-------------------------------------------------------------------------------
# B R A K E   U N I T

	def brakingDistance (self, speedA, speedB):
		'''
		Returns the distance needed to slow the speed of speedA to speedB
		
		IN: speedA [km/h], speedB [km/h]
		OUT: distance [m]
		'''
		
		dt = 0.25
		vA = speedA
		xA = 0.0
		xB = 0.0
		while (vA > speedB):
			vB = vA + 3.6*self.deltaVelocity(dt, vA, -1, -1)	# [km/h]
			aB = (vB-vA)/(3.6*dt)										# [m/s^2]
			xB = (dt*vB)/3.6 + xA										# [m]
			vA = vB
			xA = xB	
		return xB

		
#-------------------------------------------------------------------------------
#	M O T I O N

	def eqMotion (self, velocity, mode, distance):
		'''
		Differential equation of motion

		IN: velocity [km/h], distance [m]
		OUT: a = dv/dt [m/s^2]
		
		Note: distance is only parameter for slope
		'''
		
		# ToDo: NEW VARIABLE MODE
		
		engine = lambda mode, velocity: self.engineForce(velocity) if mode == 1 else 0.0
		brake = lambda mode, velocity: self.brakeForce(velocity) if mode == -1 else 0.0
		drag  = self.dragForce(velocity) + self.slopeDragForce(distance)
		
		return (engine(mode, velocity) - brake(mode, velocity) - drag)/(1000*(2.0*self.weight+self.rho_d))


	def deltaVelocity (self, dt, vA, mode, distance):
		'''
		4th-order Runge–Kutta method
		http://en.wikipedia.org/wiki/Runge%E2%80%93Kutta_methods
		
		in:	dt ... time step [s]
			vA ... actual velocity [km/h]
			distance ... only parameter for slope [m]

		out: delta v [m/s]
		'''
		
		rkA = self.eqMotion (vA, mode, distance)
		rkB = self.eqMotion (vA + (0.5 * rkA), mode, distance)
		rkC = self.eqMotion (vA + (0.5 * rkB), mode, distance)
		rkD = self.eqMotion (vA + rkC, mode, distance)

		return (rkA + 2.0*rkB + 2.0*rkC + rkD)*dt/6.0


	def motion (self, position, traveltime, distance, startspeed, endspeed, tmpspeed):
		'''
		Train passing the section of route
		
		IN: position 	... from the beginning of movement	[m]
			traveltime	... from the beginning of movement	[s]
	
			distance 	... distance to be traveled			[m]
			startspeed	... initial speed limit				[km/h]
			endspeed 	... speed limit at the end			[km/h]
			
			tmpspeed	... actual initial velocity			[km/h]
		
		OUT: list of DriveMark (time, distance, speed, acceleration)		
		'''
		
		# Todo: REQUIRED A THOROUGH ANALYSIS, I'M SURE THERE ARE ERRORS
		# For example: What exactly is the role of tmpspeed and startspeed?
		
		vA = tmpspeed
		aA = 0.0
		dt = 0.25
		xA = 0.0
		time = 0.0
		startspeed = tmpspeed
		
		result = []
		
		if (startspeed == endspeed):
			# ToDo: We have enough power?
			aB = 0.0					# [m/s^2]
			vB = startspeed				# [km/h]
			xB = distance				# [m]
			if (vA > 0.0):
				time = distance/(vA/3.6)	# [s]				
			else:
				time = 0.0
			result.append( DriveMark(traveltime + time, position + xB, vB, aB) )

			return result
		
		if (startspeed < endspeed):
			while (xA < distance) and (vA < endspeed):
				vB = vA + 3.6*self.deltaVelocity(dt, vA, 1, position + xA)	# [km/h]
				aB = (vB-vA)/(3.6*dt)									# [m/s^2]
				xB = (dt*vB)/3.6 + xA									# [m]
				time = time + dt										# [s]
				vA = vB
				xA = xB	
				result.append( DriveMark(traveltime + time, position + xB, vB, aB) )

			if (xA < distance):
				aB = 0.0						# [m/s^2]
				xB = distance					# [m]
				vB = endspeed					# [km/h]
				time = distance/(endspeed/3.6)	# [s]				
				result.append( DriveMark(traveltime + time, position + xB, vB, aB) )

			return result				

		if (startspeed > endspeed):			
			if (endspeed != 0.0):
				while (xA < distance) and (vA > endspeed):
					vB = vA + 3.6*self.deltaVelocity(dt, vA, -1, position + xA)	# [km/h]
					aB = (vB-vA)/(3.6*dt)										# [m/s^2]
					xB = (dt*vB)/3.6 + xA										# [m]
					time = time + dt											# [s]
					vA = vB
					xA = xB	
					result.append( DriveMark(traveltime + time, position + xB, vB, aB) )

				if (xA < distance):
					aB = 0.0						# [m/s^2]
					xB = distance					# [m]
					vB = endspeed					# [km/h]
					time = distance/(endspeed/3.6)	# [s]				
					result.append( DriveMark(traveltime + time, position + xB, vB, aB) )

				return result
			
			# We need to stop at the end of section!
			else: 
				tmp = []
				tmp.append( DriveMark(0.0, position + distance, 0.0, 0.0) )
				vA = 0
				xA = 0
				time = 0
				while (vA < tmpspeed):
					vB = vA + 3.6*self.deltaVelocity(-dt, vA, -1, position + distance - xA)	# [km/h]
					aB = -(vB-vA)/(3.6*dt)										# [m/s^2]
					xB = (dt*vB)/3.6 + xA										# [m]
					time = time + dt													# [s]
					vA = vB
					xA = xB	
					tmp.append( DriveMark(time, position + distance - xA, vB, aB) )

				if (xB < distance):
					aB = 0.0						# [m/s^2]
					xB = xB							# [m]
					vB = tmpspeed					# [km/h]
					time = xB/(vB/3.6)	# [s]				
					result.append( DriveMark(traveltime + time, position + xB, vB, aB) )
					
					
				for r in reversed (tmp):
					result.append( DriveMark(traveltime + time - r.time, r.distance, r.speed, r.acceleration) )
			
				return result


#-------------------------------------------------------------------------------		
# T R A I N   D R I V E R

	def getSection (self):
		'''
		Returns the railway line suitable for passing		
		'''
		# Todo: REQUIRED A THOROUGH ANALYSIS
		# If someone knows a better solution, like I learn something new
				
		tmp = []
		tmp.append( Route.RouteMark(0.0, self.route.route[0].speedlimit, 0) )

		for i in range(len(self.route.route)-1):
			position = self.route.route[i+1].distance
			speedA = self.route.route[i].speedlimit
			speedB = self.route.route[i+1].speedlimit
		
			if (speedA > speedB):
				if (speedB == 0.0):
					x = position - self.brakingDistance(speedA, 0.0)
					position = position - self.brakingDistance(self.route.getSpeedLimit(x), 0.0)
				else:
					x = position - self.brakingDistance(speedA, speedB)
					position = position - self.brakingDistance(self.route.getSpeedLimit(x), speedB)

			tmp.append(Route.RouteMark(position, speedA, 0) )			
			tmp.append(Route.RouteMark(position, speedB, 0) )
		
		# very ugly solution
		tmp.append(tmp[-1])
		tmp.append(tmp[-1])	

		result = []
		for i in range(-2,len(tmp)-2):
			if (tmp[i].distance <= tmp[i+1].distance) and (tmp[i+1].distance <= tmp[i+2].distance):
				result.append(tmp[i])
		del tmp
					
		return result


	def run (self, route):
		'''
		Passing route
		
		'''	
		# ToDo: Clarify interface for the rest of the simulation
		# Todo: REQUIRED A THOROUGH ANALYSIS
		# If someone knows a better solution, like I learn something new
		
		self.route = route
		sector = self.getSection()
		position = 0.0
		traveltime = 0.0
		result = []
		for i in range(len(sector)-1):
			distance = sector[i+1].distance - sector[i].distance
			startspeed = sector[i].speedlimit
			endspeed = sector[i+1].speedlimit
			if (i == 0):
				tmpspeed = 0
				tmptime = 0
			else:
				tmpspeed = result[-1].speed
				tmptime = result[-1].time
			traveltime = traveltime + tmptime
			result += self.motion(position, traveltime, distance, startspeed, endspeed, tmpspeed)
			position = position + distance

		return result


if __name__ == "__main__":

	import doctest
	doctest.testmod()
