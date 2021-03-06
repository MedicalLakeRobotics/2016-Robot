// RobotBuilder Version: 2.0
//
// This file was generated by RobotBuilder. It contains sections of
// code that are automatically generated and assigned by robotbuilder.
// These sections will be updated in the future when you export to
// C++ from RobotBuilder. Do not put any code or make any change in
// the blocks indicating autogenerated code or it will be lost on an
// update. Deleting the comments indicating the section will prevent
// it from being updated in the future.


#include "DrivePointTurnCmd.h"

// BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=CONSTRUCTOR

DrivePointTurnCmd::DrivePointTurnCmd(double angle, double speed, double timeout, double mode): Command() {
    m_angle = angle;
    m_speed = speed;
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
void DrivePointTurnCmd::Initialize() {
	Robot::drivetrain->ResetEncoders();
	SetTimeout(m_timeout);
	netTable = NetworkTable::GetTable("MyVisionData");

	if (m_mode == 1){
		if (netTable->GetNumber("AutoAimFlag",0.0) == true){
		// if (Robot::drivetrain->GetAutoAimFlag() == true){
			m_angle = netTable->GetNumber("DeltaX",0.0);
			netTable->PutNumber("DeltaX",0.0);					// clear out so we don't repeat movement

			// m_angle = netTable->GetNumber("FinalShootHdg",0.0) - Robot::drivetrain->GetHeadingByGyro();
			// m_angle =  Robot::drivetrain->GetShooterAutoAimHdg() - Robot::drivetrain->GetHeadingByGyro();
			printf("AutoAim Hdg Begin = %6.2f, Current Hdg = %6.2f \n",m_angle, Robot::drivetrain->GetHeadingByGyro());	// Logged autoaim angle to console
		}
		else{
			m_angle = 0;	// we dont have valid data to auto aim so make it zero
		}
	}
	m_angle = Round(m_angle,2);				// Round to 2 decimal places
}

// Called repeatedly when this Command is scheduled to run
void DrivePointTurnCmd::Execute() {

	if(m_angle<0) Robot::drivetrain->TankDrive(-m_speed,m_speed);
	if(m_angle>=0) Robot::drivetrain->TankDrive(m_speed,-m_speed);
}

// Make this return true when this Command no longer needs to run execute()
bool DrivePointTurnCmd::IsFinished() {
	if ((IsTimedOut()) || (m_angle == 0)) return true;		// zero angle means no motion needed

	LeftEncoderValue = Robot::drivetrain->GetLeftEncoder();
	RightEncoderValue = Robot::drivetrain->GetRightEncoder();

	// could add code here to look how many degrees are left in the turnand vary the speed as needed

	if(((abs(LeftEncoderValue+RightEncoderValue))/2) < ((abs(m_angle))/kEncoderConvPtTurn)) return false;
	if(((abs(LeftEncoderValue+RightEncoderValue))/2) >= ((abs(m_angle))/kEncoderConvPtTurn)) return true;
    return false;	// this is here just so the compiler wont give out an error warning
}

// Called once after isFinished returns true
void DrivePointTurnCmd::End() {
	Robot::drivetrain->Stop();
	if (m_mode == 1){
		printf("AutoAim Hdg End = %6.2f, Current Hdg = %6.2f \n",m_angle, Robot::drivetrain->GetHeadingByGyro());	// Logged autoaim angle to console
	}
}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void DrivePointTurnCmd::Interrupted() {
	End();
}


float DrivePointTurnCmd::Round(float value, int digits){
	int mult = 1;
	if (digits == 0) mult = 1;
	else if (digits == 1) mult = 10;
	else if (digits == 2) mult = 100;
	else if (digits == 3) mult = 1000;
	else if (digits == 4) mult = 10000;
	else if (digits == 5) mult = 100000;
	else if (digits == 6) mult = 1000000;

	return (round(value * mult) / mult);
}

