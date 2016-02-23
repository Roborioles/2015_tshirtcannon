/*
 * VisionCom.h
 *
 *  Created on: Feb 19, 2016
 *      Author: James
 */

#ifndef VISIONCOM_H_
#define VISIONCOM_H_
#include "Commands/Command.h"
#include "WPILIB.h"
class VisionCom:public Command{

private:

	static NetworkTable* contours;
	static void turnLeft(double);
	static void turnRight(double);
	static void stopTurning();
	static void startGrip();
	static const int width=320, height=240,imageCenterX=width/2,looseTolerance=5,tightTolerance=3;
public:
	VisionCom();
	static void pauseGrip();
	static void resumeGrip();
	static void alignBot();
	virtual void Initialize();
	virtual void Execute();
	virtual bool IsFinished();
	virtual void End();
	virtual void Interrupted();
};


#endif /* SRC_COMMANDS_VISIONCOM_H_ */
