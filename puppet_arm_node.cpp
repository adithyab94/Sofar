/**
 * \file
 * \brief
 * \author
 * \version 0.1
 * \date
 *
 * \param[in]
 *
 * Subscribes to: <BR>
 *    °
 *
 * Publishes to: <BR>
 *    °
 *
 * Description
 *
 */


//Cpp
#include <sstream>
#include <stdio.h>
#include <vector>
#include <iostream>
#include <stdlib.h>
#include <math.h>

//ROS
#include "ros/ros.h"

// Include here the ".h" files corresponding to the topic type you use.
#include <sensor_msgs/JointState.h>
#include <baxter_core_msgs/JointCommand.h>
// You may have a number of globals here.
//...

// Callback functions...

sensor_msgs::JointState last_state ;
bool state_received = false ;

void jointStateCallback(sensor_msgs::JointState jsMsg){
    last_state = jsMsg ;
    if(last_state.name.size() < 10 ) return ;
    state_received = true ;
}


int main (int argc, char** argv)
{

    //ROS Initialization
    ros::init(argc, argv, "Puppet_arm_node");

    // Define your node handles: YOU NEED AT LEAST ONE !

    ros::NodeHandle nh_loc("~"), nh_glob ;

    // Read the node parameters if any


    // Declare your node's subscriptions and service clients
    ros::Subscriber jointstateSub = nh_glob.subscribe<sensor_msgs::JointState>("/robot/joint_states",1,jointStateCallback) ;

    // Declare you publishers and service servers
   ros::Publisher pub = nh_glob.advertise<baxter_core_msgs::JointCommand>("/robot/limb/left/joint_command", 1);
   //ros::Publisher pub = nh_glob.advertise<baxter_core_msgs::JointCommand>("/command", 1);


       ros::Rate rate(10);   // Or other rate.
       while (ros::ok()){
           ros::spinOnce();
           baxter_core_msgs::JointCommand left_joint_command;
           left_joint_command.mode = 2;

           // Your node's code goes here.
           if(!state_received){
               ROS_INFO("Waiting for state...\n") ;
               rate.sleep() ;
               continue ;
           }

           if(left_joint_command.mode == 1) {
                       for (int i = 0; i<7; i++){
                           left_joint_command.names.push_back(last_state.name[i+2]);
                           if (i%2 == 0)
                               left_joint_command.command.push_back(-last_state.position[i+9]);
                           else
                               left_joint_command.command.push_back(last_state.position[i+9]);
                       }
                   }if (left_joint_command.mode == 2){
                       for (int i = 0; i<7; i++){
                           left_joint_command.names.push_back(last_state.name[i+2]);
                           if (i%2 == 0)
                               left_joint_command.command.push_back(-last_state.velocity[i+9]);
                           else
                               left_joint_command.command.push_back(last_state.velocity[i+9]);
                       }
                   }
           pub.publish(left_joint_command) ;
           rate.sleep();
       }
   }
