
#include <ros/ros.h>
#include <std_msgs/Int32.h>
#include "linux/EApiOs.h"
#include "EApi.h"
#include "semaeapi.h"

#define OUT_DIRECTION 0
#define LEVEL_LOW     0
#define LEVEL_HIGH    1



void counterCallback(const std_msgs::Int32::ConstPtr& msg)
{
    ROS_INFO("---init SEMA---");
    char addr[16] = "127.0.0.1";
    uint32_t libHandle_;
    uint32_t ret = SemaEApiLibInitialize(false, IP_V4, addr, 0, (char *)"123", &libHandle_);
    if (ret != EAPI_STATUS_SUCCESS){
        ROS_INFO("[ERROR] Neuron_GPIO -Can't initialize SEMA Lib. Error code: %X\n", ret);
        }

    // init GPIO
    int _pin = 7;

    // set direciton
    ret = SemaEApiGPIOSetDirection(libHandle_, EAPI_GPIO_GPIO_ID(_pin), 0x01, OUT_DIRECTION);
    if (ret != EAPI_STATUS_SUCCESS){
        ROS_INFO("[ERROR] Neuron_GPIO --- Error setting GPIO direction: 0x%X\n", ret);
        }


    if(msg->data == 1){
        ROS_INFO("LED ON");
        // Set level low
        ret = SemaEApiGPIOSetLevel(libHandle_, EAPI_GPIO_GPIO_ID(_pin), 0x01, LEVEL_HIGH);
        if (ret != EAPI_STATUS_SUCCESS){
            ROS_INFO("[ERROR] Neuron_GPIO --- Error setting GPIO level: 0x%X\n", ret);
           }
        }

    else if(msg->data == 0){
        ROS_INFO("LED OFF");
        // Set level high
        ret = SemaEApiGPIOSetLevel(libHandle_, EAPI_GPIO_GPIO_ID(_pin), 0x01, LEVEL_LOW);
        if (ret != EAPI_STATUS_SUCCESS){
            ROS_INFO("[ERROR] Neuron_GPIO --- Error setting GPIO level: 0x%X\n", ret);
           }
        }
    
    else{
        ROS_INFO("Please enter 1 for LED ON or 0 for LED OFF");}

    SemaEApiLibUnInitialize(libHandle_);
}



int main(int argc, char** argv){
    
    ros::init(argc, argv, "led_sub_node");
    ROS_INFO("Start Example: LED Blink");
    
    ros::NodeHandle nh;

    ros::Subscriber sub = nh.subscribe("led_blink",100,counterCallback);

    ros::spin();

    return 0;
}

