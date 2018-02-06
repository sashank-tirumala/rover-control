/*** This code controls rover movement and gives temperature,presssure, soil temperature and altitude readings constantly back ***/
#include <ros.h>
#include <rover_msgs/WheelVelocity.h>
#include <rover_msgs/SC_task.h>
#include<rover_msgs/digger.h>
#include <Wire.h>
#include <Adafruit_BMP085.h>
#include <DallasTemperature.h>
#include <avr/wdt.h>

/*** DEFINITIONS START HERE ***/
Adafruit_BMP085 bmp; //Pressure Sensor
OneWire oneWire(ONE_WIRE_BUS); // Communication protocol necessary for temperature sensor.
DallasTemperature sensors(&oneWire);
float Celsius =0;
const int HumidSensor= 2;

#define ONE_WIRE_BUS 24
#define dir1 47
#define pwm1 7
#define dir2 49
#define pwm2 6
#define dir3 29
#define pwm3 5
#define dir4 27
#define pwm4 4
#define dir5 25
#define pwm5 3
#define dir6 23
#define pwm6 2
#define drill_pin 48

float left,right;
int sensor_flag;
int reset_flag;

ros::NodeHandle nh;
rover_msgs::SC_task sensor_msg;
ros::Publisher sensor_pub("SC_Task_Sensors", &sensor_msg);
ros::Subscriber<rover_msgs::WheelVelocity> locomotion_sub("rover1/wheel_vel", &roverMotionCallback);
ros::Subscriber<rover_msgs::digger> digger_sub("dig", &drillCallBack);
/*** DEFINITIONS END HERE ***/

void loco(int vel,int dir_pin,int pwm_pin)
{
if(vel<=0)
  {
   digitalWrite(dir_pin,LOW);
   analogWrite(pwm_pin,abs(vel));
  }
else
  {
    digitalWrite(dir_pin,HIGH);
    analogWrite(pwm_pin,abs(vel));
  }


}




void roverMotionCallback(const rover_msgs::WheelVelocity& RoverVelocity){



  left = map(RoverVelocity.left,-70,70,-175,175);
  right = map(RoverVelocity.right,-70,70,-175,175);



   if(RoverVelocity.reset_flag==1)
   {
     reset_flag=1;
   }


  loco(left,dir1,pwm1);
  loco(right,dir2,pwm2);
  loco(left,dir3,pwm3);
  loco(right,dir4,pwm4);
  loco(left,dir5,pwm5);
  loco(right,dir6,pwm6);



 }

 void drillCallBack(const rover_msgs::digger& dig)
 {
     if (dig.drill==1)
    {
      digitalWrite(drill_pin,HIGH);
    }
    else
    {
      digitalWrite(drill_pin,LOW);
    }

  sensor_flag = dig.sensor_flag;

 }

 void setup(){
   nh.initNode();

   nh.subscribe(locomotion_sub);
   nh.subscribe(digger_sub);
   nh.advertise(sensor_pub);

  bmp.begin();
  sensors.begin();

  wdt_disable();
  wdt_enable(WDTO_8S);

   pinMode(dir1,OUTPUT);
   pinMode(dir2,OUTPUT);
   pinMode(dir3,OUTPUT);
   pinMode(dir4,OUTPUT);
   pinMode(dir5,OUTPUT);
   pinMode(dir6,OUTPUT);
   pinMode(pwm1,OUTPUT);
   pinMode(pwm2,OUTPUT);
   pinMode(pwm3,OUTPUT);
   pinMode(pwm4,OUTPUT);
   pinMode(pwm5,OUTPUT);
   pinMode(pwm6,OUTPUT);



 }

 void loop(){


 sensor_msg.Atm_temp=bmp.readTemperature();

 sensor_msg.Atm_press=bmp.readPressure();

 sensor_msg.Soil_humi=analogRead(HumidSensor);

 sensor_msg.Altitude=bmp.readAltitude();

  sensors.requestTemperatures();
  Celsius = sensors.getTempCByIndex(0);

 sensor_msg.Soil_temp=Celsius;

 if(reset_flag==1)  wdt_reset();
 reset_flag=0;
 if(sensor_flag==1)
 {
 sensor_pub.publish( &sensor_msg );
 }
   nh.spinOnce();
   delay(1);
}
