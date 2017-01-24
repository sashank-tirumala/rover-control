
(cl:in-package :asdf)

(defsystem "rover_msgs-msg"
  :depends-on (:roslisp-msg-protocol :roslisp-utils )
  :components ((:file "_package")
    (:file "GripperMotion" :depends-on ("_package_GripperMotion"))
    (:file "_package_GripperMotion" :depends-on ("_package"))
    (:file "ArmAngle" :depends-on ("_package_ArmAngle"))
    (:file "_package_ArmAngle" :depends-on ("_package"))
    (:file "CameraMotion" :depends-on ("_package_CameraMotion"))
    (:file "_package_CameraMotion" :depends-on ("_package"))
    (:file "WheelVelocity" :depends-on ("_package_WheelVelocity"))
    (:file "_package_WheelVelocity" :depends-on ("_package"))
  ))