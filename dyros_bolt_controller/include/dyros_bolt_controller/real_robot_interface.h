#ifndef REAL_ROBOT_INTERFACE_H
#define REAL_ROBOT_INTERFACE_H

#include <std_msgs/Float64.h>
#include <std_msgs/Bool.h>
#include <unordered_set>
#include "dyros_bolt_controller/odrive_socketcan.h"

#define DEFAULT_CAN_INTERFACE       "can0"
#define DEFAULT_CAN_BITRATE         500000
#define DEFAULT_ENGAGE_ON_STARTUP   false
#define DEFAULT_DISENGAGE_ON_SHUTDOWN   true

#include "control_base.h"

namespace dyros_bolt_controller
{
class RealRobotInterface : public ControlBase
{
public:
    RealRobotInterface(ros::NodeHandle &nh, double Hz);
    virtual ~RealRobotInterface() { }

    virtual void readDevice() override;
    virtual void update() override; // update controller based on readdevice
    virtual void writeDevice() override; // publish to actuate devices
    virtual void wait() override;

    void motorEngageCallback(const std_msgs::Bool::ConstPtr& msg);
    void calibrationCallback(const std_msgs::Bool::ConstPtr& msg);
    
    odrive::ODriveSocketCan odrv;


private:
    ros::Rate rate_;

    ros::Subscriber cl_ctrl_sub;
    ros::Subscriber calibration_sub;

    bool areMotorsReady();
    

};

}
#endif // REAL_ROBOT_INTERFACE_H









