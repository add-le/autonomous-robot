

var statm = {
	nom : "mission_3",
	fin : -1,
}

function init() {
	console.log("Init mission : "+statm.nom)
}

function exec(statr,xbee) { 

	if (statm.fin != -1) {
		if (statr.dist_mm < 100) {
			xbee.sendFrame("xaa29gz")
        		xbee.sendFrame("xaa01dz")
			
		}
		else if(statr.dist_mm > 100) {
			
				// moteurs gauche et droit à la vitesse max pendant 900 ms
				xbee.sendFrame("xaa39gz")
        			xbee.sendFrame("xaa39dz")
			
		}
	}
	
	

}

module.exports = {
	statm : statm,
	init : init,
    exec : exec,
}


