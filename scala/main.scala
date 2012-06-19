class RouteMark (val distance : Double, val speedlimit : Double, val slope : Double)


class RailRoute {

	var route = List[RouteMark]()

	def loadFromFile (file: String) {

		import scala.io.Source		
		for (line <- Source.fromFile(file).getLines()) {
			val seq = line.split(' ')
			route = route ::: List(new RouteMark(seq(0).toDouble, seq(1).toDouble, seq(2).toDouble))
		}
	}
}


class Train {

	val maxspeed = 140		// [km/h]
	val power = 2000		// [kW]
	val edb = 1700			// [kW]
	val weight = 155.5		// [t] 
	val lenght = 79.2		// [m]
	val wcount = 3			// number of wagons

	val rho_a = 0.030624
	val rho_b = 0.07424
	val rho_c = 1.35
	val rho_d = 14.973

}


class Drive (val train : Train) {

	def Adhesion (velocity : Double) : Double = {
		(7500.0/((velocity).abs+44.0) + 161.0)*0.001
	}

	def ResistForce (velocity : Double) : Double = {
		((velocity*velocity*train.rho_a) + (velocity*train.rho_b) + train.rho_c)*9.81
	}

/*
	def SlopeResistForce (route, position):

		train.weight * 9.81 * route.getSlope(position)
*/

}


object TestTrain extends App {

	var route = new RailRoute
	var vehicle = new Train
	var driving = new Drive (vehicle)
	
	route.loadFromFile("sample_A.route")
	println("distance\tspeedlimit\tslope")
	for (r <- route.route) {
		println("" + r.distance + '\t' + r.speedlimit + '\t' + r.slope)
	}
	
	

	
//	var i : Int = 0
//	for(i <- 0 to 160) {
//		println(i + "\t" + driving.Adhesion(i) + "\t" + driving.ResistForce(i));
//	}

}
