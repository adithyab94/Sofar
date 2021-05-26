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
#include <tf2_ros/transform_broadcaster.h>
#include <tf2/LinearMath/Quaternion.h>
#include <geometry_msgs/TransformStamped.h>

// You may have a number of globals here.
//...

// Callback functions...

void myFirstCallback(/* Define here the variable which hold the message */){
    // ... Callback function code
}


int main (int argc, char** argv)
{

	//ROS Initialization
    ros::init(argc, argv, "tf_broadcaster_05");

    // Define your node handles: YOU NEED AT LEAST ONE !
    ros::NodeHandle nh;

    // Read the node parameters if any
    // ...

    // Declare your node's subscriptions and service clients
    // ...

    // Declare you publishers and service servers
    tf2_ros::TransformBroadcaster pub;


    ros::Rate rate(10);   // Or other rate.
    while (ros::ok()){
        ros::spinOnce();

        // Your node's code goes here.

        geometry_msgs::TransformStamped transformStamped;

        transformStamped.header.stamp = ros::Time::now();
        transformStamped.header.frame_id = "right_gripper";
        transformStamped.child_frame_id = "frame_05";
        transformStamped.transform.translation.x = 0;
        transformStamped.transform.translation.y = 0;
        transformStamped.transform.translation.y = 0.0;

        //SetRPY
        tf2::Quaternion q;
        q.setRPY(0, M_PI, 0);

        transformStamped.transform.rotation.x = q.x();
        transformStamped.transform.rotation.y = q.y();
        transformStamped.transform.rotation.z = q.z();
        transformStamped.transform.rotation.w = q.w();

        pub.sendTransform(transformStamped);

        rate.sleep();
    }
}
