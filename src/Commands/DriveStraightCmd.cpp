// RobotBuilder Version: 2.0
//
// This file was generated by RobotBuilder. It contains sections of
// code that are automatically generated and assigned by robotbuilder.
// These sections will be updated in the future when you export to
// C++ from RobotBuilder. Do not put any code or make any change in
// the blocks indicating autogenerated code or it will be lost on an
// update. Deleting the comments indicating the section will prevent
// it from being updated in the future.


#include "DriveStraightCmd.h"

// BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=CONSTRUCTOR

DriveStraightCmd::DriveStraightCmd(double speed, double distance, double timeout, double mode): Command() {
    m_speed = speed;
    m_distance = distance;
    m_timeout = timeout;
    m_mode = mode;
        // Use requires() here to declare subsystem dependencies
    // eg. requires(chassis);
    // BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=REQUIRES
	Requires(Robot::drivetrain.get());
    // END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=REQUIRES
}

    // END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=CONSTRUCTOR

// Called just before this Command runs the first time
void DriveStraightCmd::Initialize() {
	SetTimeout(m_timeout);
	Robot::drivetrain->ResetEncoders();
	Robot::drivetrain->SetFirstTimeFlag(0);				// set flag to zero for gyro correction
	if (m_distance < 0) m_distance = m_distance * -1;	// Distance is always positive, speed can be negative

	Robot::drivetrain->ZeroBeginEndValues();
	Robot::drivetrain->PostBeginValues();
}

// Called repeatedly when this Command is scheduled to run
void DriveStraightCmd::Execute() {
//	SmartDashboard::PutNumber("Target Speed=", m_speed);
//	SmartDashboard::PutNumber("Target Distance=", m_distance);
	if (m_mode==0) Robot::drivetrain->TankDrive(m_speed, m_speed);
	if (m_mode==1) Robot::drivetrain->Drive(m_speed, 0);

//	Robot::drivetrain->PostCurrValues();
//	Robot::drivetrain->DisplaySmartdashValues();
}

// Make this return true when this Command no longer needs to run execute()
bool DriveStraightCmd::IsFinished() {
	if (IsTimedOut()) return true;	// used in all modes

	if ((m_speed >= 0 ) && (Robot::drivetrain->GetDistance() >= m_distance)) return true;
	if(Robot::drivetrain->GetDistance() <= (m_distance * -1))	return true;

    return false;
}

// Called once after isFinished returns true
void DriveStraightCmd::End() {
	Robot::drivetrain->Stop();							// stop the motors
//	Robot::drivetrain->PostEndValues();
//	Robot::drivetrain->DisplaySmartdashValues();
}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void DriveStraightCmd::Interrupted() {
	End();
}