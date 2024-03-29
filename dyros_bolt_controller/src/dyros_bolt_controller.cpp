#include <ros/ros.h>
#include "dyros_bolt_controller/mujoco_interface.h"
#include "dyros_bolt_controller/real_robot_interface.h"
using namespace dyros_bolt_controller;

#include <math.h>

// volatile bool *prog_shutdown;

// void SIGINT_handler(int sig)
// {
//     cout << " CNTRL : shutdown Signal" << endl;
//     *prog_shutdown = true;
// }

int main(int argc, char **argv)
{
    ros::init(argc, argv, "dyros_bolt_controller");
    ros::NodeHandle nh("~");

    std::string mode;
    nh.param<std::string>("run_mode", mode, "simulation");
    ControlBase *ctr_obj;
    
    // int shm_id_;
    // init_shm(shm_msg_key, shm_id_, &ctr_obj->tc_shm_);
    // prog_shutdown = &ctr_obj->tc_shm_->shutdown;

    double Hz;
    nh.param<double>("control_frequency", Hz, 150.0);

    if(mode == "simulation")
    {
        ROS_INFO("DYROS BOLT MAIN CONTROLLER - !!! MUJOCO SIMULATION MODE !!!");
        ctr_obj = new mujoco_interface(nh, Hz);
    }
    else if(mode == "real_robot")
    {
        ROS_INFO("DYROS BOLT MAIN CONTROLLER - !!! REAL ROBOT MODE !!!");
        ctr_obj = new RealRobotInterface(nh, Hz);
    }
    else
    {
        ROS_FATAL("Please choose simulation or real_robot");
    }

    while(ros::ok())
    {
        ctr_obj->readDevice();
        ctr_obj->update();
        ctr_obj->compute();
        ctr_obj->reflect();
        ctr_obj->writeDevice();
        ctr_obj->wait();

        if(ctr_obj->isShuttingDown())
        {
          break;
        }
    }
    delete ctr_obj;

    return 0;
}
