
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
        
    ros::init(argc, argv, "pwm_node");
    ROS_INFO("Start Example: pwm");

    ros::NodeHandle nh;

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
        ROS_INFO("[SUCCESS] Start putput voltage");
    }


    // define pwm value
    //************
    int max_val=255;  //255 not stable
    int pwm_val=120;
    //*************
    int free_val=max_val-pwm_val;

    float max_val_f=max_val;
    float pwm_val_f=pwm_val;
    float free_val_f=free_val;

    float d=pwm_val_f/max_val_f;
    
    int max_num=4;
    int num_d=d*max_num;
    ROS_INFO("[SUCCESS] d = %f", d);
    ROS_INFO("[SUCCESS] num_d = %d", num_d);

    while (ros::ok()){
        
        for (int i = 0; i < num_d; i++){
            ret = SemaEApiGPIOSetLevel(libHandle_, EAPI_GPIO_GPIO_ID(_pin), 0x01, LEVEL_HIGH);
            if (ret != EAPI_STATUS_SUCCESS){
            ROS_INFO("[ERROR] Neuron_GPIO - Error setting GPIO level. Error code: %X\n", ret);
            }
            ros::spinOnce();
            loop_rate.sleep();
        }

        for (int i = num_d; i < max_num; i++){
            ret = SemaEApiGPIOSetLevel(libHandle_, EAPI_GPIO_GPIO_ID(_pin), 0x01, LEVEL_LOW);
            if (ret != EAPI_STATUS_SUCCESS){
            ROS_INFO("[ERROR] Neuron_GPIO - Error setting GPIO level. Error code: %X\n", ret);
            }
            ros::spinOnce();
            loop_rate.sleep();
        }

        }
    SemaEApiLibUnInitialize(libHandle_);

    return 0;
    }






