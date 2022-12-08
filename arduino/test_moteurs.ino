// Arduino PWM Speed Control：
// https://wiki.dfrobot.com/Arduino_Motor_Shield__L298N___SKU_DRI0009_

int E1 = 5;
int M1 = 4;
int E2 = 6;
int M2 = 7;

void setup() {
    pinMode(M1, OUTPUT);
    pinMode(M2, OUTPUT);
    //Serial.begin(9600);
}

int count = 0;

void loop() {
    //Serial.print("count = ");
    //Serial.println(count);
    count++;
    if (count > 1) {
            analogWrite(E1, 0);
            analogWrite(E2, 0);
    }
    else {
        int value;
        for(value = 0 ; value <= 255; value+=5) {
            digitalWrite(M1,HIGH);      // moteur 1 marche avant
            digitalWrite(M2, HIGH);     // moteur 2 marche avant
            analogWrite(E1, value);   //PWM Speed Control
            analogWrite(E2, value);   //PWM Speed Control
            delay(30);
        }
    }

    delay(1000);
}
