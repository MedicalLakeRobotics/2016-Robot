// RobotBuilder Version: 2.0
//
// This file was generated by RobotBuilder. It contains sections of
// code that are automatically generated and assigned by robotbuilder.
// These sections will be updated in the future when you export to
// C++ from RobotBuilder. Do not put any code or make any change in
// the blocks indicating autogenerated code or it will be lost on an
// update. Deleting the comments indicating the section will prevent
// it from being updated in the future.

#include "LiftSys.h"
#include "../RobotMap.h"
#include "../Commands/LiftByJoystick.h"

// BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=INCLUDES
// END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=INCLUDES

// BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=CONSTANTS

    // END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=CONSTANTS

LiftSys::LiftSys() : Subsystem("LiftSys") {
    // BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=DECLARATIONS
    liftMtr = RobotMap::liftSysLiftMtr;
    liftEncoder = RobotMap::liftSysLiftEncoder;
    liftUpLmtSw = RobotMap::liftSysLiftUpLmtSw;
    liftLoLmtSw = RobotMap::liftSysLiftLoLmtSw;
    liftPot = RobotMap::liftSysLiftPot;

    // END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=DECLARATIONS
}

void LiftSys::InitDefaultCommand() {
    // Set the default command for a subsystem here.
    // SetDefaultCommand(new MySpecialCommand());
    // BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=DEFAULT_COMMAND


    // END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=DEFAULT_COMMAND

	currEnc = 0, beginEnc = 0, endEnc = 0;
	currEncAngle = 0, beginEncAngle = 0, endEncAngle = 0;
	currLoLmtSw = 0, currHiLmtSw = 0;
}


// Put methods for controlling this subsystem
// here. Call these from Commands.

// --------------------- Drive Lift Motor ----------------------
void LiftSys::DriveLiftMtr(float speed){
	if ((speed < 0 ) && (GetLowerLmtSw() == true)){		// lowering hit the low limit switch
		liftMtr->Set(0);
		// ResetEncoder();		// not used anymore
		return;
	}
	if ((speed > 0 ) && (GetUpperLmtSw() == true)){  // raising and have hit the upper limit switch
		liftMtr->Set(0);
		return;
	}
	liftMtr->Set(speed * -1);	// everything is good so continue driving motor Reversed Polarity based on wiring
}


// -------------------------- Lift By Joy Stick -------------------------------
void LiftSys::LiftByJoystick(std::shared_ptr<Joystick> joy){
		throttle=joy->GetThrottle();						// put throttle value into variable Throttle
		throttle=((throttle-1)/-2);							// converts range to 0 to +1

		yValue=joy->GetY();									// sets y value of joystick to variable YValue
		if((yValue <= XYDEADBAND) &&
		   (yValue >= -XYDEADBAND))
				yValue=0;									// if YValue is within the deadband, don't move
		else
				yValue=yValue*throttle/1.7;					// changing YValue based on Throttle value

		DriveLiftMtr(yValue);

		// ---------- during TESTING ONLY drive the laserpointer ro here -------------
		//yValue = 1 - (yValue + 1) / 2;					// Also reverse direction to match joystick
}


// -------------------------- Lift By TopHat Switches on Joystick -----------------------------
void LiftSys::LiftByTopSw(std::shared_ptr<Joystick> joy){

	int topPOV = joy->GetPOV();

	if (topPOV == 0){
		DriveLiftMtr(kLiftSpeedTopHatSw * -1);				// top forward button pressed
		return;
	}
	if (topPOV == 180){
		DriveLiftMtr(kLiftSpeedTopHatSw);					// Reverse
		return;
	}
	DriveLiftMtr( 0 );										// no button presse3d so stop motor
}


void LiftSys::LiftMtrStop(){
	liftMtr->Set(0);
}

// ----------------------- Auto Aim Methods ------------------------
//  These variables hold the recommended shooter angles for auto aim

void LiftSys::PutShooterAutoAimFlag(bool flag){
	shooterAutoAimFlag = flag;
}

bool LiftSys::GetAutoAimFlag(){
	return shooterAutoAimFlag;
}

float LiftSys::GetShooterAngle(){
	return shooterAutoAimAngle;
}


void LiftSys::PutShooterAngle(float angle){
	shooterAutoAimAngle = angle;
}


// -------------------- Get Angle Measurement ------------------------------
float LiftSys::GetLiftAngle(int method){
	return GetLiftAngleByPot();
//	if (method == 0) return GetLiftAngleByPot();
//	if (method == 1) return GetLiftAngleByEncoder();
//	if (method == 2) return GetLiftAngleByPot();
//	return 0;
}


// ********************************************************************************
// ----------------------------- Pot Methods -------------------------------------
// ********************************************************************************

// ------------------ Set default Pot values for Conversion to Angle ---------------
void LiftSys::SetPotDefaults(float Offset, float Scale){	// This is initialized on startup by robot.cpp
	kPotOffset = Offset;
	kPotScale = Scale;
}

float LiftSys::GetLiftAngleByPot(){
	float volt = liftPot->GetAverageVoltage();
//	float angle= (volt - kPotOffset) / kPotScale;
	float angle = 35.915 * volt - 53.981;
	SmartDashboard::PutNumber("Lift Angle",angle);
	SmartDashboard::PutNumber("Lift Volt",volt);
//	SmartDashboard::PutNumber("Lift Offset",kPotOffset);
//	SmartDashboard::PutNumber("Lift Scale",kPotScale);
	return angle;
}


float LiftSys::GetLiftPotValue(){
	return (liftPot->GetAverageVoltage());
}

// ----------------------- Limit Switch Methods -------------------------
bool LiftSys::GetLowerLmtSw(){
	if (liftLoLmtSw->Get() == LIMITSWITCHPRESSED) return true;
	return false;
}
bool LiftSys::GetUpperLmtSw(){
	if (liftUpLmtSw->Get() == LIMITSWITCHPRESSED) return true;
	return false;
}

// ------------------- Encoder Methods -------------------------------------
float LiftSys::GetLiftAngleByEncoder(){
	return ConvertLiftEncoderToAngle(GetLiftEncoder());
}

void LiftSys::ResetEncoder(){
	liftEncoder->Reset();
}

double LiftSys::GetLiftEncoder(){
	return liftEncoder->GetRaw();
}

// ***********************************************************************************
//                  Convert encoder counts to angle and Vice Versa                   *
// ***********************************************************************************

// ------------------- Convert Angle to Encoder Cnt ------------------
float LiftSys::ConvertLiftAngleToEncoder(float angle){
	int i=0;
	int returnVal;

	if ( angle < AngleTbl[0] ) angle  = AngleTbl[0];									// Safe Low value
	if ( angle > AngleTbl[ EncTblSize-1] ) angle =  AngleTbl[EncTblSize-1];				// Safe Hi Value

	for (int i = 0; i < EncTblSize - 1; i++)
	{	// loop through table until we find the encoder cnt bound within Hi and Lo
		// break out. The i value will then point to the valid range. Then call a
		// routine that will extrapolate a more specific angle value

		if ((angle >= AngleTbl[i]) && (angle < AngleTbl[i+1])) break;
	}
	int encLo = EncCntTbl[i];
	int encHi = EncCntTbl[i+1];
	int angLo = AngleTbl[i];
	int angHi = AngleTbl[i+1];
	returnVal = int((((encHi - encLo) / (angHi - angLo)) * (angle - angLo)) + encLo);
	//printf ("\nEncoder calc enc=%f encLo=%f encHi=%f angleLo%f angHi%f return=%d",encValue, encLo, encHi, angleLo, angleHi, returnVal );
	return returnVal;
}


// ------------------- Convert Encoder Cnt to Angle ----------------------------
float LiftSys::ConvertLiftEncoderToAngle(float encCnt){
	int i=0;
	float tempAngle;

	if ( encCnt < EncCntTbl[0] ) encCnt  = EncCntTbl[0];									// Safe Low value
	if ( encCnt > EncCntTbl[ EncTblSize-1] ) encCnt =  EncCntTbl[EncTblSize-1];				// Safe Hi Value

	for (int i = 0; i < EncTblSize - 1; i++)
	{	// loop through table until we find the encoder cnt bound within Hi and Lo
		// break out. The i value will then point to the valid range. Then call a
		// routine that will extrapolate a more specific angle value

		if ((encCnt >= EncCntTbl[i]) && (encCnt < EncCntTbl[i+1])) break;
	}
	int encLo = EncCntTbl[i];
	int encHi = EncCntTbl[i+1];
	float angLo = AngleTbl[i];
	float angHi = AngleTbl[i+1];
	tempAngle = ((((angHi - angLo) / (encHi - encLo)) * (encCnt - encLo)) + angLo);
	//printf ("\nEncoder calc enc=%f encLo=%f encHi=%f angleLo%f angHi%f return=%d",encValue, encLo, encHi, angleLo, angleHi, returnVal );
	return tempAngle;
}


// ------------------- Log Data Methods ------------------------
void LiftSys::ZeroBeginEndValues(){
	beginEnc = 0;
	beginEncAngle = 0;
}

void LiftSys::PostBeginValues(){
	beginEnc = liftEncoder->GetRaw();
	beginEncAngle = GetLiftAngleByEncoder();
}

void LiftSys::PostEndValues(){
	endEnc = liftEncoder->GetRaw();
	endEncAngle = GetLiftAngleByEncoder();
}

void LiftSys::PostCurrValues(){
	currEnc = liftEncoder->GetRaw();
	currEncAngle = GetLiftAngleByEncoder();
}

void LiftSys::DisplaySmartdashValues(){

	DisplaySmartdashSimple();
/*
	SmartDashboard::PutNumber("Begin Encoder", beginEnc);
	SmartDashboard::PutNumber("Begin EncAngle",beginEncAngle);
	SmartDashboard::PutNumber("End Encoder", endEnc);
	SmartDashboard::PutNumber("End EncAngle",endEncAngle);
*/
}


void LiftSys::DisplaySmartdashSimple(){
	if (liftLoLmtSw->Get() == LIMITSWITCHPRESSED)
		SmartDashboard::PutString("Lower LmtSw", "Pressed");
	else
		SmartDashboard::PutString("Lower LmtSw", "NOT Pressed");

	if (liftUpLmtSw->Get() == LIMITSWITCHPRESSED)
		SmartDashboard::PutString("Upper LmtSw", "Pressed");
	else
		SmartDashboard::PutString("Upper LmtSw", "NOT Pressed");

	SmartDashboard::PutNumber("Lift Angle",GetLiftAngleByPot());
	SmartDashboard::PutNumber("Lift Pot Value",GetLiftPotValue());


	//	SmartDashboard::PutNumber("Curr Encoder", GetLiftEncoder());
	//	SmartDashboard::PutNumber("Curr EncAngle",GetLiftAngleByEncoder());

}
