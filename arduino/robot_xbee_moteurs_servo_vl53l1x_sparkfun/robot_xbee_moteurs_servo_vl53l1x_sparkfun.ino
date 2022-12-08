
#include <XBee.h>
XBee xbee = XBee();

#define MAXBUF 80
char xbee_buffer_envoyer[MAXBUF];
char xbee_buffer_recevoir[MAXBUF];

void xbee_envoyer() {
	XBeeAddress64 addr64 = XBeeAddress64(0x00000000, 0x0000FFFF);
	Tx64Request tx = Tx64Request(addr64, (uint8_t*)xbee_buffer_envoyer, strlen(xbee_buffer_envoyer));
	xbee.send(tx);
}

#include <Wire.h>

// gestion d'un timeout moteur
unsigned long time_moteur = 0;		
// indique le début de la dernière commande moteur (valeurs en ms)
unsigned long timeout_moteur = 1000;	
// arret des moteurs si pas de nouvelle commande envoyée


// gestion d'un timeout capteurs pour l'envoi des données toutes les n ms
unsigned long time_capteurs = 0;		
unsigned long timeout_capteurs = 100; // envoi toutes les 100 ms


/********************************* setup ****************************/

void setup() {
	Serial.begin(9600);
	xbee.begin(Serial);

	xbee_buffer_envoyer[0] = 0;
	xbee_buffer_recevoir[0] = 0;

	Wire.begin();

	servo_init();
	moteur_init();

	vl53l1x_init();


	delay(2000);
}


void loop() {


	xbee.readPacket();
    
	// lecture paquet XBee
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

			//softwareSerial.println(xbee_buffer_recevoir);
		}
	}

	lire_commande();



	//softwareSerial.println(millis());
	//softwareSerial.println(time_robot + timeout_robot);
	//softwareSerial.println();

	// arrêt du robot à la fin de la commande moteur
	if (millis() > time_moteur + timeout_moteur) {
		moteurs_stop();
	}

	// envoi des données des capteurs
	if (millis() > time_capteurs + timeout_capteurs) {
		vl53l1x_lire();
		envoi_donnees_capteurs();
		time_capteurs = millis();
	}

}

/*******************************************************************/
/************************** gestion des commandes ******************/
/*******************************************************************/

// une commande est une séquence de 6 caractères

int c1 = -1; // x indique debut commande
int c2 = -1; // commande partie 1
int c3 = -1; // commande partie 2
int c4 = -1; // commande partie 3
int c5 = -1; // commande partie 4
int c6 = -1; // z indique fin commande
int c7 = -1;

// commandes moteur
// c2 : a ou r (moteur gauche, avant ou reculer)
// c3 : a ou r (moteur droite, avancer ou reculer)
// c4 : 0..9 (vitesse moteur, 0 : arrêt, 9 : max)
// c5 : 1..9 (durée commande, 1 : 100 ms ; 2 : 200 ms ; ... ; 9 : 900 ms)

// commande servo
// c2 : s
// c3 : - ou + (sens de rotation du servo)
// c4 : 0 : 0° , 1 : 10°, 2 : 10°, ..., 9: 90° (rotation du servo)

void lire_commande() {

		c1 = xbee_buffer_recevoir[0];
		c2 = xbee_buffer_recevoir[1];
		c3 = xbee_buffer_recevoir[2];
		c4 = xbee_buffer_recevoir[3];
		c5 = xbee_buffer_recevoir[4];
  		c6 = xbee_buffer_recevoir[5];
  		c7 = xbee_buffer_recevoir[6];

		if ((c1 == 'x') && (c7 == 'z')) {

			sprintf(xbee_buffer_envoyer,"cmd c1 = %c c2 = %c c3 = %c c4 = %c c5 = %c c6 = %c c7 = %c",(char)c1,(char)c2,(char)c3,(char)c4,(char)c5,(char)c6,(char)c7);
			xbee_envoyer();
			xbee_buffer_envoyer[0] = 0;

			exec_commande();
		}

		for (int i=0 ; i<MAXBUF ; i++) {
			xbee_buffer_recevoir[i] = 0;
		}



}

void exec_commande() {

	

	if (c2 == 's') { // commande servo
		int angle = 90;
		if (c3 == '+') {
			if (c4 == '1') angle = 100;
			else if (c4 == '2') angle = 110;
			else if (c4 == '3') angle = 120;
			else if (c4 == '4') angle = 130;
			else if (c4 == '5') angle = 140;
			else if (c4 == '6') angle = 150;
			else if (c4 == '7') angle = 160;
			else if (c4 == '8') angle = 170;
			else if (c4 == '9') angle = 180;		
		}
		else if (c3 == '-') {
			if (c4 == '1') angle = 80;
			else if (c4 == '2') angle = 70;
			else if (c4 == '3') angle = 60;
			else if (c4 == '4') angle = 50;
			else if (c4 == '5') angle = 40;
			else if (c4 == '6') angle = 30;
			else if (c4 == '7') angle = 20;
			else if (c4 == '8') angle = 10;
			else if (c4 == '9') angle = 0;		
		}
		servo_angle(angle);
	}

	if ((c2 == 'a') || (c2 == 'r')) { // commande moteur

		int speed = 255;
		if (c4 == '0') speed = 0;
		else if (c4 == '1') speed = 135;
		else if (c4 == '2') speed = 150;
		else if (c4 == '3') speed = 165;
		else if (c4 == '4') speed = 180;
		else if (c4 == '5') speed = 195;
		else if (c4 == '6') speed = 210;
		else if (c4 == '7') speed = 225;
		else if (c4 == '8') speed = 240;
		else if (c4 == '9') speed = 255;

		timeout_moteur = 1000;
		if (c5 == '1') timeout_moteur = 100;
		else if (c5 == '2') timeout_moteur = 200;
		else if (c5 == '3') timeout_moteur = 300;
		else if (c5 == '4') timeout_moteur = 400;
		else if (c5 == '5') timeout_moteur = 500;
		else if (c5 == '6') timeout_moteur = 600;
		else if (c5 == '7') timeout_moteur = 700;
		else if (c5 == '8') timeout_moteur = 800;
		else if (c5 == '9') timeout_moteur = 900;
	  if(c6 == 'g') 
    {
          if (c2 == 'a') moteur_gauche_avant(speed);
          else if (c2 == 'r') moteur_gauche_arriere(speed);
    }
		else if(c6 =='d')
    {
          if (c3 == 'a') moteur_droite_avant(speed);
        else if (c3 == 'r') moteur_droite_arriere(speed);
    }

		
		time_moteur = millis();

		// sprintf(xbee_buffer_envoyer,"time = %ld",time_moteur);

		// xbee_envoyer();
		// xbee_buffer_envoyer[0] = 0;

	
	}

}

/*******************************************************************/
/********************* moteurs - shield DRI0009 ********************/
/*******************************************************************/


//Arduino PWM Speed Control：
int E1 = 5;  
int M1 = 4; 
int E2 = 6;                      
int M2 = 7;                        


void moteur_init() {
	pinMode(M1, OUTPUT);   
	pinMode(M2, OUTPUT); 
}




void moteur_gauche_avant(int speed) {
	digitalWrite(M1,LOW);
	analogWrite(E1, speed);   //PWM Speed Control
}

void moteur_gauche_arriere(int speed) {
	digitalWrite(M1,HIGH);
	analogWrite(E1, speed);   //PWM Speed Control
}

void moteur_droite_avant(int speed) {
 	digitalWrite(M2,LOW);
	analogWrite(E2, speed);   //PWM Speed Control
}

void moteur_droite_arriere(int speed) {
	digitalWrite(M2,HIGH);
	analogWrite(E2, speed);   //PWM Speed Control
}

void moteurs_stop() {
	analogWrite(E1, 0);   //PWM Speed Control
	analogWrite(E2, 0);   //PWM Speed Control
}

/*******************************************************************/
/********************************* servo ***************************/
/*******************************************************************/

#include <Servo.h>

Servo myservo;

void servo_init() {
	myservo.attach(8);
}

void servo_angle(int angle) { // 0 .. 180
	myservo.write(angle);
}






/*******************************************************************/
/********************************* VL53L1X *************************/
/*******************************************************************/

#include <ComponentObject.h>
#include <RangeSensor.h>
#include <SparkFun_VL53L1X.h>
#include <vl53l1x_class.h>
#include <vl53l1_error_codes.h>
#include "SparkFun_VL53L1X.h" //Click here to get the library: http://librarymanager/All#SparkFun_VL53L1X

#define SHUTDOWN_PIN 2
#define INTERRUPT_PIN 3

SFEVL53L1X distanceSensor;
//Uncomment the following line to use the optional shutdown and interrupt pins.
//SFEVL53L1X distanceSensor(Wire, SHUTDOWN_PIN, INTERRUPT_PIN);

int vl53l1x_dist = -1;
int vl53l1x_dist_rate = -1;
int vl53l1x_dist_status = -1;

void vl53l1x_init() {
    if (distanceSensor.begin() != 0) //Begin returns 0 on a good init
  {
    Serial.println("Sensor failed to begin. Please check wiring. Freezing...");
    while (1)
      ;
  }

  distanceSensor.setDistanceModeShort();
  //distanceSensor.setDistanceModeLong();

  Serial.println("Sensor online!");

}

void vl53l1x_lire() {
  distanceSensor.startRanging(); //Write configuration bytes to initiate measurement
  while (!distanceSensor.checkForDataReady())
  {
    delay(1);
  }
  int distance = distanceSensor.getDistance(); //Get the result of the measurement from the sensor
  distanceSensor.clearInterrupt();
  distanceSensor.stopRanging();

  //Serial.print("Distance(mm): ");
  //Serial.print(distance);
  int signalRate = distanceSensor.getSignalRate();
  //Serial.print("\tSignal rate: ");
  //Serial.print(signalRate);

  byte rangeStatus = distanceSensor.getRangeStatus();
  //Serial.print("\tRange Status: ");
  //Serial.print(rangeStatus);
  //Serial.println();

 vl53l1x_dist = distance;
 vl53l1x_dist_rate = signalRate;
 vl53l1x_dist_status = rangeStatus; // 0 : OK ; 7 : NOK


}




/*******************************************************************/
/******************* envoi des données des capteurs ****************/
/*******************************************************************/

void envoi_donnees_capteurs() {
	/*
    Serial.write("{");
    Serial.write("\"d\":");
	Serial.print(vl53l1x_dist);
	Serial.write(",");
    Serial.write("\"dr\":");
	Serial.print(vl53l1x_dist_rate);
	Serial.write(",");
    Serial.write("\"ds\":");
	Serial.print(vl53l1x_dist_status);
	Serial.write("}\n");
*/

	sprintf(xbee_buffer_envoyer,"{\"d\":%d,\"dr\":%d,\"ds\":%d}",vl53l1x_dist,vl53l1x_dist_rate,vl53l1x_dist_status);

	xbee_envoyer();
	xbee_buffer_envoyer[0] = 0;

}
