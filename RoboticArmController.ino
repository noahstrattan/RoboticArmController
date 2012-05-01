/*
  Florida Atlantic University
  Teleoperated Robotic Arm
  Noah Strattan
  Last modified: 04-30-2012
  
  Robotic arm controller code
  Recieves data from the computer and controls servos
  
  Setup:
  Arduino Uno
  Servos connected to pins 3,5,6,9,10,11
  Force sensor on pin A5
*/

// Libraries
#include <Servo.h>

#define ftl(x) ((x)>=0?(long)((x)+0.5):(long)((x)-0.5))  //float to long conversion

// Setup servo objects
Servo servo[6];   // Array of servos. Ordered from base up

// Arrays for servo positioning
long servoPos[6] = {90,90,90,90,90,90};
long currentPos[6] = {90,90,90,90,90,90};

void setup() {
  Serial.begin(9600);      // Start serial connection
  
  // Attach servos and set pulse limits (2 and 3 switched because of wiring error)
  servo[0].attach(3, 900, 2100);    // Base servo          HS-475HB (900 - 2100)
  servo[1].attach(5, 900, 2100);    // Shoulder servo      HS-805BB (900 - 2100)
  servo[3].attach(6, 900, 2100);    // Elbow servo         HS-755HB (900 - 2100)
  servo[2].attach(9, 900, 2100);    // Wrist tilt servo    HS-645MG (900 - 2100)
  servo[4].attach(10, 900, 2100);   // Wrist rotate servo  HS-225MG (900 - 2100)
  servo[5].attach(11, 900, 2100);   // Grip servo          HS-422 (900 - 2100)
  
  // Init servo positions
  servo[0].write(90);
  servo[1].write(90);
  servo[2].write(90);
  servo[3].write(90);
  servo[4].write(90);
  servo[5].write(90);
}

void loop() {
  // Read serial values and output to arm
  if(Serial.available() > 7){
    if(Serial.read() == 48 && Serial.read() == 151){
      servoPos[0] = Serial.read();
      servoPos[1] = Serial.read();
      servoPos[2] = Serial.read();
      servoPos[3] = Serial.read();
      servoPos[4] = Serial.read();
      servoPos[5] = Serial.read();
      
      servoPos[5] = 180 - servoPos[5];
      
      // Servo pulses
      float bas_servopulse = 1500.0 - ((servoPos[0] - 90) * 6.6);
      float shl_servopulse = 1500 + ((servoPos[1] - 90.0) * 6.6);
      float elb_servopulse = 1500 -  ((servoPos[2] - 90.0) * 6.6);
      float wri_servopulse = 1500 + ((servoPos[3] - 90)  * 6.6);
      
      servo[4].write(servoPos[4]);
      servo[5].write(servoPos[5]);
      
      servo[0].writeMicroseconds(ftl(bas_servopulse));
      servo[1].writeMicroseconds(ftl(shl_servopulse));
      servo[2].writeMicroseconds(ftl(elb_servopulse));
      servo[3].writeMicroseconds(ftl(wri_servopulse));
     
    }
  }
  
  // Feedback code
  byte feedback[2] = {48,0};  // Start byte (48) and data byte
  feedback[1] = analogRead(5) / 4;  // read sensor. divide by 4 to scale to byte
  Serial.write(feedback,2);  // output to computer
  
}

// Experimental function, not used as of now
// Moves servo, incorperating an esaing method to smooth servo movement
void moveServoSmooth(int i){
  while(abs(currentPos[i] - servoPos[i]) > 2){
    if(currentPos[i] > servoPos[i]){
      currentPos[i] = currentPos[i] + ((currentPos[i] - servoPos[i]) * 0.5);
    }
    else if(currentPos[i] < servoPos[i]){
      currentPos[i] = currentPos[i] - ((servoPos[i] - currentPos[i]) * 0.5);
    }
    servo[i].write(currentPos[i]);
    delay(2);
  }
}
