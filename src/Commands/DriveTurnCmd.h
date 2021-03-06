// RobotBuilder Version: 2.0
//
// This file was generated by RobotBuilder. It contains sections of
// code that are automatically generated and assigned by robotbuilder.
// These sections will be updated in the future when you export to
// C++ from RobotBuilder. Do not put any code or make any change in
// the blocks indicating autogenerated code or it will be lost on an
// update. Deleting the comments indicating the section will prevent
// it from being updated in the future.


#ifndef DRIVETURNCMD_H
#define DRIVETURNCMD_H


#include "Commands/Subsystem.h"
#include "../Robot.h"

/**
 *
 *
 * @author ExampleAuthor
 */
class DriveTurnCmd: public Command {
public:
    // BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=CONSTRUCTOR
	DriveTurnCmd(double lftSpeed, double rtSpeed, double angle, double timeOut, double mode);

    // END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=CONSTRUCTOR

	double distance, speed, timeout, beginstartdist, beginenddist;
	float beginHeading;
	int mode;		// Mode 1=Distance, 2=Contact, 3=Sonar Distance, 4=Timeout
	int turnFlag;	// 1 = straight, 2=Pt.Turn, 3=Swing Turn, 4 Arc Turn
	virtual void Initialize();
	virtual void Execute();
	virtual bool IsFinished();
	virtual void End();
	virtual void Interrupted();

private:
	// BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=VARIABLES

    double m_lftSpeed;
    double m_rtSpeed;
    double m_angle;
    double m_timeOut;
    double m_mode;

    // END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=VARIABLES
};

#endif
