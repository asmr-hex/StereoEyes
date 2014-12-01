/*
 * StereoEyes.h
 *
 *  Created on: Nov 7, 2014
 *      Author: connor
 */

#ifndef STEREOEYES_H_
#define STEREOEYES_H_

#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <string>
#include <algorithm>
#include <sstream>
#include <fstream>
#include <vector>
#include <stdlib.h>
#include <ctype.h>

#include <opencv2/opencv.hpp>
#include "cv.h"
#include "cxmisc.h"
#include "highgui.h"
#include "cvaux.h"



static void
StereoCalib(const char* imageList, int nx, int ny, int useUncalibrated, float _squareSize);

class StereoEyes {
public:
	StereoEyes(void);
	~StereoEyes(void);
	int getCalibImg(int camA, int camB, int imgN);
	void Calibrate(const char* imageList, int nx, int ny, double squareSize);
	void Calib(const char* imageList, int nx, int ny, double squareSize);
	cv::Mat genDisparity(int A, int B);
	void initDisparity(int nDisparities, int SADWindow,
			const char* mx1File, const char* my1File,
			const char* mx2File, const char* my2File);
	std::string getImgCalibDir() { return _img_calib_dir; }
	void setImgCalibDir(std::string newDir) {
		_img_calib_dir = newDir;
	}
private:
	std::string _img_calib_dir = "./img/calibration/";

};

#endif /* STEREOEYES_H_ */
