#!/bin/env python3.2
# coding=utf-8


import math
import train.route


class DriveMark:
	'''
	Information on the driving
	'''

	def __init__ (self, time, distance, speed):

		self.time = time					# [s]
		self.distance = distance			# [m]
		self.speed = speed					# [km/h]


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
		# return (9000.0/(math.fabs(velocity)+42.0) + 116.0)*0.001

		# Curtius-Kniffler
		return (7500.0/(math.fabs(velocity)+44.0) + 161.0)*0.001

		
	def dragForce (self, velocity):
		'''
		Drag of vehicles during moving
		
		IN: velocity [km/h]
		OUT: drag force [N]
		'''

		return (math.pow(velocity,2.0)*self.rho_a + velocity*self.rho_b + self.rho_c)*9.81


	def slopeDragForce (self, route, position):
		'''
		Drag force arising from the uphill / downhill
		
		IN: route, position [m]
		OUT: slope drag force [N]
		route ... description of the rail route
		'''
		#ToDo: implement resistive forces caused by cornering or a tunnel

		return self.weight * 9.81 * route.getSlope(position)
	
		
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
		bb = (self.adhesion(velocity)/0.15) * self.wcount * 20000 * 0.75
		return bb


#-------------------------------------------------------------------------------
#	M O T I O N

	def eqMotion (self, velocity, mode, route, position):
		'''
		Differential equation of motion

		IN: velocity [km/h], mode [-], route, position [m]
		OUT: a = dv/dt [m/s^2]
		Note: mode ... accelerator / brake lever
			route ... description of the rail route
			position ... it is needed to determine the climb / descent
		'''
		
		engine = lambda mode, velocity: self.engineForce(velocity) if mode == 1 else 0.0
		brake = lambda mode, velocity: self.brakeForce(velocity) if mode == -1 else 0.0
		drag  = self.dragForce(velocity) + self.slopeDragForce(route, position)
		
		return (engine(mode, velocity) - brake(mode, velocity) - drag)/(1000*(2.0*self.weight+self.rho_d))


	def deltaVelocity (self, dt, vA, mode, route, position):
		'''
		4th-order Runge–Kutta method
		http://en.wikipedia.org/wiki/Runge%E2%80%93Kutta_methods
		
		IN:	dt ... time step [s]
			vA ... actual velocity [km/h]
			mode ... accelerator / brake lever
			route ... description of the rail route
			position ... it is needed to determine the climb / descent
		OUT: delta v [m/s]
		'''
		
		rkA = self.eqMotion (vA, mode, route, position)
		rkB = self.eqMotion (vA + (0.5 * rkA), mode, route, position)
		rkC = self.eqMotion (vA + (0.5 * rkB), mode, route, position)
		rkD = self.eqMotion (vA + rkC, mode, route, position)

		return (rkA + 2.0*rkB + 2.0*rkC + rkD)*dt/6.0


	def motion (self, route, position, distance, speedA, speedB):
		'''
		Train passing the section of route
		
		IN: route ... description of the rail route
			position ... it is needed to determine the climb / descent [m]
			distance ... length of section [m]
			speedA ... actual initial velocity [km/h]
			speedB ... new speed limit at the end of section [km/h]
		OUT: list of DriveMark (time = 0, distance, speed)		
		'''

		dt = 0.25
		xA = 0.0
		result = []
		
		# uniform movement
		if (speedA == speedB):
			result.append( DriveMark(0, position + distance, speedA) )
		
		# accelerated motion
		if(speedA < speedB):
			vA = speedA
			while (xA < distance) and (vA < speedB):
				vB = vA + 3.6*self.deltaVelocity(dt, vA, 1, route, position + xA)	# [km/h]
				xB = (dt*vB)/3.6 + xA												# [m]
				vA = vB
				xA = xB	
				result.append( DriveMark(0, position + xB, vB) )
		
		# deccelerated motion
		if (speedA > speedB):		
			vA = speedB
			result.append( DriveMark(0.0, position + distance, speedB) )
			while (xA < distance) and (vA < speedA):
				vB = vA + 3.6*self.deltaVelocity(-dt, vA, -1, route, position + distance - xA)	# [km/h]
				xB = (dt*vB)/3.6 + xA															# [m]
				vA = vB
				xA = xB	
				result.insert(0, DriveMark(0, position + distance - xA, vB) )
		
		return result

#-------------------------------------------------------------------------------		
# T R A I N   D R I V E R


	def deltaSpeedDistance (self, speedA, speedB, route, position):
		'''
		Returns the distance needed to change the speed (speedA --> speedB)
		
		IN: speedA ... initial speed [km/h]
			speedB ... final speed [km/h]
			route ... description of the rail route
			position ... it is needed to determine the climb / descent [m]
		OUT: distance [m]
		'''
		
		dt = 0.25
		vA = speedA
		xA = 0.0
		xB = 0.0

		if (speedA > speedB):
			mode = -1
		elif (speedA < speedB):
			mode = 1
		else:
			return 0	

		# The lambda tests whether already achieved the required speed
		condition = lambda mode, current, final: True if (mode * (current - final)) < 0.0 else False
		while (condition(mode, vA, speedB)):
			vB = vA + 3.6*self.deltaVelocity(dt, vA, mode, route, position)	# [km/h]
			xB = (dt*vB)/3.6 + xA											# [m]
			vA = vB
			xA = xB

		return xB
					

	def getOutlook (self, speedA, speedB, distance, route, position):
		'''
		Returns the speed, distance in order to optimize the passage section
		(type: speedA ​​> speedB, initial_speed < speedA)

		IN: speedA ... initial speed [km/h]
		speedB ... speed limit of next section [km/h]
		distance ... lenght of current section [m]
		route ... description of the rail route
		position ... it is needed to determine the climb / descent [m]
		OUT: speed [km/h], distance [m]
				
		Note: We would like to train first accelerated and then started to slow
		down the speedB.
		'''
		speedlimit = route.getSpeedLimit(position)
		
		# If a sufficient distance to slow is less than the length of the section.
		if (self.deltaSpeedDistance(speedA, speedB, route, position) < distance):
			i = 0
			suff_distance = 0.0
			speedC = speedA + i			
			while ((suff_distance < distance) and (speedC <= speedlimit)):
				i += 0.2
				suff_distance = 0.0
				speedC = speedA + i
				result_distance = self.deltaSpeedDistance(speedA, speedC, route, position)
				suff_distance += self.deltaSpeedDistance(speedC, speedB, route, position)
				suff_distance += result_distance
				
			return (speedC-0.2),  result_distance

		return speedA, 0.0


	def run (self, route):
		'''
		Passing route
		IN: route ... description of the rail route
		OUT: list of informations on the driving (time, distance, velocity
		'''	
		
		position = 0.0
		speed = 0.0
		passage = []

		# for each section of rail route
		for i in range(len(route.route)-1):
			distance = route.route[i+1].distance - position
			speedA = route.route[i].speedlimit
			speedB = route.route[i+1].speedlimit

			# uniform drive or acceleration on current section
			if (speedA <= speedB):
				passage += self.motion(route, position, distance, speed, speedA)
				position += distance
				speed = passage[-1].speed

			# We need to slow down, but we will try to pass as quickly as possible.
			if (speedA > speedB):
				# So we simply divide the section into two parts, we will try to
				# reach a maximum speed so that we can still safely slow down.
				speedC, tmpdistance = self.getOutlook(speed, speedB, distance, route, position)

				passage += self.motion(route,position,tmpdistance,speed,speedC)
				position += tmpdistance
				speed = passage[-1].speed

				passage += self.motion(route,position,distance-tmpdistance,speed,speedB)				
				position += distance - tmpdistance
				speed = passage[-1].speed

		# calculation of travel time
		for i in range(len(passage)-1):
			delta_v = (passage[i+1].speed - passage[i].speed) / 2.0 + passage[i].speed
			delta_distance = passage[i+1].distance - passage[i].distance
			passage[i+1].time = math.fabs(2 * delta_distance / delta_v) + passage[i].time

		return passage



if __name__ == "__main__":

	import doctest
	doctest.testmod()
