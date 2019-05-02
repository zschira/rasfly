#include "rasfly_controller.hpp"
#include <Eigen/Dense>
#include <Eigen/QR>

struct rasfly::Controller::DefaultController {
	DefaultController(const PhysicalProperties &properties) {
		Kp << properties.roll.kp, properties.pitch.kp, properties.yaw.kp;
		Kp << properties.roll.kd, properties.pitch.kd, properties.yaw.kd;
		moments = Eigen::Matrix3d::Zero();
		moments(0, 0) = properties.moments.Ix;
		moments(1, 1) = properties.moments.Iy;
		moments(2, 2) = properties.moments.Iz;
		torque_state <<	properties.taux.M1, properties.taux.M2, properties.taux.M3, properties.taux.M4,
						properties.tauy.M1, properties.tauy.M2, properties.tauy.M3, properties.tauy.M4,
						properties.tauz.M1, properties.tauz.M2, properties.tauz.M3, properties.tauz.M4,
						1,					1,					1,					1;
		// Construct linear solver using LDLT decomposition
		ldlt.compute(torque_state);
	}
	// Gain vectors
	Eigen::Vector3d Kp;
	Eigen::Vector3d Kd;

	Eigen::Matrix3d moments;

	Eigen::Matrix4d torque_state;
	Eigen::ColPivHouseholderQR<Eigen::Matrix4d> ldlt;
};

rasfly::Controller::Controller(nlohmann::json config) : properties(config), pimpl(std::make_unique<DefaultController>(properties)) {
	
}

rasfly::Controller::~Controller() { }