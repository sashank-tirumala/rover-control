/* This piece of code rotates the camera based on the message it recieves from the topic camera_dir */

#include <ros.h>
#include <rover_msgs/CameraMotion.h>
#include <Servo.h> // A library to control servo motors

Servo mainCameraYaw;
int yaw_initial = 90; // Angle of the camera
ros::NodeHandle nh;
void cameraMotionCallback(const rover_msgs::CameraMotion& CameraVelocity)
{
  //nh.loginfo("CameraVelocity.A_button");

  /*** This piece of code clamps the angle of the camera between 0 to 180 degrees ***/
  if(CameraVelocity.X_button)
      if(yaw_initial > 180)
          {
            yaw_initial = 180;
          }
      else
          {
            yaw_initial = yaw_initial + 2;
          }

  if(CameraVelocity.B_button)
      if(yaw_initial < 0)
          {
                  yaw_initial = 0;
          }
      else
          {
            yaw_initial = yaw_initial - 2;
          }
    /*** CODE ENDS HERE ***/

    mainCameraYaw.write(yaw_initial);
}
ros::Subscriber<rover_msgs::CameraMotion> cammotion_sub("rover1/camera_dir", &cameraMotionCallback);
void setup()
{
  nh.initNode();
  nh.subscribe(cammotion_sub);
  mainCameraYaw.write(yaw_initial);
  mainCameraYaw.attach(A0);
}

void loop()
{
  nh.spinOnce();
  delay(1);
}
