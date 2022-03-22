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
// Numerotation of XL :
// ON FLOOR
// 2nd FLOOR : 4 3 2
// 1st FLOOR : 5 6 1
// ON ARM :
// 2nd RAW : 10 9 8
// 1st RAW : 11 12 7

Order::Order() {
    commClient = std::make_shared<ActionClient>();
    commClient->set_shared(commClient);
    commClient->wait_for_connection(); 
}

Oder::take_distrib_vertical(int id) { // Dépends du côté où est #IFDEF #ENDIF POUR LE MOMENT C EST POUR LE COTE JAUNE
    double KP_x;
    double KP_y;

    double distance_palet_one; // TO DEF
    double distance_palet_two; // TO DEF
    double distance_palet_three; // TO DEF

    double angle_arm_floor; // TO DEF
    double angle_arm_arm; // TO DEF

    RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "===== Begining of the order - Take Distributor %d Vertical =====", id);
    // We choose the KP (middle of the distrib) on the depend of the distrib
    // ORDER PALET : RED - GREEN - BLUE : From the air of playground to the border
    switch (id)
    {
    case 1:
        KP_x = 144.29+RADIUS_BASE_1A+10;
        KP_y = 1250-115;
        break;
    case 2:
        KP_x = 1350+115;
        KP_y = 144.29+RADIUS_BASE_1A+10;
        break;
    case 3:
        KP_x = 1650+115;
        KP_y = 144.29+RADIUS_BASE_1A+10;
        break;
    default:
        return false;
    }

    this->move(KP_x,KP_y);
    if(id==1){
        this->angleABS(std::M_PI);
    }
    else{
        this->angleABS(-std::M_PI);
    }

    auto res_move_one = commClient->send((int64_t) OrderCodes::MOVE, RADIUS_BASE_1A-distance_palet_one, 0, 0); 
    MotionStatusCodes status_move_one = static_cast<MotionStatusCodes>(res_move_one.get()->motion_status);
    if(status_move_one == MotionStatusCodes::COMPLETE) {
        RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Result: finished with a distance of %lf", RADIUS_BASE_1A-distance_palet_one);
    } else if(status_move_one == MotionStatusCodes::NOT_COMPLETE){
        RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Motion was not complete");
        return false;
    } else if(status_move_one == MotionStatusCodes::MOTION_TIMEOUT) {
        RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Motion has been timed out");
        return false;
    }
    auto res_move_arm_floor_one = commClient->send((int64_t) OrderCodes::MOVE_ARM, 0, 1, angle_arm_floor);
    MotionStatusCodes status_move_arm_floor_one = static_cast<MotionStatusCodes>(res_move_arm_floor_one.get()->motion_status);
    if(status_move_arm_floor_one == MotionStatusCodes::COMPLETE) {
        RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Result: finished with a angle of %lf", angle_arm_floor);
    } else if(status_move_arm_floor_one == MotionStatusCodes::NOT_COMPLETE){
        RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Motion was not complete");
        return false;
    } else if(status_move_arm_floor_one == MotionStatusCodes::MOTION_TIMEOUT) {
        RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Motion has been timed out");
        return false;
    }
    auto res_move_arm_arm_one = commClient->send((int64_t) OrderCodes::MOVE_ARM, 0, 1+6, angle_arm_arm);
    MotionStatusCodes status_move_arm_arm_one = static_cast<MotionStatusCodes>(res_move_arm_arm_one.get()->motion_status);
    if(status_move_arm_arm_one == MotionStatusCodes::COMPLETE) {
        RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Result: finished with a angle of %lf", angle_arm_arm);
    } else if(status_move_arm_arm_one == MotionStatusCodes::NOT_COMPLETE){
        RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Motion was not complete");
        return false;
    } else if(status_move_arm_arm_one == MotionStatusCodes::MOTION_TIMEOUT) {
        RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Motion has been timed out");
        return false;
    }

    auto res_move_two = commClient->send((int64_t) OrderCodes::MOVE, distance_palet_one-distance_palet_two, 0, 0); 
    MotionStatusCodes status_move_two = static_cast<MotionStatusCodes>(res_move_two.get()->motion_status);
    if(status_move_two == MotionStatusCodes::COMPLETE) {
        RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Result: finished with a distance of %lf", distance_palet_one-distance_palet_two);
    } else if(status_move_two == MotionStatusCodes::NOT_COMPLETE){
        RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Motion was not complete");
        return false;
    } else if(status_move_two == MotionStatusCodes::MOTION_TIMEOUT) {
        RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Motion has been timed out");
        return false;
    }
    auto res_move_arm_floor_two = commClient->send((int64_t) OrderCodes::MOVE_ARM, 0, 6, angle_arm_arm); 
    MotionStatusCodes status_move_arm_floor_two = static_cast<MotionStatusCodes>(res_move_arm_floor_two.get()->motion_status);
    if(status_move_arm_floor_two == MotionStatusCodes::COMPLETE) {
        RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Result: finished with a angle of %lf", angle_arm_floor);
    } else if(status_move_arm_floor_two == MotionStatusCodes::NOT_COMPLETE){
        RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Motion was not complete");
        return false;
    } else if(status_move_arm_floor_two == MotionStatusCodes::MOTION_TIMEOUT) {
        RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Motion has been timed out");
        return false;
    }
    auto res_move_arm_arm_two = commClient->send((int64_t) OrderCodes::MOVE_ARM, 0, 6+6, angle_arm_arm); 
    MotionStatusCodes status_move_arm_arm_two = static_cast<MotionStatusCodes>(res_move_arm_arm_two.get()->motion_status);
    if(status_move_arm_arm_two == MotionStatusCodes::COMPLETE) {
        RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Result: finished with a angle of %lf", angle_arm_arm);
    } else if(status_move_arm_arm_two == MotionStatusCodes::NOT_COMPLETE){
        RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Motion was not complete");
        return false;
    } else if(status_move_arm_arm_two == MotionStatusCodes::MOTION_TIMEOUT) {
        RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Motion has been timed out");
        return false;
    }

    auto res_move_three = commClient->send((int64_t) OrderCodes::MOVE, distance_palet_two-distance_palet_three, 0, 0);
    MotionStatusCodes status_move_three = static_cast<MotionStatusCodes>(res_move_three.get()->motion_status);
    if(status_move_three == MotionStatusCodes::COMPLETE) {
        RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Result: finished with a distance of %lf", distance_palet_three-distance_palet_one);
    } else if(status_move_three == MotionStatusCodes::NOT_COMPLETE){
        RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Motion was not complete");
        return false;
    } else if(status_move_three == MotionStatusCodes::MOTION_TIMEOUT) {
        RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Motion has been timed out");
        return false;
    }
    auto res_move_arm_floor_three = commClient->send((int64_t) OrderCodes::MOVE_ARM, 0, 5, angle_arm_arm);
    MotionStatusCodes status_move_arm_floor_three = static_cast<MotionStatusCodes>(res_move_arm_floor_three.get()->motion_status);
    if(status_move_arm_floor_three == MotionStatusCodes::COMPLETE) {
        RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Result: finished with a angle of %lf", angle_arm_floor);
    } else if(status_move_arm_floor_three == MotionStatusCodes::NOT_COMPLETE){
        RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Motion was not complete");
        return false;
    } else if(status_move_arm_floor_three == MotionStatusCodes::MOTION_TIMEOUT) {
        RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Motion has been timed out");
        return false;
    }
    auto res_move_arm_arm_three = commClient->send((int64_t) OrderCodes::MOVE_ARM, 0, 5+6, angle_arm_arm);
    MotionStatusCodes status_move_arm_arm_three = static_cast<MotionStatusCodes>(res_move_arm_arm_three.get()->motion_status);
    if(status_move_arm_arm_three == MotionStatusCodes::COMPLETE) {
        RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Result: finished with a angle of %lf", angle_arm_arm);
    } else if(status_move_arm_arm_three == MotionStatusCodes::NOT_COMPLETE){
        RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Motion was not complete");
        return false;
    } else if(status_move_arm_arm_three == MotionStatusCodes::MOTION_TIMEOUT) {
        RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Motion has been timed out");
        return false;
    }
    auto res_move_end = commClient->send((int64_t) OrderCodes::MOVE, distance_palet_three-distance_palet_one, 0, 0);
    MotionStatusCodes status_move_end = static_cast<MotionStatusCodes>(res_move_end.get()->motion_status);
    if(status_move_end == MotionStatusCodes::COMPLETE) {
        RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Result: finished with a distance of %lf", distance_palet_three-distance_palet_one);
        return true;
    } else if(status_move_end == MotionStatusCodes::NOT_COMPLETE){
        RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Motion was not complete");
        return false;
    } else if(status_move_end == MotionStatusCodes::MOTION_TIMEOUT) {
        RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Motion has been timed out");
        return false;
    }

}

Order::take_statue(){

    RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "===== Begining of the order - Take Statue =====");
    this->move(421.43,1700); // Move to the key point one look the doc bellow of key point 
    // NOTE THAT WE HAVE 1,6cm OF MARGE FOR THE ROTATION / WE MIGHT BE GO HIGHER
    this->angleABS(3*M_PI/4); // To face the support of the "Abri de chantier"

    auto res_move = commClient->send((int64_t) OrderCodes::MOVE, 170-HAFL_LENGHT_2A, 0, 0); // We move 10mm further than needed
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
    auto res_move_back = commClient->send((int64_t) OrderCodes::MOVE, HAFL_LENGHT_2A-170, 0, 0); // We move 10mm further than needed
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
    this->moveABS(421.43,1704); // Move to the key point one look the doc bellow of key point
    this->angleABS(7*M_PI/4); // To face the support of the "Abri de chantier"

    auto res_move = commClient->send((int64_t) OrderCodes::MOVE, HAFL_LENGHT_2A-170, 0, 0); // We move 10mm further than needed
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
    auto res_move_back = commClient->send((int64_t) OrderCodes::MOVE, 170-HAFL_LENGHT_2A, 0, 0); // We move 10mm further than needed
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

Order::angleABS(double angle_rel){
    double angle = -RobotMotion::angle+angle_rel;
    RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "===== Begin of the order - Angle ABS =====");
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
    RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "===== End of the order - Angle ABS =====");
}

Order::moveABS(double distance_rel){
    RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "===== Begin of the order - Distance ABS =====");
    auto res = commClient->send((int64_t) OrderCodes::MOVE, distance_rel, 0, 0);
    MotionStatusCodes status = static_cast<MotionStatusCodes>(res.get()->motion_status);
    if(status == MotionStatusCodes::COMPLETE) {
        RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Result: finished with an angle of %lf", distance_rel);
        return true;
    } else if(status == MotionStatusCodes::NOT_COMPLETE){
        RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Motion was not complete");
        return false;
    } else if(status == MotionStatusCodes::MOTION_TIMEOUT) {
        RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Motion has been timed out");
        return false;
    }
    RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "===== End of the order - Distance ABS =====");
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