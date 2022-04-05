#include "MotionPublisher.hpp"
#include <chrono>
#include "rclcpp/rclcpp.hpp"
#include "../serial/SerialPort.hpp"
#include "motion_msg_srv/msg/motion.hpp"
#include <const_shared/CommunicationConst.hpp>
#include <const_shared/MotionConst.hpp>
#include <cmath>
#include <action_msg_srv_shared/order_codes.hpp>
#include <order/motion.h>
#include "../bit_decoder/bit_decoder.hpp"


MotionPublisher::MotionPublisher(std::shared_ptr<scom::SerialPort> gateway) : 
    microcontroller_gateway(gateway), Node("motion_publisher") {
        publisher_ = this->create_publisher<motion_msg_srv::msg::Motion>("motion_topic", 10);
        motion_status = MotionStatusCodes::NOT_COMPLETE;
    }

int64_t MotionPublisher::get_motion_status() const {
    return (int64_t) this->motion_status;
}


void MotionPublisher::broadcast_motion(int32_t expected_left_ticks, int32_t expected_right_ticks) {

    int32_t left_ticks = 0, right_ticks = 0;

    int32_t left_ticks_mult = 1;
    int32_t right_ticks_mult = 1;

    if(expected_left_ticks < 0) {
        expected_left_ticks = -expected_left_ticks;
        left_ticks_mult = -1;
    }
    if(expected_right_ticks < 0) {
        expected_right_ticks = -expected_right_ticks;
        right_ticks_mult = -1;
    }

    int32_t previous_left_ticks = MOTION_CRITERIA, previous_right_ticks = MOTION_CRITERIA;

    auto start = std::chrono::system_clock::now();
    bool read_error = false;
    do {

            read_error = false;

            microcontroller_gateway->call_remote_function<Get_Ticks>();
            std::this_thread::sleep_for(READ_FEEDBACK_DELAY);
            auto value = microcontroller_gateway->receive_feedback<Get_Ticks>();
            
            bit_encoder::values<Get_Ticks, int32_t> decoded_values{};
            decoded_values.decoder.decode(value);

            if(decoded_values.decoder.decoded.at(0) < 65536 && 
                decoded_values.decoder.decoded.at(0) > -5000 && 
                decoded_values.decoder.decoded.at(1) < 65536 &&
                decoded_values.decoder.decoded.at(1) > -5000) 
            {

                    previous_left_ticks = left_ticks;
                    previous_right_ticks = right_ticks;

                    left_ticks = decoded_values.decoder.decoded.at(0);
                    right_ticks = decoded_values.decoder.decoded.at(1);
            } else {
                read_error = true;
            }

            auto now = std::chrono::system_clock::now();
            auto interval = std::chrono::duration_cast<std::chrono::milliseconds>(now - start);
            if(interval.count() >= TIMEOUT) {
                RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Timed out %d\n", interval.count());
                this->motion_status = MotionStatusCodes::MOTION_TIMEOUT;
                microcontroller_gateway->call_remote_function<Motion_Set_Forward_Translation_Setpoint, Shared_Tick>(0);
                return;
            }
            
            auto msg = motion_msg_srv::msg::Motion();
            msg.left_ticks = read_error ? 0 : left_ticks_mult * (left_ticks - previous_left_ticks);
            msg.right_ticks = read_error ? 0 : right_ticks_mult * (right_ticks - previous_right_ticks);
            RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Publishing: %d %d\n", msg.left_ticks, msg.right_ticks);
            this->publisher_->publish(msg);
            std::this_thread::sleep_for(MOTION_BROADCAST_PERIOD);

        } while ( abs(left_ticks - previous_left_ticks) >= MOTION_CRITERIA ||
                    abs(right_ticks - previous_right_ticks) >= MOTION_CRITERIA);

    if(abs(left_ticks - expected_left_ticks) >= TICKS_INCERTITUDE || (right_ticks - expected_right_ticks) >= TICKS_INCERTITUDE) {
        this->motion_status = MotionStatusCodes::NOT_COMPLETE;
    } else {
        this->motion_status = MotionStatusCodes::COMPLETE;
    }

    microcontroller_gateway->call_remote_function<Motion_Set_Forward_Translation_Setpoint, Shared_Tick>(0);
}
