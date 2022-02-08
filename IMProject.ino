/*
  #include "src\hardware\ObserverControlSystem.h"
  #include "src\utility\MemoryFree.h"

  ObserverControlSystem* ocs = NULL;

  void setup()
  {
  ocs = new ObserverControlSystem();
  }

  void loop()
  {
  ocs->run();

  Serial.begin(115200);
  Serial.print("freeMemory : ");
  Serial.println(freeMemory());
  Serial.end();

  delay(1000);
  }
*/
/*
#include "src\hardware\TurretControlSystem.h"
#include "src\utility\MemoryFree.h"

TurretControlSystem* tcs = NULL;

void setup()
{
  tcs = new TurretControlSystem();
}

void loop()
{
  tcs->run();

  Serial.begin(115200);
  Serial.print("freeMemory : ");
  Serial.println(freeMemory());

  delay(1000);
}
*/
/*
#include "src\hardware\ServerControlSystem.h"
#include "src\utility\MemoryFree.h"

ServerControlSystem* scs = NULL;

void setup()
{
  scs = new ServerControlSystem();
}

void loop()
{
  scs->run();

  Serial.begin(115200);
  Serial.print("freeMemory : ");
  Serial.println(freeMemory());

  delay(1000);
}
*/
//====================================================================================//
/*
  #include "src\imLibrary\StepMotorDriver.h"

  StepMotorDriver* s = NULL;
:
  void setup()
  {
    Serial.begin(115200);
  s = new StepMotorDriver(200, 77, 8, 9, 10);
  s->setRotatePerMinute(3);
  }

  void loop()
  {
  if(Serial.available())
  {
    float angle = Serial.parseFloat();
    s->setAngle(angle);
    Serial.println(s->getAngle());
  }
  }
*/
/*
#include "src\imLibrary\Bluetooth.h"

Bluetooth* s = NULL;
//Bluetooth* r = NULL;

void setup()
{
  Serial.begin(9600);
  s = new Bluetooth(50, 51, 30);
//  r = new Bluetooth(52, 53, 31);
}

unsigned long ptime = 0;

void loop()
{
  if(Serial.available())
    s->write(Serial.read());

  if(s->available())
    Serial.write(s->read());
    
  if(millis() - ptime > 1000 && s->isConnected())
  {
    Serial.println("connected");
    ptime = millis();
  }

//  if(Serial.available())
//    r->write(Serial.read());
//
//  if(r->available())
//    Serial.write(r->read());
//
//  if(millis() - ptime > 1000 && r->isConnected())
//  {
//    Serial.println("connected");
//    ptime = millis();
//  }
}
*/
/*
#include "src\imLibrary\Encoder.h"

Encoder e(18, 40, 6000);

void setup()
{
  Serial.begin(115200);
}

void loop()
{
  if(Serial.available())
  {
    Serial.read();
    Serial.println(e.getAngle());
  }
}
*/
/*
#include "src\imLibrary\AngleMotor.h"
#include "src\imLibrary\StepMotorDriver.h"
#include "src\imLibrary\Encoder.h"

StepMotorDriver* motor = NULL;
Encoder* encoder = NULL;

void setup()
{
  Serial.begin(115200);
  motor = new StepMotorDriver(200, 77, 8, 9, 10);
  motor->setRotatePerMinute(3);
  encoder = new Encoder(20, 21, 3000L);
  
//  motor = new StepMotorDriver(200, 1, 3, 4, 5);
//  motor->setRotatePerMinute(10);
//  encoder = new Encoder(18, 19, 2500);
}

void loop()
{
  int angle = 0;
  while(!Serial.available());
    angle = Serial.parseInt();
    
  int deltaAngle = 0;
  do
  {
    Serial.println("Angle");
    Serial.println(angle);
    motor->setAngle(angle);
    Serial.println("Encoder Angle");
    Serial.println(encoder->getAngle());
    Serial.println("Current Angle");
    Serial.println(motor->getAngle());
    
    motor->changeCurrentAngle(encoder->getAngle());
    Serial.println("Changed Angle");
    Serial.println(motor->getAngle());

    deltaAngle = deltaAngleCheck(angle, motor->getAngle());
    Serial.println("delta Angle");
    Serial.println(deltaAngle);
  } while(abs(deltaAngle) > 1);
}

int deltaAngleCheck(int aim, int current)
{
  int result = 0;
  if(aim > current)
    result = aim - current;
  else
    result = current - aim;

    if(result < 0)
      result += 360;
      
    if(result >= 360)
      result -= 360;

    if(result > 180)
      result -= 360;

  return result;
}
*/

#include "src\imLibrary\LinearMotor.h"

LinearMotor* l = NULL;

void setup()
{
  l = new LinearMotor(3);
}

void loop()
{
  l->setLength(100);
}
