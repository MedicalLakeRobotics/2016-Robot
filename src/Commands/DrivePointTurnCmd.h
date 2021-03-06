// RobotBuilder Version: 2.0
//
// This file was generated by RobotBuilder. It contains sections of
// code that are automatically generated and assigned by robotbuilder.
// These sections will be updated in the future when you export to
// C++ from RobotBuilder. Do not put any code or make any change in
// the blocks indicating autogenerated code or it will be lost on an
// update. Deleting the comments indicating the section will prevent
// it from being updated in the future.


#ifndef DRIVEPOINTTURNCMD_H
#define DRIVEPOINTTURNCMD_H


#include "Commands/Subsystem.h"
#include "../Robot.h"

/**
 *
 *
 * @author ExampleAuthor
 */
class DrivePointTurnCmd: public Command {
public:
    // BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=CONSTRUCTOR
	DrivePointTurnCmd(double angle, double speed, double timeout, double mode);

    // END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=CONSTRUCTOR

	virtual void Initialize();
	virtual void Execute();
	virtual bool IsFinished();
	virtual void End();
	virtual void Interrupted();

	float Round(float value, int digits);

private:
	// BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=VARIABLES

    double m_angle;
    double m_speed;
    double m_timeout;
    double m_mode;

    // END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=VARIABLES
	std::shared_ptr<NetworkTable> netTable;							// NetTables to talk to RoboRealm
	std::vector<std::string> keys;

	const float kEncoderConvPtTurn = 0.02978;			// avg degrees rotation per encoder cnt. Average lf & rt together
	int LeftEncoderValue;
	int RightEncoderValue;
	const float kFastTurnSpeed = 0.8;
	const float kMedTurnSpeed = 0.5;
	const float kSlowTurnSpeed = 0.2;
};

#endif
