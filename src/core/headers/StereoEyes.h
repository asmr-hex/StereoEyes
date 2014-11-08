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
#include <opencv2/opencv.hpp>

class StereoEyes {
public:
	StereoEyes(void);
	~StereoEyes(void);
	int Calibrate(int camA, int camB);

};

#endif /* STEREOEYES_H_ */
