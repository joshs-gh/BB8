
#include <AFMotor.h>
#include <SoftwareSerial.h>
#include <ArduinoBlue.h>

//creates two objects to control two terminals on the motor shield
AF_DCMotor motor1(2);
AF_DCMotor motor2(4);

const unsigned long BAUD_RATE = 9600;
const int BLUETOOTH_TX = 9;
const int BLUETOOTH_RX = 13;
const int LEFTSPEED = 175;
const int RIGHTSPEED = 215;

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
  if (throttle == 49 && steering == 49) {
    Stop();
    delay(50);
    return;
  }
  int speedAdj = map(steering, 0, 99, -40, 40);
  motor1.setSpeed(LEFTSPEED+speedAdj); 
  motor2.setSpeed(RIGHTSPEED-speedAdj);

  if (throttle > 49) {
    forward();
  }
  else {
    back();
  }
}

void forward()
{
  motor1.run(BACKWARD); //rotate the motor clockwise
  motor2.run(FORWARD); //rotate the motor clockwise
}

void back()
{
  motor1.run(FORWARD); //rotate the motor counterclockwise
  motor2.run(BACKWARD); //rotate the motor counterclockwise
}

void Stop()
{
  motor1.setSpeed(0);
  motor2.run(RELEASE); //turn motor1 off
  motor2.setSpeed(0);
  motor2.run(RELEASE); //turn motor2 off
}
