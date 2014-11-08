/*
 * calibrate.cpp
 *
 *  Created on: Nov 7, 2014
 *      Author: connor
 */

#include "./headers/StereoEyes.h"


StereoEyes::StereoEyes(void) {

}

StereoEyes::~StereoEyes(void) {

}

int StereoEyes::Calibrate(int camA, int camB){

	int frameWait = 3, imgN = 0;
	char* calib_dir = "./img/calibration/";
	char leftFN[50];
	char rightFN[50];

    //initialize and allocate memory to load the video stream from camera
    cv::VideoCapture cameraA(camA);
    cv::VideoCapture cameraB(camB);

    if( !cameraA.isOpened() ) return 1;
    if( !cameraB.isOpened() ) return 1;

    while(true) {

    	/*Count down to image acquisition*/
    	for (int k = 1; k <= frameWait ; k++){
    		sleep(1);
    		std::cout << k << "..." << std::endl;
    	}
    	std::cout <<"Acquiring Images..." << std::endl;

        //grab and retrieve each frames of the video sequentially
        cv::Mat3b frame0;
        cameraA >> frame0;
        cv::Mat3b frame1;
        cameraB >> frame1;

        cv::imshow("Video0", frame0);
        cv::imshow("Video1", frame1);

        /*Get image names*/
        sprintf(leftFN, "%sleft_%d.png", calib_dir, imgN);
        sprintf(rightFN, "%sright_%d.png", calib_dir, imgN);

        /*Save Captured Images */
        cv::imwrite(leftFN, frame0);
        cv::imwrite(rightFN, frame1);
        imgN++;

        //wait for 40 milliseconds
        int c = cvWaitKey(40);
        //exit the loop if user press "Esc" key  (ASCII value of "Esc" is 27)
        if(27 == char(c)) break;
    }

    return 1;
}





