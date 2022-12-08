// Faire avancer le robot tout droit en l'arrêtant si un obstacle est détecté en face

var statm = {
	nom : "mission_1",
	fin : -1,
}

function init() {
	console.log("Init mission : "+statm.nom)
}

function exec(statr,xbee) {

	if (statm.fin != -1) {
		if (statr.dist_mm < 100) {
			statm.fin = -1 // mission desactivée
			console.log("Mission desactivée : "+statm.nom)
		}
		else {
			if (statm.fin == -1) {
				statm.fin = -1 // mission desactivée
				 
				xbee.sendFrame("xaa09gz") // arrêt des moteurs gauche et droit
				xbee.sendFrame("xaa09dz")
				
			}
			else {
				// moteurs gauche et droit à la vitesse max pendant 900 ms
				xbee.sendFrame("xaa99gz")
        			xbee.sendFrame("xaa99dz")
			}
		}
	}

}

module.exports = {
	statm : statm,
	init : init,
    exec : exec,
}


