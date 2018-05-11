//https://howtomechatronics.com/tutorials/arduino/ultrasonic-sensor-hc-sr04/ Sensor code I referenced while programming.
//https://github.com/RichOwen789/Obstacle-Dodging-Arduino-Car GitHub repository
//********************************************* Setup

//set up start button
const int startButton = 12;
boolean start = false;

//set up ultrasonic sensor
const int trigPin = 8;
const int echoPin = 9;

//set up cart for driving
#include <Servo.h>
Servo rmotor;
Servo lmotor;
const int rstop = 93; //right motor stops given 93
const int lstop = 92; //left motor stops given 92... almost...
long timeLastAdjustment = 0; //angle must be adjusted, sice left motor never fully stops.
int cxy = 10; //To Be Determined... controls distance traveled. Constant multiplier.
int ca = 100; //To Be Determined... controls angle of rotation. Constant multiplier.
int tspeed = 5; //TBD... how fast should the servos turn?

void setup() {
  rmotor.attach(10); //attatch motors to pins 10 and 11
  lmotor.attach(11);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(startButton, INPUT);
  Serial.begin(9600);
}

//********************************************* Methods

//ultrasonic sensor methods
int objDistance() {

  long averageDistance = 0;
  for(int i = 0; i < 100; i++) {
    digitalWrite(trigPin, LOW); //set it low before high, clean
    delayMicroseconds(200);
  
    digitalWrite(trigPin, HIGH); //trigger to send sound
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
  
    long duration = pulseIn(echoPin, HIGH); //sound travel time
    long distance = duration * 0.034 / 2; //distance to object in cm.
  
    averageDistance += distance;
  }

  averageDistance = averageDistance / 25;
  Serial.println(averageDistance);
  
  return averageDistance;
}

//cart motion methods
void straight() {
  lmotor.write(lstop+tspeed); //start moving both motors
  rmotor.write(rstop-tspeed); //there is slight difference/error in movement speeds.
  delay(cxy*10); //multiplied by constant cxy
}

void rturn() {
  rmotor.write(rstop+tspeed);
  lmotor.write(lstop+tspeed);
  delay(ca); //turn right
}

void lturn() {
  rmotor.write(rstop-tspeed);
  lmotor.write(lstop-tspeed);
  delay(ca); //turn left
}

void adjustAngle() { //left motor never fully stops.
  lmotor.write(lstop + 2);
  delay((millis() - timeLastAdjustment)*3/24);
  lmotor.write(lstop);
  timeLastAdjustment = millis();
}

void rest(int t) {
  rmotor.write(rstop);
  lmotor.write(lstop);
  for(int i = 0; i < t/10; i++) { //this for loop makes code smoother
    delay(10);
    adjustAngle();
  }
}

void drive() {
  
  if(objDistance() > 8) {
    straight();
  }
  else {
    if(random(100) < 50) {
      while(objDistance() < 15) {
        lturn();
      }
    }
    else {
      while(objDistance() < 15) {
        rturn();
      }
    }
  }
}

//********************************************* Main loop

void loop() {
  if(start) {
    drive();
  }
  else {
    rest(10);
    if(digitalRead(startButton) == HIGH) {
      start = true;
      rest(2000);
    }
  }
}

//********************************************* testing methods

void printDistance() {
  Serial.print("Distance: ");
  Serial.println(objDistance());
  delay(10);
}



























