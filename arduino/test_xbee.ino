
// test XBee
// quand un programme node envoie un paquet XBee
// on lit le paquet et on le retourne précédé de la chaîne "Arduino : "

// test en utilisant node/test_xbee.js




#include <XBee.h>
XBee xbee = XBee();

char xbee_buffer_envoyer[80];
char xbee_buffer_recevoir[80];

void xbee_envoyer() {
	XBeeAddress64 addr64 = XBeeAddress64(0x00000000, 0x0000FFFF);
	Tx64Request tx = Tx64Request(addr64, (uint8_t*)xbee_buffer_envoyer, strlen(xbee_buffer_envoyer));
	xbee.send(tx);
}

/********************************* setup ****************************/

void setup() {
	Serial.begin(9600);
	xbee.begin(Serial);

	xbee_buffer_envoyer[0] = 0;
   	  		
	delay(4000);
}


void loop() {
	xbee.readPacket();
    
	// si un paquet arrive par XBee on le renvoie
	if (xbee.getResponse().isAvailable()) {
		if (xbee.getResponse().getApiId() == RX_16_RESPONSE) { // 129 0x81
			//softwareSerial.println("packet");
			strcpy(xbee_buffer_recevoir,"");
			int pos = strlen(xbee_buffer_recevoir);
			for (int i=4; i < xbee.getResponse().getFrameDataLength(); i++) {
				xbee_buffer_recevoir[pos] = xbee.getResponse().getFrameData()[i];
				//softwareSerial.print((char)xbee.getResponse().getFrameData()[i]);
				pos++;
				xbee_buffer_recevoir[pos] = 0;
			}

			if (strlen(xbee_buffer_recevoir) > 0) {
				strcpy(xbee_buffer_envoyer,"Arduino : ");
				strcat(xbee_buffer_envoyer,xbee_buffer_recevoir);
        xbee_buffer_recevoir[0] = 0;
				xbee_envoyer();
			}
		}
	}


}


