var XBEE = require("./xbee_api_mode_esc.js").XBEE

var xbee = new XBEE("/dev/ttyACM0",{callback : function (data) {
	try {
        var s = data.toString()
        console.log("XBEE : "+s)
    }
    catch (e) {}

}})

var count = 0

function envoyer() {
    count++
    xbee.sendFrame("envoi "+count)
}

setTimeout(() => {
    envoyer()
}, 6000);

setTimeout(() => {
    envoyer()
}, 7000);