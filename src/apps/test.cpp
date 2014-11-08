/*
 * test.cpp
 *
 *  Created on: Nov 7, 2014
 *      Author: connor
 */

#include "../core/headers/StereoEyes.h"

int main() {

	StereoEyes system;

	system.Calibrate(0, 1);

	std::cout << "FINISHED\n";

	return 0;
}



