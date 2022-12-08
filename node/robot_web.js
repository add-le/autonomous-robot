
// pour tester le robot
// http://localhost:3000/robot

// pour voir l'état du robot
// http://localhost:3000/lire_stat

// pour envoyer une commande au robot (avancer pendant 900 ms)
// http://localhost:3000/cmd?val=avancer_9


var fs = require('fs')
var express = require('express')
var bodyParser = require('body-parser')
const { send } = require('process')

var XBEE = require("./xbee_api_mode_esc.js").XBEE
// require("serialport");
// require('xbee-api');

var config = require("./config.js")

/****************************************************************************/
/******************************* missions du robot **************************/
/****************************************************************************/

var mission_avancer_3_secondes = require("./mission_avancer_3_secondes")
var mission_1 = require("./mission_1")
var mission_2 = require("./mission_2")
var mission_3 = require("./mission_3")
var mission_4 = require("./mission_4")
var mission_5 = require("./mission_5")
var mission_6 = require("./mission_6")


/****************************************************************************/
/******************************* configuration ******************************/
/****************************************************************************/

var port_web = config.port_web

var rep_html = config.rep_html
if (fs.existsSync(rep_html)) {
    console.log("rep_html OK : "+rep_html)
}
else {
    console.log("Erreur dossier non trouvé rep_html : "+rep_html)
}

var tty = config.tty
//console.log(process.argv.length)
if (process.argv.length > 2) { // si le tty est passé en paramètre (comX, /dev/tty, ...)
    tty = process.argv[2]
}
console.log("tty : "+tty)



/****************************************************************************/
/******************************* état du robot ******************************/
/****************************************************************************/

// etat courant du robot
var statr = {
    dist_mm : -1,
    dist_rate : -1,
    dist_status : -1,
}

/****************************************************************************/
/************************** mission courante du robot ***********************/
/****************************************************************************/

var mission = null // mission courante

/****************************************************************************/
/********************************** XBee ************************************/
/****************************************************************************/

var xbee = new XBEE(tty,{callback : function (data) {
	try {
        var s = data.toString()
        if (s.startsWith("{")) {
            var st = JSON.parse(s)
            if (typeof st.d !== "undefined") {
                statr.dist_mm = st.d
            }
            if (typeof st.dr !== "undefined") {
                statr.dist_rate = st.dr
            }
            if (typeof st.ds !== "undefined") {
                statr.dist_status = st.ds
            }
            //console.log("mm = "+stat.dist_mm+" rate = "+stat.dist_rate+" status = "+stat.dist_status)

            // gestion des missions

            if (mission != null) {
                mission.exec(statr,xbee)
                // on passe en paramètre l'état du robot
                // et xbee pour pouvoir envoyer des commandes au robot
            }
        }
        else {
            console.log("XBEE : "+s)
        }
    }
    catch (e) {}

}})

/****************************************************************************/
/*************************** serveur web ************************************/
/****************************************************************************/


var app = express();
app.use(bodyParser.json()); // on veut pouvoir passer en paramètre des objets json
app.use(bodyParser.urlencoded({extended: true}) ); // true si objets complexes

app.use("/robot", express.static(rep_html,{index:'index.html'}));

app.get('/cmd',function(req,res) {
    var val = req.query.val
    console.log("cmd = "+val)
    if (val == "stop") {
        // on annule toutes les missions
        mission = null
        // on arrête le robot
        xbee.sendFrame("xaa09gz") // arrêt des moteurs gauche et droit
	xbee.sendFrame("xaa09dz")
    }
    else if (val == "servo_droite") {
        xbee.sendFrame("xs-9..z")
    }
    else if (val == "servo_avant") {
        xbee.sendFrame("xs+0..z")
    }
    else if (val == "servo_gauche") {
        xbee.sendFrame("xs+9..z")
    }
    else if (val == "avancer_1") {
        xbee.sendFrame("xaa19gz")
        xbee.sendFrame("xaa19dz")
    }
    else if (val == "avancer_9") {
    	xbee.sendFrame("xaa99gz")
        xbee.sendFrame("xaa99dz")
    }
    else if (val == "avancer_3_secondes") {
        mission = mission_avancer_3_secondes // voir les require au début
        mission.init()
        mission.statm.fin = Date.now() + 3000 // indique le moment de fin en ms
    }
    else if (val == "mission_1") {
        mission = mission_1
        mission.init()
        mission.statm.fin = 0 
    }
    else if (val == "mission_2") {
        mission = mission_2
        mission.init()
        mission.statm.fin = 0 
    }
    else if (val == "mission_3") {
        mission = mission_3
        mission.init()
        mission.statm.fin = 0 
    }
    else if (val == "mission_4") {
        mission = mission_4
        mission.init()
        mission.statm.fin = 0
    }
    else if (val == "mission_5") {
        mission = mission_5
        mission.init()
        mission.statm.fin = Date.now() + 5000 
        mission.statm.id = 0 
    }
    else if (val == "mission_6") {
        mission = mission_6// voir les require au début
        mission.init()
        mission.statm.fin = 0 // indique le moment de fin en ms
    }

    res.send("ok")
})

app.get('/lire_stat',function(req,res) {
    res.send(statr)
})

var server = app.listen(port_web, function() {
    console.log('Express server listening on port ' + port_web)
});
