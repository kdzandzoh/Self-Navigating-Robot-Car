//The servo library is a requirement for the servo to operate
//The NewPing library provides better distance measurements using the sensor
#include <Servo.h>         
#include <NewPing.h>    

//Declaring the Pins of the Motors
const int LeftMotorForward = 12;
const int LeftMotorBackward = 11;
const int RightMotorForward = 10;
const int RightMotorBackward = 9;

//Declaring the Pins of the Ultrasound Sensor
const int trigPin = 1;
const int echoPin = 2;

int distance;
int duration;

servo myservo; 

void setup(){
//Serial monitor, servo, and pin setup
  pinMode(LeftMotorForward, OUTPUT);
  pinMode(LeftMotorBackward, OUTPUT);
  pinMode(RightMotorForward, OUTPUT);
  pinMode(RightMotorBackward, OUTPUT);


  pinMode(trigPin,OUTPUT);
  pinMode(echoPin,INPUT);
  
  myservo.attach(13); 

  myservo.write(90);
  delay(2000);
  distance = getDistance();
  delay(100);
  distance = getDistance();
  delay(100);
  distance = getDistance();
  delay(100);
  distance = getDistance();
  delay(100);
  
  Serial.begin(9600);
}

void loop(){
  
  distance = getDistance();
  int distanceRight = 0;
  
//If the ultrasound detects that the distance is less than or equal to 20 cm,
//it will stop, look right and determine that new distance when looking right
  if (distance <= 20){
    stopCar();
    delay(300);
    
    //When the car stops, the red LED turns on until it makes a decision
    digitalWrite(9, HIGH);
    digitalWrite(8, LOW);
    digitalWrite(7, LOW); 
    
    distanceRight = lookRight();
    delay(300);
       
//If the distance when it looks right is less than or equal to 20cm, the car will
//turn left because that warns the car of an obstacle to the right.
  if (distanceRight <= 20){
      turnLeft();
      stopCar();
    
    //When the car stops, the blue LED turns on as it decides to turn left and 
    // will only switch back to green after the car has finished its turn 
    digitalWrite(8, LOW);
    digitalWrite(7, HIGH);
    digitalWrite(6, LOW); 
    }
    
//If the distance when it looks right is greater than 20cm, the car will turn
//right because there is no obstacle there. Instead of checking left and right,
//we decided to look in one direction and make a decision from there,
//ultimately allowing the car to navigate the maze quicker. 
  if (distanceRight > 20) {
      turnRight();
      stopCar();
    
    //When the car stops, the white LED turns on as it decides to turn right and 
    // will only switch back to green after the car has finished its turn 
    digitalWrite(8, HIGH);
    digitalWrite(7, HIGH);
    digitalWrite(6, HIGH); 
  }

//If none of those distances apply (no wall blocking the car), it will simply 
//advance forward
  else{
    moveForward(); 
    
    //If there are no obstacles, the green LED is on as the car advances forward.
    digitalWrite(8, LOW);
    digitalWrite(7, LOW);
    digitalWrite(6, HIGH); 
  }
}

int lookRight(){  
  myservo.write(10);
  delay(500);
  int distance = getDistance();
  delay(100);
  myservo.write(90);
  return distance;
}

void stopCar(){ //None of the motors are powered

  digitalWrite(RightMotorForward, 255);
  digitalWrite(LeftMotorForward, 255);
  digitalWrite(RightMotorBackward, 255);
  digitalWrite(LeftMotorBackward, 255);
}

void moveForward(){ 
//The motors with the forward inputs are powered, the backward ones are not.  
    analogWrite(LeftMotorForward, 0);
    analogWrite(RightMotorForward, 0);

    analogWrite(LeftMotorBackward, 255);
    analogWrite(RightMotorBackward, 255); 
}

void turnRight(){
//Turn to get the initial rotation. The motors turn in opposite directions
//in order to maximize the force (torque)
  digitalWrite(LeftMotorForward, 0);
  digitalWrite(RightMotorBackward, 0);
  
  digitalWrite(LeftMotorBackward, 255);
  digitalWrite(RightMotorForward, 255);
}

void turnLeft(){
//Turn to get the initial rotation. The motors turn in opposite directions
//in order to maximize the force (torque)
  digitalWrite(LeftMotorBackward, 0);
  digitalWrite(RightMotorForward, 0);
  digitalWrite(LeftMotorForward, 255);
  digitalWrite(RightMotorBackward, 255);
}

//Function that the sensor will use to determine the distance 
//between it and a potential object
void getDistance(){
  digitalWrite(trigPin,LOW); //Resets the trigPin signal
  delayMicroseconds(2); 
  
  digitalWrite(trigPin,HIGH); //trigPin sends out the signal
  delayMicroseconds(10);
  
  digitalWrite(trigPin,LOW); //Then the trigPin goes low
  
  duration=pulseIn(echoPin,HIGH); //duration is the pulseIn to the echoPin	 
  distance=duration*0.0345/2; //Converts the duration to distance in cm
  Serial.println(distance);  
}

