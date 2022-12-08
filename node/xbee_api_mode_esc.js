
var SerialPort = require("serialport");
var xbee_api = require('xbee-api');

	
class XBEE {
	
	constructor(tty,objCallback) {
		
	  this.tty = tty;
	  this.objCallback = objCallback; // objet devant traiter le callback

	  this.xbeeAPI = null;

	  this.start();
	}
	
	callback(s) {
		this.objCallback.callback(s);
	}

	start() {
		 var xbee = this;
		 
		 var xbeeAPI = new xbee_api.XBeeAPI({
			  api_mode: 2
			});
		 
		 xbee.xbeeAPI = xbeeAPI
		 
		 var port = new SerialPort(xbee.tty, {
			  baudRate: 9600
			});
		 
		 port.pipe(xbeeAPI.parser);
		 xbeeAPI.builder.pipe(port);

		 port.on('open', function() {
		 	  console.log('opened : '+xbee.tty);
		 	  // https://www.npmjs.com/package/xbee-api
		 			
		 	  //port.write(frame)
		 	  
		 });

		 port.on('error', function(err) {
		 	  console.log('error ', err.message);
		 });

		 port.on('close', function () {
		 	console.log("closed ");
		 });

		 xbeeAPI.parser.on("data", function(frame) {
				//console.log(">>", frame);
				// on ne s'intéresse qu'aux frames de données
				// envoyées par l'Arduino

				// example de frame envoyée par l'Arduino
				//	{
				//	type: 129,
				//	remote16: '0000',
				//	rssi: 40,
				//	receiveOptions: 2,
				//	data: <Buffer 63 63 63 63 63>
				//	}

				if (frame.type == 0x81) { // 129
					xbee.callback(frame.data)
				}

			});


	}
	
	sendFrame(data) {
	 	  var frame = {
	 			    type: 0x00, // xbee_api.constants.FRAME_TYPE.TX_REQUEST_64 
	 			    id: 0x01, // optional, nextFrameId() is called per default
	 			    destination64: "000000000000FFFF",
	 			    options: 0x00, // optional, 0x00 is default
	 			    data: data // Can either be string or byte array.
	 			    // en status on aura un paquet de type 137 (0x89)
	 			    // en réception on aura un paquet de type 129 (0x81)
	 			}
	 			 
	 	  this.xbeeAPI.builder.write(frame);

	}
	
	
}

module.exports = {
		XBEE : XBEE
}
