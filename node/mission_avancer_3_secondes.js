
// on fait avancer le robot pendant 3 secondes
// sauf s'il y a un obstacle à moins de 100 mm

// état de la mission
// fin indique le moment de la fin de la mission, arrêt si -1
var statm = {
	nom : "avancer_3_secondes",
	fin : -1,
}

function init() {
	console.log("Init mission : "+statm.nom)
}

function exec(statr,xbee) { // statr : état du robot, xbee : envoi des commandes au robot par XBee

	// si le robot est à moins de 100 mm d'un obstacle, la mission s'arrête

	//console.log("dist = "+(0+statr.dist_mm))

	if (statm.fin != -1) {
		if (statr.dist_mm < 100) {
			statm.fin = -1 // mission desactivée
			console.log("Mission desactivée : "+statm.nom)
		}
		else {
			if (Date.now() > statm.fin) {
				statm.fin = -1 // mission desactivée
				xbee.sendFrame("xaa09z") // arrêt des moteurs gauche et droit
			}
			else {
				xbee.sendFrame("xaa99z") // moteurs gauche et droit à la vitesse max pendant 900 ms
			}
		}
	}

}

module.exports = {
	statm : statm,
	init : init,
    exec : exec,
}


