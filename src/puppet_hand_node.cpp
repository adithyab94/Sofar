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

#include <tf2_ros/transform_listener.h>
#include <baxter_core_msgs/SolvePositionIK.h>
#include <baxter_core_msgs/JointCommand.h>
#include <sensor_msgs/JointState.h>
#include <geometry_msgs/PoseStamped.h>

// You may have a number of globals here.
//...

// Callback functions...

void myFirstCallback(/* Define here the variable which hold the message */){
    // ... Callback function code
}


int main (int argc, char** argv)
{

    //ROS Initialization
    ros::init(argc, argv, "puppet_hand_node");

    // Define your node handles: YOU NEED AT LEAST ONE !
    ros::NodeHandle nh;

    // Read the node parameters if any
    // ...

    // Declare your node's subscriptions and service clients
    // ...

    // Declare you publishers and service servers
     ros::Publisher pub = nh.advertise<baxter_core_msgs::JointCommand>("/robot/limb/left/joint_command", 1);

    //ros::Publisher pub = nh.advertise<baxter_core_msgs::JointCommand>("/my_command", 1);

    ros::ServiceClient ikServiceClient = nh.serviceClient<baxter_core_msgs::SolvePositionIK>("/ExternalTools/left/PositionKinematicsNode/IKService");

    tf2_ros::Buffer tfBuffer;
    tf2_ros::TransformListener listener(tfBuffer);

    ros::Rate rate(10);   // Or other rate.
    while (ros::ok()){
        ros::spinOnce();

        // Your node's code goes here.
        geometry_msgs::TransformStamped transformStamped;

        try{
            transformStamped = tfBuffer.lookupTransform("base", "frame_05",ros::Time(0));

        }
        catch (tf2::TransformException &ex) {
            ROS_WARN("%s",ex.what());
            ros::Duration(1.0).sleep();
            continue;
        }
        //ROS_INFO("received");

        if(!ikServiceClient.isValid()){
            ROS_ERROR("Invalid service.");
            rate.sleep();
            continue;
        }
        if(!ikServiceClient.exists()){
            ROS_ERROR("Service does not exist.");
            rate.sleep();
            continue;
        }

        // Now I am in a position to call the service.
        geometry_msgs::PoseStamped ik_target;
        ik_target.pose.position.x=transformStamped.transform.translation.x;
        ik_target.pose.position.y=transformStamped.transform.translation.y;
        ik_target.pose.position.z=transformStamped.transform.translation.z;

        ik_target.pose.orientation.x=transformStamped.transform.rotation.x;
        ik_target.pose.orientation.y=transformStamped.transform.rotation.y;
        ik_target.pose.orientation.z=transformStamped.transform.rotation.z;
        ik_target.pose.orientation.w=transformStamped.transform.rotation.w;

        ik_target.header.stamp = ros::Time(0);
        ik_target.header.frame_id = "base" ;

        //service request
        baxter_core_msgs::SolvePositionIK service_req;
        service_req.request.seed_mode = service_req.request.SEED_CURRENT ;
        service_req.request.pose_stamp.push_back(ik_target);

        //call service
        if (ikServiceClient.call(service_req)){

            if(service_req.response.result_type[0]==service_req.response.RESULT_INVALID){

                ROS_INFO("No solution to IGM.");
            }
            else {
                sensor_msgs::JointState ik_msg = service_req.response.joints[0];
                baxter_core_msgs::JointCommand left_hand_msg;
                left_hand_msg.mode = left_hand_msg.POSITION_MODE;
                for (int i = 0; i < ik_msg.name.size(); ++i){
                    left_hand_msg.names.push_back(ik_msg.name[i]);
                    left_hand_msg.command.push_back(ik_msg.position[i]);
                }

                //publish command
                pub.publish(left_hand_msg);
            }

        }else {
            ROS_WARN("service request is failed");
        }

    }
}
