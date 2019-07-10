#include "rasfly_controller.hpp"
#include <iostream>
#include <Eigen/Dense>
#include <Eigen/QR>

//////////////////////////////////////////////////////////////////////////////////
/// @struct DefaultController
///
/// @brief Implementation of basic flight controller
//////////////////////////////////////////////////////////////////////////////////
struct rasfly::Controller::DefaultController {
    DefaultController(const PhysicalProperties &properties); 
    Thrust_4M CalculateThrust(const State &current, const State &trim);
        
    // Gain vectors
    Eigen::Vector3d Kp;
    Eigen::Vector3d Kd;

    Eigen::Matrix3d moments;

    Eigen::Matrix4d torque_state;
    Eigen::ColPivHouseholderQR<Eigen::Matrix4d> ldlt;

    double motor_radius;
    double max_thrust;
};

//////////////////////////////////////////////////////////////////////////////////
/// @brief Constructor requires a PhysicalProperties struct 
//////////////////////////////////////////////////////////////////////////////////
rasfly::Controller::DefaultController::DefaultController(const PhysicalProperties &properties) {
    // Set gain vectors
    Kp << properties.roll.kp, properties.pitch.kp, properties.yaw.kp;
    Kd << properties.roll.kd, properties.pitch.kd, properties.yaw.kd;
    moments = Eigen::Matrix3d::Zero();
    // Physical properties
    motor_radius = properties.motor_radius;
    max_thrust = properties.max_thrust;
    // Set moment of inertia matrix
    moments(0, 0) = properties.moments.Ix;
    moments(1, 1) = properties.moments.Iy;
    moments(2, 2) = properties.moments.Iz;
    // Set torque contribution matrix
    torque_state <<    properties.taux.M1, properties.taux.M2, properties.taux.M3, properties.taux.M4,
                    properties.tauy.M1, properties.tauy.M2, properties.tauy.M3, properties.tauy.M4,
                    properties.tauz.M1, properties.tauz.M2, properties.tauz.M3, properties.tauz.M4,
                    1,                    1,                    1,                    1;
    // Construct linear solver using LDLT decomposition
    ldlt.compute(torque_state);
}

//////////////////////////////////////////////////////////////////////////////////
/// @brief Function to calculate thrust of each motor
///
/// @param current - The current measured state returned by the imu and run through filter
/// @param trim - The trim state as determined by the
//////////////////////////////////////////////////////////////////////////////////
rasfly::Thrust_4M rasfly::Controller::DefaultController::CalculateThrust(const State &current, const State &trim) {
    // Compute state errors
    Eigen::Vector3d delta_angle, delta_angle_rate, delta_accel;
    delta_angle << current.rotation - trim.rotation;
    delta_angle_rate << current.rotation_v - trim.rotation_v;
    delta_accel << current.acceleration - trim.acceleration;
    // Compute control terms
    Eigen::Vector3d control_func = Kd.array() * delta_angle_rate.array() + Kp.array() * delta_angle.array();
    control_func = -moments * control_func;
    control_func(0) /= motor_radius; control_func(1) /= motor_radius;
    Eigen::Vector4d b(control_func(0), control_func(1), control_func(2), trim.thrust * max_thrust);
    // Solve for current thrusts as a percentage of max thrust
    Eigen::Vector4d thrusts = ldlt.solve(b) / max_thrust;
    rasfly::Thrust_4M thrust(thrusts(0), thrusts(1), thrusts(2), thrusts(3));
    return thrust;
}

//////////////////////////////////////////////////////////////////////////////////
/// @brief Constructor requires json config object
//////////////////////////////////////////////////////////////////////////////////
rasfly::Controller::Controller(nlohmann::json config) : properties(config) {
    pimpl = std::make_unique<DefaultController>(properties);
    calcThrust = [this](State &current, State &trim) {
        return pimpl->CalculateThrust(current, trim);
    };
}

rasfly::Controller::~Controller() { }