/*
 * VisionCom.cpp
 *
 *  Created on: Feb 19, 2016
 *      Author: James
 */
#include "VisionCom.h"
#include "../Robot.h"
VisionCom::VisionCom(){
	startGrip();
	pauseGrip();
	Requires(Robot::mover);

}
NetworkTable* VisionCom::contours=NULL;
void VisionCom::alignBot(){
	if(contours!=NULL){
		std::vector<double> centers=contours->GetNumberArray("centerX",llvm::ArrayRef<double>());
		//			int ctr=0;
		//			for(double x:centers)
		//			{
		//				printf("x at %d : %f",ctr++,x);
		//			}
		int centerX=centers.size()>0?centers.at(0):imageCenterX;

		int difference = imageCenterX-centerX;
		printf("difference: %d\n",difference);

		if(centers.size()==0){
			printf("-----\n");
			stopTurning();
		}
		else if (abs(difference) <= tightTolerance)
		{
			printf("--|--\n");
			stopTurning();
		}
		else if (difference<-tightTolerance)
		{
			if(difference >=-looseTolerance)
			{
				printf("-|---\n");
				turnLeft(.05);
			}
			else
			{
				printf("|----\n");
				turnLeft(.1);
			}
		}
		else if(difference>tightTolerance)
		{
			if(difference<=looseTolerance)
			{
				printf("---|-\n");
				turnRight(.05);
			}
			else
			{
				printf("----|\n");
				turnRight(.1);
			}
		}
	}else{
		contours=NetworkTable::GetTable("GRIP/contoursReport").get();
	}
}

void VisionCom::stopTurning(){

}

void VisionCom::turnLeft(double speed){
	Robot::mover->leftMotor2->Set(-speed);
	Robot::mover->leftMotor3->Set(-speed);

	Robot::mover->rightMotor0->Set(speed);
	Robot::mover->rightMotor1->Set(speed);
}

void VisionCom::turnRight(double speed){

	Robot::mover->leftMotor2->Set(speed);
	Robot::mover->leftMotor3->Set(speed);
	Robot::mover->rightMotor0->Set(-speed);
	Robot::mover->rightMotor1->Set(-speed);
}

void VisionCom::startGrip(){
	USBCamera* cam=new USBCamera("cam0",false);  //This objects constructor opens camera for us

	cam->SetBrightness(50);
	cam->SetWhiteBalanceManual(whiteBalance::kFixedFluorescent1);
	cam->SetExposureAuto();
	//cam->UpdateSettings();
	//cam->OpenCamera();
	delete cam;
	std::system("/usr/local/frc/JRE/bin/java -jar /home/lvuser/grip.jar /home/lvuser/project.grip");

	NetworkTable::GetTable("GRIP")->PutBoolean("run",false);

}

void VisionCom::resumeGrip(){

	NetworkTable::GetTable("GRIP")->PutBoolean("run",true);
}

void VisionCom::pauseGrip(){

	NetworkTable::GetTable("GRIP")->PutBoolean("run",false);
	delete contours;
}

void VisionCom::Initialize(){
	resumeGrip();
}

void VisionCom::Execute(){
	alignBot();
}

void VisionCom::End(){
	stopTurning();
	pauseGrip();
}

bool VisionCom::IsFinished(){
	return false;
}

void VisionCom::Interrupted(){
	End();
}
