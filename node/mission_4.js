

var statm = {
	nom : "mission_4",
	fin : -1,
	
}

function init() {
	console.log("Init mission : "+statm.nom)
}

function exec(statr,xbee) { 

	if (statm.fin != -2) {
		if (statr.dist_mm < 100) {
			if(Date.now() > statm.fin)
			{
			statm.fin =-2
			statm.fin = Date.now() + 3000
			}
			
			xbee.sendFrame("xaa39gz")
        		xbee.sendFrame("xaa01dz")
        		
		}
		else if(statr.dist_mm > 100) {
			
				// moteurs gauche et droit à la vitesse max pendant 900 ms
				console.log("It : "+Date.now())
				xbee.sendFrame("xaa39gz")
        			xbee.sendFrame("xaa39dz")
        			statm.fin = Date.now() + 500
			
		}
	}else if(statm.fin == -2)
	{
		if(Date.now() > statm.fin)
		{
			statm.fin =0			
		}
			
		xbee.sendFrame("xaa99gz")
        	xbee.sendFrame("xaa09dz")

	
	}
	
	

}

module.exports = {
	statm : statm,
	init : init,
    exec : exec,
}


