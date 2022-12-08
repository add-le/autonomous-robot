// Longer un mur sur 1 ou 2 mètres en arrêtant le robot s'il arrive trop près du mur

var statm = {
	nom : "mission_2",
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


