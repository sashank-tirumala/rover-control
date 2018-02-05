/*** THIS PIECE OF CODE TAKES VALUES FROM ROS CAMERA AND ROTATES THE CAMERA BASED ON IT ***/

#include <ros.h>
#include <rover_msgs/CameraMotion.h>
#include <Servo.h>
#include <avr/wdt.h> //  Code to restart arduino

Servo mainCameraYaw;
int yaw_initial = 90; // Camera angle
int reset_flag;
ros::NodeHandle nh;
ros::Subscriber<rover_msgs::CameraMotion> cammotion_sub("rover1/camera_dir", &cameraMotionCallback);

void cameraMotionCallback(const rover_msgs::CameraMotion& CameraVelocity)
{
  //nh.loginfo("CameraVelocity.A_button");

  /*** THIS CODE CLAMPS THE CAMERA ANGLE ***/
  if(CameraVelocity.X_button)
  {
      if(yaw_initial > 180)
      {
          yaw_initial = 180;
      }
      else
      {
          yaw_initial = yaw_initial + 2;
      }
   }
  if(CameraVelocity.B_button)
  {
      if(yaw_initial < 0)
      {
        yaw_initial = 0;
      }
      else
      {
        yaw_initial = yaw_initial - 2;
      }
   }
   /*** END ***/

    mainCameraYaw.write(yaw_initial);
}

void setup()
{
  nh.initNode();
  nh.subscribe(cammotion_sub);
  mainCameraYaw.write(yaw_initial);
  mainCameraYaw.attach(A0);

  wdt_disable();
  wdt_enable(WDTO_8S);

}

void loop()
{
  if(reset_flag==1)
  {
     wdt_reset();
  }
  reset_flag=0;
  nh.spinOnce();
  delay(1);
}
