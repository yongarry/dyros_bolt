#ifndef MUJOCO_INTERFACE_H
#define MUJOCO_INTERFACE_H

#include "control_base.h"
#include <ros/ros.h>
#include <std_msgs/String.h> 
#include <std_msgs/Float32.h>
#include <std_msgs/Int16.h>
#include <sensor_msgs/JointState.h>
#include <mujoco_ros_msgs/SensorState.h>
#include <mujoco_ros_msgs/JointSet.h>

namespace dyros_bolt_controller {

class mujoco_interface : public ControlBase{
public:
  SHMmsgs *tc_shm_;
  int shm_id_;

  mujoco_interface(ros::NodeHandle &nh, double Hz);
  virtual ~mujoco_interface() {deleteSharedMemory(shm_id_, tc_shm_);}// mujocoStop(); }
  
  virtual void update() override; // update controller based on readdevice
  virtual void compute() override; // compute algorithm and update all class object
  virtual void writeDevice() override; // publish to actuate devices
  virtual void wait() override;

private:  // CALLBACK

  void jointStateCallback(const sensor_msgs::JointStateConstPtr& msg);
  void sensorStateCallback(const mujoco_ros_msgs::SensorStateConstPtr& msg);
  void simCommandCallback(const  std_msgs::StringConstPtr& msg);
  void simTimeCallback(const std_msgs::Float32ConstPtr &msg);
  void axisRequestStateCallback(const std_msgs::Int16ConstPtr &msg);

  void simready();
  void torque_control();
  void joint_control();

  //void your_Callback(const sensor_msgs::ImuConstPtr& msg);

private:
  //void mujocoStart();
 // void mujocoStop();


private:
  ros::Publisher mujoco_joint_set_pub_;
  ros::Publisher mujoco_sim_command_pub_;


  ros::Subscriber mujoco_joint_state_sub_;
  ros::Subscriber mujoco_sensor_state_sub_;
  ros::Subscriber mujoco_sim_command_sub_;
  ros::Subscriber mujoco_sim_time_sub_;
  ros::Subscriber axis_request_state_sub;

  mujoco_ros_msgs::JointSet mujoco_joint_set_msg_;

  // Eigen::Vector6d mujoco_virtual_dot_;
  // Eigen::Vector6d mujoco_virtual_;

public:
  std::string ctrl_mode;
  bool simulation_running_;
  bool sim_runnung;
  bool mujoco_ready = false;
  bool mujoco_init_receive = false;
  float mujoco_sim_time;
  float mujoco_sim_last_time;

  std::string joint_name_mj[DyrosBoltModel::MODEL_DOF];
  ros::Rate rate_;
  int dyn_hz;


};

}



#endif // MUJOCO_INTERFACE_H
