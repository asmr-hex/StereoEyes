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

int StereoEyes::getCalibImg(int camA, int camB, int imgN){

	int frameWait = 120;
	int font = cv::FONT_HERSHEY_PLAIN;
	double fontSize[] = {3, 5};
	int countDown = 3;
	std::ostringstream text;
	std::string dir = getImgCalibDir();
	std::ostringstream leftFN, rightFN;
	char imList[50];
	std::sprintf(imList, "%simgList.txt", dir.c_str());
	FILE* fd = fopen(imList, "w");
	int imgCount = 0;

    //initialize and allocate memory to load the video stream from camera
    cv::VideoCapture cameraA(camA);
    cv::VideoCapture cameraB(camB);

    if( !cameraA.isOpened() ) return 1;
    if( !cameraB.isOpened() ) return 1;

    cvNamedWindow( "Video0" );
    cvNamedWindow( "Video1" );

    while(imgCount < imgN) {
    	leftFN.str(""); rightFN.str("");
    	leftFN.clear(); rightFN.clear();
    	/*Count down to image acquisition*/
    	for (int k = 1; k <= frameWait ; k++){
            //grab and retrieve each frames of the video sequentially
            cv::Mat3b frame0;
            cameraA >> frame0;
            cv::Mat3b frame1;
            cameraB >> frame1;

            //Make Output smaller
            cv::Mat3b smallFrame0, smallFrame;
            cv::Size size(frame0.cols/2, frame0.rows/2);
            cv::resize(frame0, frame0, size);
            cv::resize(frame1, frame1, size);

            //Move Windows
            cv::moveWindow("Video0",0, 0);
            cv::moveWindow("Video1",frame0.cols, 0);

            text.str("");
            text.clear();
            if(fmod(k,frameWait/3) == 0){
            	countDown--;
            	countDown = countDown == 0 ? 3 : countDown;
            	text << countDown << "...";
            	cv::Point textOrigin(frame0.cols/2 - 10, frame0.rows/2 + 10);
            	cv::putText(frame0, text.str() , textOrigin, font,
            			fontSize[1], cv::Scalar(192,83,69));
            	cv::putText(frame1, text.str() , textOrigin, font,
            			fontSize[1], cv::Scalar(192,83,69));
            }else{
            	text << countDown << "...";
            	cv::Point textOrigin(frame0.cols/2, frame0.rows/2);
            	cv::putText(frame0, text.str(), textOrigin, font,
            			fontSize[0], cv::Scalar(0,154,159));
            	cv::putText(frame1, text.str(), textOrigin, font,
            			fontSize[0], cv::Scalar(0,154,159));
            }

            //cv::resizeWindow("Video0", frame0.cols/2, frame0.rows/2 );
            cv::imshow("Video0", frame0);
            cv::imshow("Video1", frame1);

    	}
    	std::cout <<"Acquiring Images..." << std::endl;

        //grab and retrieve each frames of the video sequentially
        cv::Mat3b frame0;
        cameraA >> frame0;
        cv::Mat3b frame1;
        cameraB >> frame1;

        //Make Output smaller
        cv::Mat3b small_frame0, small_frame1;
        cv::Size size(frame0.cols/2, frame0.rows/2);
        cv::resize(frame0, small_frame0, size);
        cv::resize(frame1, small_frame1, size);

        cv::imshow("Video0", small_frame0);
        cv::imshow("Video1", small_frame1);

        /*Get image names*/
        leftFN << dir << "left" << imgCount << ".png";
        rightFN << dir << "right" << imgCount << ".png";

        std::cout << leftFN.str() << std::endl;

        /*Save Captured Images */
        cv::imwrite(leftFN.str(), frame0);
        cv::imwrite(rightFN.str(), frame1);

        //Save file names in list
        fprintf(fd, "%s\n%s\n", leftFN.str().c_str(), rightFN.str().c_str());
        //update imgCount
        imgCount++;

        //wait for 40 milliseconds
        int c = cvWaitKey(40);
        //exit the loop if user press "Esc" key  (ASCII value of "Esc" is 27)
        if(27 == char(c)) break;
    }
    fclose(fd);
    return 1;
}

void StereoEyes::Calib(const char* imgList, int nx, int ny, double squareSize){

}

void StereoEyes::Calibrate(const char* imgList, int nx, int ny, double squareSize)
{
	//Initialize Variables
	CvSize imageSize = {0,0};
	int cornersFound, N, n = nx*ny;
	std::vector<cv::Point2f> corners, points[2];
	std::vector<cv::Point3d> objectPoints;
    // ARRAY AND VECTOR STORAGE:
    double M1[3][3], M2[3][3], D1[5], D2[5];
    double R[3][3], T[3], E[3][3], F[3][3];
    double Q[4][4];
    cv::Mat _M1(3, 3, CV_64F, M1 );
    cv::Mat _M2(3, 3, CV_64F, M2 );
    cv::Mat _D1(1, 5, CV_64F, D1 );
    cv::Mat _D2(1, 5, CV_64F, D2 );
    cv::Mat _R(3, 3, CV_64F, R );
    cv::Mat _T(3, 1, CV_64F, T );
    cv::Mat _E(3, 3, CV_64F, E );
    cv::Mat _F(3, 3, CV_64F, F );
    cv::Mat _Q(4,4, CV_64F, Q);

	//Read in ImageList
	std::vector<std::string> imgNames[2];
	std::ifstream fs(imgList);
	std::string fn;
	int imgN = 0;
	while(fs >> fn){
		imgNames[imgN%2].push_back(fn);
		imgN++;

		//Find Chessboards and Corners within this image
		//(Add find on multi-scale option)
		cv::Mat img = cv::imread(fn, CV_LOAD_IMAGE_COLOR);
		//imageSize = cvGetSize(img);
		cornersFound = cv::findChessboardCorners(img, cv::Size(nx, ny),
				corners, CV_CALIB_CB_ADAPTIVE_THRESH +
				CV_CALIB_CB_NORMALIZE_IMAGE);
		//end multi-scale option

		//Display found corners
		cv::Mat timg = img.clone();
		cv::cvtColor(img, img, CV_BGR2GRAY);
		cv::cvtColor(img, timg, CV_GRAY2BGR);
		cv::drawChessboardCorners(timg, cv::Size(nx, ny),
				corners, cornersFound);
		cv::namedWindow("IMAGE", cv::WINDOW_AUTOSIZE);
		cv::imshow("IMAGE", timg);
		int c = cvWaitKey(1000);

		N = points[imgN%2].size();
		points[imgN%2].resize(N + n, cv::Point(0,0));
		//subpixel resolution and save Points
        if( cornersFound ){
            cv::cornerSubPix(img, corners,
                cv::Size(11, 11), cv::Size(-1,-1),
                cv::TermCriteria(cv::TermCriteria::COUNT +
                		cv::TermCriteria::EPS, 30, 0.01) );
            std::copy( corners.begin(), corners.end(), points[imgN%2].begin() + N );
        }
	}
	//Close input Image File
	fs.close();

	//Setup Calibration Parameters
	objectPoints.resize(N*n);
    for(int i = 0; i < ny; i++ )
    	for(int j = 0; j < nx; j++ )
    		objectPoints[i*nx + j] = cv::Point3d(i*squareSize, j*squareSize, 0);
    for(int i = 1; i < imgN/2; i++)
        copy( objectPoints.begin(), objectPoints.begin() + n,
        objectPoints.begin() + i*n );
	cv::Mat objPoints(1, N, CV_32FC3, &objectPoints[0]);
    cv::Mat imagePoints1(1, N, CV_32FC2, &points[0][0] );
    cv::Mat imagePoints2(1, N, CV_32FC2, &points[1][0] );
    cvSetIdentity(&_M1);
    cvSetIdentity(&_M2);
    cvZero(&_D1);
    cvZero(&_D2);

	//Calibrate Cameras
	std::cout << "Calibrating Stereo Cameras...\n";
//	cv::calibrateCamera(objPoints, imagePoints1, imagePoints2,
//			_M1, _D1, _M2, _D2, imageSize, _R, _T, _E, _F,
//	        cv::TermCriteria(cv::TermCriteria::COUNT +
//	        cv::TermCriteria::EPS, 100, 1e-5),
//	        CV_CALIB_FIX_ASPECT_RATIO +
//	        CV_CALIB_ZERO_TANGENT_DIST +
//	        CV_CALIB_SAME_FOCAL_LENGTH);



}

void StereoEyes::initDisparity(int nDisparities, int SADWindow,
			const char* mx1File, const char* my1File,
			const char* mx2File, const char* my2File){



}

cv::Mat StereoEyes::genDisparity(int A, int B){

	cv::StereoBM stereo( cv::StereoBM::BASIC_PRESET , 0, 21);

	//Load Camera Matrices
	cv::FileStorage xmlFile;
	xmlFile.open("mx1.xml", cv::FileStorage::READ);
	cv::Mat mx1;
	xmlFile["mx1"] >> mx1;
	std::cout << mx1 << std::endl;
	xmlFile.release();
	xmlFile.open("my1.xml", cv::FileStorage::READ);
	cv::Mat my1;
	xmlFile["my1"] >> my1;
	std::cout << my1 << std::endl;
	xmlFile.release();

	xmlFile.open("mx2.xml", cv::FileStorage::READ);
	cv::Mat mx2;
	xmlFile["mx2"] >> mx1;
	std::cout << mx2 << std::endl;
	xmlFile.release();
	xmlFile.open("my2.xml", cv::FileStorage::READ);
	cv::Mat my2;
	xmlFile["my2"] >> my1;
	std::cout << my2 << std::endl;
	xmlFile.release();


    //grab and retrieve each frames of the video sequentially
    //initialize and allocate memory to load the video stream from camera
    cv::VideoCapture cameraA(A);
    cv::VideoCapture cameraB(B);
    cv::Mat frame0;
    cameraA >> frame0;
    cv::Mat frame1;
    cameraB >> frame1;
    cv::cvtColor(frame1, frame1, CV_BGR2GRAY);
    cv::cvtColor(frame0, frame0, CV_BGR2GRAY);

    cv::remap(frame0, frame0, mx1, my1, CV_INTER_LINEAR, cv::BORDER_CONSTANT, cv::Scalar(0,0, 0));
    cv::remap(frame1, frame1, mx2, my2, CV_INTER_LINEAR, cv::BORDER_CONSTANT, cv::Scalar(0,0, 0));

    cv::Mat disp;
    stereo(frame0, frame1, disp);

    return disp;

}


