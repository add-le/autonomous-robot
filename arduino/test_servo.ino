#include <Servo.h>

Servo myservo;

void setup() {
  Serial.begin(9600);
  Serial.println("< START >");
  
  myservo.attach(8);   // Servo control
}

int count = 0;

void loop() {
  if (count > 0) {
    myservo.write(90);
    return;
  }
  count++;
  
 for(int pos = 0; pos <= 180; pos += 1)  {
    myservo.write(pos);
    delay(20);
  }
  for(int pos = 180; pos>=0; pos-=1)  {
    myservo.write(pos);
     delay(20);
  }
}

