
#include <ros/ros.h>
#include <std_msgs/Int32.h>
#include "linux/EApiOs.h"
#include "EApi.h"
#include "semaeapi.h"

#define OUT_DIRECTION 0
#define LEVEL_LOW     0
#define LEVEL_HIGH    1
#define IN_DIRECTION 1




int main(int argc, char** argv){
    
    ros::init(argc, argv, "zero_one_frequency_test_node");
    ROS_INFO("Start Test: switching between high and low voltage ");

    ros::NodeHandle nh;

    ros::Publisher pub = nh.advertise<std_msgs::Int32>("zero_OR_one", 1000);

    ros::Rate loop_rate(200); //FREQUENCY (hz) //max frequency on ocsillatmeter is about 110 hz

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
    int _pin = 9; //PIN DEFINE 

    // set direciton
    ROS_INFO("---set direction---");
    ret = SemaEApiGPIOSetDirection(libHandle_, EAPI_GPIO_GPIO_ID(_pin), 0x01, OUT_DIRECTION);
    if (ret != EAPI_STATUS_SUCCESS){
        ROS_INFO("[ERROR] Neuron_GPIO --- Error setting GPIO direction: 0x%X\n", ret);
        }
    else{
        ROS_INFO("[SUCCESS] Start loop");
    }
    while (ros::ok()){
        

        if (level==0){
            ret = SemaEApiGPIOSetLevel(libHandle_, EAPI_GPIO_GPIO_ID(_pin), 0x01, LEVEL_LOW);
            if (ret != EAPI_STATUS_SUCCESS){
            ROS_INFO("[ERROR] Neuron_GPIO - Error setting GPIO level. Error code: %X\n", ret);
            }
            msg.data = level;
            pub.publish(msg);
            level=1;
            ros::spinOnce();
        }

        else if (level==1){
            ret = SemaEApiGPIOSetLevel(libHandle_, EAPI_GPIO_GPIO_ID(_pin), 0x01, LEVEL_HIGH);
            if (ret != EAPI_STATUS_SUCCESS){
            ROS_INFO("[ERROR] Neuron_GPIO - Error setting GPIO level. Error code: %X\n", ret);
            }
            msg.data = level;
            pub.publish(msg);
            level=0;
            ros::spinOnce();
        }

        else{
            ROS_INFO("[ERROR] level is neither 0 nor 1");
            break;
        }




        loop_rate.sleep();
        

        }
    SemaEApiLibUnInitialize(libHandle_);

    return 0;
    }











