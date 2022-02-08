 /*
  * Remember!!
  * this MCU should be connected to Serial
  * To check for input data correction!!
  */

//#include <MsTimer2.h> // use timer interrupt
#include "src\MsTimer2\MsTimer2.h"
#include <Wire.h>
#include "src\TurretI2CModuleData.h"

#define ENA 5
#define IN1 8
#define IN2 9

#define ENCODER 2

int estep = 0;
int estep_read = 0;

float Pv = 3.0; //반응속도
float Iv = 1.0; // 정상상태 오차
float Dv = 1.4; //오버슛 최소화(급격한 변화 최소화)

float err_P = 0;
float err_I = 0;
float err_D = 0;
float err_B = 0;

int goal = 0;
int PID_val;
int real_val = 0;
//float RPM;

bool rpmOK = false;
bool receivedGoal = false;
int tempVal = 0;
bool rpmCheckStart = false;

void setup() 
{
  int val = 6;
  
  PID_motor(val);
  
  Serial.begin(115200);

  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  attachInterrupt(0, encoder, FALLING);

  MsTimer2::set(4, t_intrrupt); //MsTimer2::set(x, t_intrrupt) x 간격으로 보겠다의 의미 -원하는 값 설정... 작아야한다고 생각은 함
  MsTimer2::start();

/*
 * Code From SSB =>
 */
  Wire.begin(TurretI2CModuleData::MUZZLE_ADDRESS);
  Wire.onRequest(sendToMaster);
  Wire.onReceive(receiveFromMaster);
  /*
   * <= Code From SSB
   */
}
/*
 * Code From SSB =>
 */
void sendToMaster()
{
  if(isRpmOK())
  {
    rpmCheckStart = false;
    Wire.write(1);
  }
  else
    Wire.write(0);
}

bool isRpmOK()
{
  return rpmOK;
}

void receiveFromMaster(int nSize)
{
  char* c = reinterpret_cast<char*>(&tempVal);
  int i = 0;
  while(i < nSize)
  {
    *(c + i) = Wire.read();
    i++;
  }

  receivedGoal = true;
}
/* 
 * <= Code From SSB
 */

void loop() 
{
  if(receivedGoal)
  {
    Serial.println("Goal : ");
    Serial.println(tempVal);
    Serial.println("Press Any key");
    while(!Serial.available());
    Serial.read();
  
    goal = tempVal;
    receivedGoal = false;
    rpmCheckStart = true;
  }
}

void PID_motor(int m1)
{
  goal = m1;
}

void motor_controll(int motor_speed)
{
  analogWrite(ENA, motor_speed);
}

void encoder()
{
  estep++;
}

void encoder_read()
{
  estep_read = estep;
  estep = 0;
}
/*
void RPM_value()
{
  RPM = 0.781*estep_read; 이론식 계산 후 대입 - rpm측정기로 확인해보며 대입
}
*/
void pid_control()
{ 
    err_P = estep_read - goal; //오차 --- 반응속도 향상
    err_I = err_I + err_P;  // 오차 누적 --- 정상상태 오차 해결
    err_D = err_B - err_P;  //에러의 변화량 추정 --- 오버슛제거
    err_B = err_P; // 이전 에러값 최신화
    //if(err_I > 1000) err_I = -1000;

    PID_val = ((err_P * Pv) + (err_I * Iv) + (err_D * Dv)); //실제 모터의 넣는값... 직접 바꿔가며 경향성을 봐야함...
    if (PID_val >= 255) PID_val = 255;
    if (PID_val <= -255) PID_val = -255;
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    real_val = abs(255-abs(PID_val));
    motor_controll(real_val);  

/*
 * Code From SSB =>
 */
  if(abs(err_P) < 3 && rpmCheckStart)
    rpmOK = true;
  else
    rpmOK = false;
    
/*
 * <= Code From SSB
 */
}


void t_intrrupt()
{ 
  encoder_read();
  pid_control();
  //RPM_value();
//  Serial.print("estep : ");
//  Serial.println(estep_read);
//  Serial.print("err_P * Pv: ");
//  Serial.println(err_P * Pv);
//  Serial.print("err_I * Iv : ");
//  Serial.println(err_I * Iv);
//  Serial.print("err_D * Dv : ");
//  Serial.println(err_D * Dv);
//  Serial.print("PID value : ");
//  Serial.println(PID_val);
//  Serial.print("Real_value : ");
//  Serial.println(real_val);
//  Serial.println("");
}

//------------Dual BLDC------------//
//
//#include "src\MsTimer2\MsTimer2.h"
//#include <Wire.h>
//#include "src\TurretI2CModuleData.h"
//
//#define l_ENA 5
//#define l_IN1 8
//#define l_IN2 9
//#define r_ENA 6
//#define r_IN1 10
//#define r_IN2 11
//
//#define l_ENCODER 2
//#define r_ENCODER 3
//
//int l_estep = 0;
//int l_estep_read = 0;
//int r_estep = 0;
//int r_estep_read = 0;
//
//float Pv = 3.0; //반응속도 왜 모두 작을수록 잘될까
//float Iv = 1.0; // 정상상태 오차
//float Dv = 1.0; //오버슛 최소화(급격한 변화 최소화)
//
//float l_err_P;
//float l_err_I;
//float l_err_D;
//float l_err_B;
//
//float r_err_P;
//float r_err_I;
//float r_err_D;
//float r_err_B;
//
//int goal = 0;
//
//int l_PID_val;
//int l_real_val;
//float l_RPM;
//int r_PID_val;
//int r_real_val;
//float r_RPM;
//
//bool rpmOK = false;
//bool receivedGoal = false;
//int tempVal = 0;
//bool rpmCheckStart = false;
//
////int val = 0;
//
//void setup() 
//{
//  int val = 6;
//
//  PID_motor(val);
//  
//  Serial.begin(115200);
//
//  pinMode(l_ENA, OUTPUT);
//  pinMode(l_IN1, OUTPUT);
//  pinMode(l_IN2, OUTPUT);
//  pinMode(r_ENA, OUTPUT);
//  pinMode(r_IN1, OUTPUT);
//  pinMode(r_IN2, OUTPUT);
//  
//  attachInterrupt(0, l_encoder, FALLING);
//  attachInterrupt(1, r_encoder, FALLING);
//
//  MsTimer2::set(4, t_intrrupt); //MsTimer2::set(x, t_intrrupt) x 간격으로 보겠다의 의미 -원하는 값 설정... 작아야한다고 생각은 함
//  MsTimer2::start();
//
///*
// * Code From SSB =>
// */
//  Wire.begin(TurretI2CModuleData::MUZZLE_ADDRESS);
//  Wire.onRequest(sendToMaster);
//  Wire.onReceive(receiveFromMaster);
//  /*
//   * <= Code From SSB
//   */
//}
///*
// * Code From SSB =>
// */
//void sendToMaster()
//{
//  if(isRpmOK())
//  {
//    rpmCheckStart = false;
//    Wire.write(1);
//  }
//  else
//    Wire.write(0);
//}
//
//bool isRpmOK()
//{
//  return rpmOK;
//}
//
//void receiveFromMaster(int nSize)
//{
//  char* c = reinterpret_cast<char*>(&tempVal);
//  int i = 0;
//  while(i < nSize)
//  {
//    *(c + i) = Wire.read();
//    i++;
//  }
//
//  receivedGoal = true;
//}
///* 
// * <= Code From SSB
// */
//
//void loop() 
//{
//  if(receivedGoal)
//  {
//    Serial.println("Goal : ");
//    Serial.println(tempVal);
//    Serial.println("Press Any key");
//    while(!Serial.available());
//    Serial.read();
//  
//    goal = tempVal;
//    receivedGoal = false;
//    rpmCheckStart = true;
//  }
//}
//
//void PID_motor(int m1)
//{
//  goal = m1;
//}
//
//void l_motor_controll(int l_motor_speed)
//{
//  analogWrite(l_ENA, l_motor_speed);
//}
//
//void r_motor_controll(int r_motor_speed)
//{
//  analogWrite(r_ENA, r_motor_speed);
//}
//
//void l_encoder()
//{
//  l_estep++;
//}
//
//void r_encoder()
//{
//  r_estep++;
//}
//
//void l_encoder_read()
//{
//  l_estep_read = l_estep;
//  l_estep = 0;
//}
//
//void r_encoder_read()
//{
//  r_estep_read = r_estep;
//  r_estep = 0;
//}
///*
//void RPM_value()
//{
//  RPM = 0.781*estep_read; 이론식 계산 후 대입 - rpm측정기로 확인해보며 대입
//}
//*/
//void pid_control()
//{ 
//  l_err_P = l_estep_read - goal; //오차 --- 반응속도 향상
//  l_err_I = l_err_I + l_err_P;  // 오차 누적 --- 정상상태 오차 해결
//  l_err_D = l_err_B - l_err_P;  //에러의 변화량 추정 --- 오버슛제거
//  l_err_B = l_err_P; // 이전 에러값 최신화
//
//  r_err_P = r_estep_read - goal; //오차 --- 반응속도 향상
//  r_err_I = r_err_I + r_err_P;  // 오차 누적 --- 정상상태 오차 해결
//  r_err_D = r_err_B - r_err_P;  //에러의 변화량 추정 --- 오버슛제거
//  r_err_B = r_err_P; // 이전 에러값 최신화
//
//  l_PID_val = ((l_err_P * Pv) + (l_err_I * Iv) + (l_err_D * Dv));
//  if (l_PID_val >= 255) l_PID_val = 255;
//  if (l_PID_val <= -255) l_PID_val = -255;
//  digitalWrite(l_IN1, LOW);
//  digitalWrite(l_IN2, HIGH);
//  l_real_val = abs(255-abs(l_PID_val));
//
//  r_PID_val = ((r_err_P * Pv) + (r_err_I * Iv) + (r_err_D * Dv));
//  if (r_PID_val >= 255) r_PID_val = 255;
//  if (r_PID_val <= -255) r_PID_val = -255;
//  digitalWrite(r_IN1, HIGH);
//  digitalWrite(r_IN2, LOW);
//  r_real_val = abs(255-abs(r_PID_val));
//  
//  l_motor_controll(l_real_val);
//  r_motor_controll(r_real_val);
//
///*
// * Code From SSB =>
// */
//  if(abs(l_err_P) < 3 && abs(r_err_P) < 3 && rpmCheckStart)
//    rpmOK = true;
//  else
//    rpmOK = false;
//    
///*
// * <= Code From SSB
// */
//}
//
//
//void t_intrrupt()
//{ 
//  l_encoder_read();
//  r_encoder_read();
//  pid_control();
//  //RPM_value();
////  Serial.print("estep : ");
////  Serial.println(estep_read);
////  Serial.print("err_P * Pv: ");
////  Serial.println(err_P * Pv);
////  Serial.print("err_I * Iv : ");
////  Serial.println(err_I * Iv);
////  Serial.print("err_D * Dv : ");
////  Serial.println(err_D * Dv);
////  Serial.print("PID value : ");
////  Serial.println(PID_val);
////  Serial.print("Real_value : ");
////  Serial.println(real_val);
////  Serial.println("");
//}
