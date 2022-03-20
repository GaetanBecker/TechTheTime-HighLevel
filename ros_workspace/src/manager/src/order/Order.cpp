#include "OrderClasse.hpp"
#include "controller/controllerSetup.hpp"
#include "order/order.hpp"
#include "client/ClientT.hpp"
#include <iostream>
#include <string>
#include <cstddef>
#include <stdexcept>
#include "rclcpp/rclcpp.hpp"
#include "order_codes.hpp"
#include "RobotMotion.hpp"
#include <cmath>

// By Gaétan Becker (the doc), jtm tim 30%

Order::Order() {
    auto commClient_MCU = std::make_shared<ActionClient>();
    commClient_MCU->set_shared(commClient_MCU);
    commClient_MCU->wait_for_connection(); 
}

Order::take_statue(){

    RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "===== Begining of the order - Take Statue =====");
    this->move(414,1704); // Move to the key point one look the doc bellow of key point
    this->angle(3*M_PI/4); // To face the support of the "Abri de chantier"

    auto res_move = commClient->send((int64_t) OrderCodes::MOVE, 160, 0, 0); // We move 10mm further than needed
    MotionStatusCodes status_move = static_cast<MotionStatusCodes>(res_move.get()->motion_status);
    if(status_move == MotionStatusCodes::COMPLETE) {
        RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Result: finished with a distance of %lf", 160);
    } else if(status_move == MotionStatusCodes::NOT_COMPLETE){
        RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Motion was not complete");
        return false;
    } else if(status_move == MotionStatusCodes::MOTION_TIMEOUT) {
        RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Motion has been timed out");
        return false;
    }
    auto res_move_arm_down = commClient->send((int64_t) OrderCodes::MOVE_ARM, 0, 13, std::M_PI*(1/2+1/3)); // 0rad of the arm is set at the vertical of the robot
    MotionStatusCodes status_move_arm_down = static_cast<MotionStatusCodes>(res_move_arm_down.get()->motion_status);
    if(status_move_arm_down == MotionStatusCodes::COMPLETE) {
        RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Result: finished with a angle of %lf", std::M_PI*(1/2+1/3));
    } else if(status_move_arm_down == MotionStatusCodes::NOT_COMPLETE){
        RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Motion was not complete");
        return false;
    } else if(status_move_arm_down == MotionStatusCodes::MOTION_TIMEOUT) {
        RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Motion has been timed out");
        return false;
    }
    auto res_pompe_activ = commClient->send((int64_t) OrderCodes::ACTIVATE_PUMP, 0, 13, 0);
    MotionStatusCodes status_pompe_activ = static_cast<MotionStatusCodes>(res_pompe_activ.get()->motion_status);
    if(status_pompe_activ == MotionStatusCodes::COMPLETE) {
        RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Result:  Pump Activated");
    } else if(status_pompe_activ == MotionStatusCodes::NOT_COMPLETE){
        RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Motion was not complete");
        return false;
    } else if(status_pompe_activ == MotionStatusCodes::MOTION_TIMEOUT) {
        RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Motion has been timed out");
        return false;
    }
    auto res_move_arm_up = commClient->send((int64_t) OrderCodes::MOVE_ARM, 0, 13, -std::M_PI*(1/2+1/3)); // 0rad of the arm is set at the vertical of the robot
    MotionStatusCodes status_move_arm_up = static_cast<MotionStatusCodes>(res_move_arm_up.get()->motion_status);
    if(status_move_arm_up == MotionStatusCodes::COMPLETE) {
        RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Result: finished with a angle of %lf", -std::M_PI*(1/2+1/3));
    } else if(status_move_arm_up == MotionStatusCodes::NOT_COMPLETE){
        RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Motion was not complete");
        return false;
    } else if(status_move_arm_up == MotionStatusCodes::MOTION_TIMEOUT) {
        RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Motion has been timed out");
        return false;
    }
    auto res_move_back = commClient->send((int64_t) OrderCodes::MOVE, -160, 0, 0); // We move 10mm further than needed
    MotionStatusCodes status_move_back = static_cast<MotionStatusCodes>(res_move_back.get()->motion_status);
    if(status_move_back == MotionStatusCodes::COMPLETE) {
        RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Result: finished with a distance of %lf", -160);
        RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "===== End of the order - Take Statue =====");
        return true;
    } else if(status_move_back == MotionStatusCodes::NOT_COMPLETE){
        RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Motion was not complete");
        return false;
    } else if(status_move_back == MotionStatusCodes::MOTION_TIMEOUT) {
        RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Motion has been timed out");
        return false;
    }
}

Order::drop_replic(){

    RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "===== Begining of the order - Drop Replic =====");
    this->move(414,1704); // Move to the key point one look the doc bellow of key point
    this->angle(7*M_PI/4); // To face the support of the "Abri de chantier"

    auto res_move = commClient->send((int64_t) OrderCodes::MOVE, -160, 0, 0); // We move 10mm further than needed
    MotionStatusCodes status_move = static_cast<MotionStatusCodes>(res_move.get()->motion_status);
    if(status_move == MotionStatusCodes::COMPLETE) {
        RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Result: finished with a distance of %lf", -160);
    } else if(status_move == MotionStatusCodes::NOT_COMPLETE){
        RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Motion was not complete");
        return false;
    } else if(status_move == MotionStatusCodes::MOTION_TIMEOUT) {
        RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Motion has been timed out");
        return false;
    }
    auto res_move_arm_down = commClient->send((int64_t) OrderCodes::MOVE_ARM, 0, 14, -std::M_PI*(1/2+1/3)); // 0rad of the arm is set at the vertical of the robot
    MotionStatusCodes status_move_arm_down = static_cast<MotionStatusCodes>(res_move_arm_down.get()->motion_status);
    if(status_move_arm_down == MotionStatusCodes::COMPLETE) {
        RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Result: finished with a angle of %lf", -std::M_PI*(1/2+1/3));
    } else if(status_move_arm_down == MotionStatusCodes::NOT_COMPLETE){
        RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Motion was not complete");
        return false;
    } else if(status_move_arm_down == MotionStatusCodes::MOTION_TIMEOUT) {
        RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Motion has been timed out");
        return false;
    }
    auto res_pompe_activ = commClient->send((int64_t) OrderCodes::RELEASE_PUMP, 0, 14, 0);
    MotionStatusCodes status_pompe_activ = static_cast<MotionStatusCodes>(res_pompe_activ.get()->motion_status);
    if(status_pompe_activ == MotionStatusCodes::COMPLETE) {
        RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Result: Pump Desactivated");
    } else if(status_pompe_activ == MotionStatusCodes::NOT_COMPLETE){
        RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Motion was not complete");
        return false;
    } else if(status_pompe_activ == MotionStatusCodes::MOTION_TIMEOUT) {
        RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Motion has been timed out");
        return false;
    }
    auto res_move_arm_up = commClient->send((int64_t) OrderCodes::MOVE_ARM, 0, 13, std::M_PI*(1+1/3)); // 0rad of the arm is set at the vertical of the robot
    MotionStatusCodes status_move_arm_up = static_cast<MotionStatusCodes>(res_move_arm_up.get()->motion_status);
    if(status_move_arm_up == MotionStatusCodes::COMPLETE) {
        RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Result: finished with a angle of %lf", std::M_PI*(1+1/3));
    } else if(status_move_arm_up == MotionStatusCodes::NOT_COMPLETE){
        RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Motion was not complete");
        return false;
    } else if(status_move_arm_up == MotionStatusCodes::MOTION_TIMEOUT) {
        RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Motion has been timed out");
        return false;
    }
    auto res_move_back = commClient->send((int64_t) OrderCodes::MOVE, 160, 0, 0); // We move 10mm further than needed
    MotionStatusCodes status_move_back = static_cast<MotionStatusCodes>(res_move_back.get()->motion_status);
    if(status_move_back == MotionStatusCodes::COMPLETE) {
        RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Result: finished with a distance of %lf", -160);
        RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "===== End of the order - Drop Relic =====");
        return true;
    } else if(status_move_back == MotionStatusCodes::NOT_COMPLETE){
        RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Motion was not complete");
        return false;
    } else if(status_move_back == MotionStatusCodes::MOTION_TIMEOUT) {
        RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Motion has been timed out");
        return false;
    }
}

Order::angle(double angle){

    RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "===== Begin of the order - Angle =====");
    if(angle>std::M_PI){
        auto res_angle = commClient->send((int64_t) OrderCodes::START_ROTATE_LEFT, 0, 0, 2*std::M_PI-angle);
    }
    else{
        auto res_angle = commClient->send((int64_t) OrderCodes::START_ROTATE_RIGHT, 0, 0, angle);
    }
    MotionStatusCodes status_move = static_cast<MotionStatusCodes>(res_angle.get()->motion_status);
    if(status_move == MotionStatusCodes::COMPLETE) {
        RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Result: finished with an angle of %lf", &angle);
        return true;
    } else if(status_move == MotionStatusCodes::NOT_COMPLETE){
        RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Motion was not complete");
        return false;
    } else if(status_move == MotionStatusCodes::MOTION_TIMEOUT) {
        RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Motion has been timed out");
        return false;
    }
    RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "===== End of the order - Angle =====");
}


Order::move(double aim_x,double aim_y) {

    RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "===== Begin of the order - Move =====");
    double curr_x = RobotMotion::x;
    double curr_y = RobotMotion::y;
    double curr_angle = RobotMotion::angle;

    double distance = sqrt((aim_x-curr_x)**2 + (aim_y-curr_y)**2); //Physical distance between too position in staight line
    double aim_angle;
    // Voir Feuille Gaétan
    double cosAIM_ANGLE;
    double x = std::cos(curr_angle);
    double y = std::sin(curr_angle);
    double x_prime = aim_x - curr_x;
    double y_prime = aim_y - curr_y;

    cosAIM_ANGLE = (x*x_prime+y*y_prime)/(x**2+y**2);

    aim_angle = std::acos(cosAIM_ANGLE);

    // Pour savoir si on a vraiment le bonne angle on regarde le sinus

    double sinAIM_ANGLE =(x_prime*y-y_prime*x)/(y**2+x**2);

    if(std::asin(aim_angle)==-sinAIM_ANGLE){
        aim_angle += std::M_PI;
    }
    if(aim_angle>std::M_PI){
        auto res_angle = commClient->send((int64_t) OrderCodes::START_ROTATE_LEFT, 0, 0, 2*std::M_PI-aim_angle);
    }
    else{
        auto res_angle = commClient->send((int64_t) OrderCodes::START_ROTATE_RIGHT, 0, 0, aim_angle); // Possible de laisser que le IF et de mettre le send à l'extérieur
    }
    // Response Treat Rotation
    MotionStatusCodes status_angle = static_cast<MotionStatusCodes>(res_angle.get()->motion_status);
    if(status_angle == MotionStatusCodes::COMPLETE) {
        RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Result: finished with an angle of %lf", &aim_angle); // Faut différencier si on a pri theta ou 2pi-theta
    } else if(status_angle == MotionStatusCodes::NOT_COMPLETE){
        RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Motion was not complete");
        return false; // We can avoid this if we look if we tend to get closer to the target with the move  but without the rotate
    } else if(status_angle == MotionStatusCodes::MOTION_TIMEOUT) {
        RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Motion has been timed out");
        return false; // Same thing
    }

    auto res_move = commClient->send((int64_t) OrderCodes::MOVE, distance, 0, 0);

    MotionStatusCodes status_move = static_cast<MotionStatusCodes>(res_move.get()->motion_status);
    if(status_move == MotionStatusCodes::COMPLETE) {
        RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Result: finished with a distance of %lf", &distance);
        RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "===== End of the order - Move =====");
        return true;
    } else if(status_move == MotionStatusCodes::NOT_COMPLETE){
        RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Motion was not complete");
        return false;
    } else if(status_move == MotionStatusCodes::MOTION_TIMEOUT) {
        RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Motion has been timed out");
        return false;
    }
}