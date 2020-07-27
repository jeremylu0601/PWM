#include <ros/ros.h>
#include <std_msgs/Int32.h>

void counterCallback(const std_msgs::Int32::ConstPtr& msg){
    if(msg->data == 0){
        ROS_INFO("User is pushing the button!");
        }
    else if(msg->data == 1){
        ROS_INFO("User does not push the button!");
        }
    }

int main(int argc, char** argv){
    
    ros::init(argc, argv, "button_sub_node");
    ROS_INFO("Start Example: Push the Button");

    ros::NodeHandle nh;

    ros::Subscriber sub = nh.subscribe("button_push",1000,counterCallback);

    ros::spin();

    return 0;
    }
