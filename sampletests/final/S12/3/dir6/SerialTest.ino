// This project is an adaptation of 
// http://www.instructables.com/id/RC-Car-to-Robot/


// Library for the adafruit motor shield
// you can download this from https://github.com/adafruit/Adafruit-Motor-Shield-library
#include <AFMotor.h>


// The RC car has two motors.  The one on the back wheels drives the car
// and the one on the front provides steering.
// Here we attach the two motors to the motor shield
AF_DCMotor motorDrive(1);  // Drive attached to M1
AF_DCMotor motorSteer(2);  // Steer attached to M2






void setup() 
{
    Serial.begin(9600);           // Some serial printout for debugguing
    motorDrive.setSpeed(200);     // Set the motor speed for driving
    motorSteer.setSpeed(255);    // Contol the angle of the steering when turning - 255 is full distance

}



void loop() 
{
   
    if (Serial.read() == 'f')
    {
      motorDrive.run(FORWARD);
    }

    if(Serial.read() == 'b')
    {
      motorDrive.run(BACKWARD);
    }

    if(Serial.read()=='s')
    {
      motorDrive.run(RELEASE);
    }
    
} // end loop



