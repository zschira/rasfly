#ifndef _RASFLY_TYPES_H_
#define _RASFLY_TYPES_H_

#include "json.hpp"
#include <string>
#include <array>
#include <Python.h>
#include <vector>

namespace rasfly {

//////////////////////////////////////////////////////////////////////////////////
/// @struct python_type
///
/// @brief Inherit to create python type
//////////////////////////////////////////////////////////////////////////////////
struct python_type {
    PyObject_HEAD;
};

//////////////////////////////////////////////////////////////////////////////////
/// @struct Vec3 
///
/// @brief Vector 3 type used to store most state members
//////////////////////////////////////////////////////////////////////////////////
struct Vec3 : public python_type {
    Vec3(){}
    Vec3(double _x, double _y, double _z) { x = _x; y = _y; z = _z; }
    Vec3(const Vec3& rhs) { x = rhs.x; y = rhs.y; z = rhs.z; }

    // Values
    double x = 0;
    double y = 0;
    double z = 0;

    // Identifier
    const char *name = "vec3";

    //////////////////////////////////////////////////////////////////////////////////
    /// @brief Subtraction operator returns new Vec3
    //////////////////////////////////////////////////////////////////////////////////
    inline friend Vec3 operator-(const Vec3& lhs, const Vec3& rhs) {
        return Vec3(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);
    }

    //////////////////////////////////////////////////////////////////////////////////
    /// @brief Stream operator that allows to easily convert Vec3 to Eigen types
    //////////////////////////////////////////////////////////////////////////////////
    template<class mat>
    inline friend mat& operator<< (mat& lhs, const Vec3& rhs) {
        lhs << rhs.x, rhs.y, rhs.z;
        return lhs;
    }
};

//////////////////////////////////////////////////////////////////////////////////
/// @struct State 
///
/// @brief State struct to store all necessary physical state information
//////////////////////////////////////////////////////////////////////////////////
struct State : public python_type {
    State() {}
    State(const State& rhs) {
        position = rhs.position;
        velocity = rhs.velocity;
        acceleration = rhs.acceleration;
        rotation = rhs.rotation;
        rotation_v = rhs.rotation_v;
        rotation_a = rhs.rotation_a;
        thrust = rhs.thrust;
    }
    // State Vectors
    Vec3 position;
    Vec3 velocity;
    Vec3 acceleration;
    Vec3 rotation;
    Vec3 rotation_v;
    Vec3 rotation_a;
    // Scalar thrust
    double thrust;

    // Identifier
    const char *name = "state";
};

//////////////////////////////////////////////////////////////////////////////////
/// @struct Gains 
///
/// @brief Struct to store gain values. A Gains struct will exist for roll, pitch,
///        and yaw
//////////////////////////////////////////////////////////////////////////////////
struct Gains {
    Gains(const nlohmann::json &json_str, std::string axis) {
        kp = json_str[axis + "_proportional_gain"];
        kd = json_str[axis + "_derivative_gain"];
        ki = json_str[axis + "_integral_gain"];
    }
    double kp;                // Proportional gains
    double kd;                // Derivative gains
    double ki;                // Integral gains
};

//////////////////////////////////////////////////////////////////////////////////
/// @struct MomentsInertia 
///
/// @brief Struct to store moments of inertia. All values are in kg*m^2
//////////////////////////////////////////////////////////////////////////////////
struct MomentsInertia {
    MomentsInertia(const nlohmann::json &json_str) {
        Ix = json_str["Ix"];
        Iy = json_str["Iy"];
        Iz = json_str["Iz"];
    }
    double Ix;    
    double Iy;    
    double Iz;    
};

//////////////////////////////////////////////////////////////////////////////////
/// @struct TorqueContribution 
///
/// @brief Struct to store the torque contributed by each motor on each axis
//////////////////////////////////////////////////////////////////////////////////
struct TorqueContribution {
    TorqueContribution(const nlohmann::json &json_str, const std::string &axis) {
        if(json_str["frame_type"] == "x") {
            if(axis == "X") {
                M1 = 1; M2 = 1; M3 = -1; M4 = -1;
            } else if(axis == "Y") {
                M1 = 1; M2 = -1; M3 = -1; M4 = 1;
            } else if(axis == "Z") {
                M1 = 1; M2 = -1; M3 = 1; M4 = -1;
            }
        }
    }
    // Denotes whether each motor contributes to a torque
    double M1;
    double M2;
    double M3;
    double M4;
};

//////////////////////////////////////////////////////////////////////////////////
/// @struct PhysicalProperties 
///
/// @brief Struct to store physical properties related to copter
//////////////////////////////////////////////////////////////////////////////////
struct PhysicalProperties {
    PhysicalProperties(const nlohmann::json &json_str) : 
    roll(json_str, "roll"), pitch(json_str, "pitch"), 
    yaw(json_str, "yaw"), moments(json_str),
    taux(json_str, "X"), tauy(json_str, "Y"), tauz(json_str, "Z") 
    {
        mass = json_str["mass"];
        motor_radius = json_str["motor_radius"];
        max_thrust = json_str["max_thrust"];
    }
    double mass;                                    // [kg]
    double motor_radius;                            // [m]
    double max_thrust;                                // [N]
    Gains roll;                                        // Dimensionless
    Gains pitch;                                    // Dimensionless
    Gains yaw;                                        // Dimesnionless
    MomentsInertia moments;                            // kd*m^2
    TorqueContribution taux;                        // Dimensionless
    TorqueContribution tauy;                        // Dimensionless
    TorqueContribution tauz;                        // Dimensionless
};

//////////////////////////////////////////////////////////////////////////////////
/// @struct Thrust 
///
/// @brief Struct to store calculated thrust values. Values are stored as a percentage
///        of the maximum thrust
//////////////////////////////////////////////////////////////////////////////////
struct Thrust {
    Thrust(const double _T1, const double _T2, const double _T3, const double _T4) {
        T1 = _T1;
        T2 = _T2;
        T3 = _T3;
        T4 = _T4;
    }
    Thrust(const Thrust& rhs) { T1 = rhs.T1; T2 = rhs.T2; T3 = rhs.T3; T4 = rhs.T4; }
    Thrust() {}
    double T1 = 0;
    double T2 = 0;
    double T3 = 0;
    double T4 = 0;

    // Identifier
    const char *name = "thrust";
};

}

#endif