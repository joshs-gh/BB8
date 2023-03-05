// https://sites.google.com/stonybrook.edu/arduinoble/
// https://www.youtube.com/watch?v=qFbfLk-jUd4 - His instructions to use 0/1 broke USB upload w/ avrdude
// https://www.amazon.com/gp/product/B01DG61YRM - Soldered m/f to pins 9/13 for TX/RX on HM-10: https://www.amazon.com/gp/product/B06WGZB2N4


#include <AFMotor.h>
#include <SoftwareSerial.h>
#include <ArduinoBlue.h>

//creates two objects to control two terminals on the motor shield
AF_DCMotor rightMotor(1);
AF_DCMotor leftMotor(2);

const unsigned long BAUD_RATE = 9600;
const int BLUETOOTH_TX = 9;
const int BLUETOOTH_RX = 13;
const int LEFTSPEED = 100; // 215;
const int RIGHTSPEED = 100; //215;
const int LEFTADJUST = 40;
const int RIGHTADJUST = 40;

int prevThrottle = 49;
int prevSteering = 49;
int throttle, steering;

SoftwareSerial bluetooth(BLUETOOTH_TX, BLUETOOTH_RX);
ArduinoBlue phone(bluetooth); // pass reference of bluetooth object to ArduinoBlue constructor


void setup()
{
  Serial.begin(BAUD_RATE);  //Set the baud rate to your Bluetooth module.
  bluetooth.begin(BAUD_RATE);
  delay(100);
  Serial.println("setup complete");
}

void loop() {

  // Throttle and steering values go from 0 to 99.
  // When throttle and steering values are at 99/2 = 49, the joystick is at center.
  throttle = phone.getThrottle();
  steering = phone.getSteering();

  // Display throttle and steering data if steering or throttle value is changed
  if (prevThrottle != throttle || prevSteering != steering) {
    Serial.print("Throttle: "); Serial.print(throttle); Serial.print("\tSteering: "); Serial.println(steering);
    prevThrottle = throttle;
    prevSteering = steering;
  }

  roll();
}

void roll()
{
  if ((throttle >= 45 && throttle <=53) && steering == 49) {
    Stop();
    delay(50);
    return;
  }
  int speedAdjLeft = map(steering, 0, 99, -LEFTADJUST, LEFTADJUST);
  int speedAdjRight = map(steering, 0, 99, -RIGHTADJUST, RIGHTADJUST);
  Serial.print("Setting Speeds - Left: ");Serial.print(LEFTSPEED+speedAdjLeft); Serial.print("\tRight: "); Serial.println(RIGHTSPEED-speedAdjRight);
  leftMotor.setSpeed(LEFTSPEED+speedAdjLeft);
  rightMotor.setSpeed(RIGHTSPEED-speedAdjRight); 

  if (throttle > 53) {
    forward();
  }
  if (throttle < 45)  {
    back();
  }
}

void forward()
{
  rightMotor.run(BACKWARD); //rotate the motor clockwise
  leftMotor.run(FORWARD); //rotate the motor clockwise
}

void back()
{
  rightMotor.run(FORWARD); //rotate the motor counterclockwise
  leftMotor.run(BACKWARD); //rotate the motor counterclockwise
}

void Stop()
{
  rightMotor.setSpeed(0);
  leftMotor.run(RELEASE); //turn rightMotor off
  leftMotor.setSpeed(0);
  leftMotor.run(RELEASE); //turn leftMotor off
}
