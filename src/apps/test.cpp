/*
 * test.cpp
 *
 *  Created on: Nov 7, 2014
 *      Author: connor
 */

#include "../core/headers/StereoEyes.h"

int main() {

	StereoEyes system;

	//Set Calibration imgs directory
	system.setImgCalibDir("./img/calibration/myImg/");
	//Get Calibration Images
	system.getCalibImg(0, 0, 10);

	//Calibrate Cameras from input Images
	//system.Calib("./img/calibration/myImg/imgList.txt", 9, 6, 2.5);

	//Generate Disparity Maps
	//system.genDisparity(0,1);


	std::cout << "FINISHED\n";

	return 0;
}



