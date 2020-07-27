#include "ros/ros.h"
#include "std_msgs/Int32.h"

#include "linux/EApiOs.h"
#include "EApi.h"
#include "semaeapi.h"

#define IN_DIRECTION 1

int main(int argc, char** argv){
    
    ros::init(argc, argv, "button_pub_node");
    ROS_INFO("Start Example: Push the Button");

    ros::NodeHandle nh;

    ros::Publisher pub = nh.advertise<std_msgs::Int32>("button_push", 1000);

    ros::Rate loop_rate(1);

    std_msgs::Int32 msg;

    char addr[16] = "127.0.0.1";
    uint32_t libHandle_;
    uint32_t level = 0;

    // init SEMA
    ROS_INFO("---init SEMA---");
    uint32_t ret = SemaEApiLibInitialize(false, IP_V4, addr, 0, (char *)"123", &libHandle_);
    if (ret != EAPI_STATUS_SUCCESS){
        ROS_INFO("[ERROR] Neuron_GPIO -Can't initialize SEMA Lib. Error code: %X\n", ret);
        }

    // init GPIO
    ROS_INFO("---init GPIO---");
    int _pin = 9;

    // set direciton
    ROS_INFO("---set direction---");
    ret = SemaEApiGPIOSetDirection(libHandle_, EAPI_GPIO_GPIO_ID(_pin), 0x01, IN_DIRECTION);
    if (ret != EAPI_STATUS_SUCCESS){
        ROS_INFO("[ERROR] Neuron_GPIO --- Error setting GPIO direction: 0x%X\n", ret);
        }

    while (ros::ok()){
        
        // get level
        ROS_INFO("Getting level......");
        ret = SemaEApiGPIOGetLevel(libHandle_, EAPI_GPIO_GPIO_ID(_pin),0x01, &level);
        if (ret != EAPI_STATUS_SUCCESS){
            ROS_INFO("[ERROR] Neuron_GPIO - Error getting GPIO level. Error code: %X\n", ret);
            }
        else { ROS_INFO("The input level is %d\n", level);}

        msg.data = level;
        pub.publish(msg);
        ros::spinOnce();
        loop_rate.sleep();
        

        }
    SemaEApiLibUnInitialize(libHandle_);

    return 0;
    }
