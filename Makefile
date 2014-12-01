CC=g++
CFLAGS= -I/usr/local/include/opencv
LIBS = -L/usr/local/lib -lopencv_core -lopencv_calib3d -lopencv_imgproc -lopencv_highgui
DIALECT = -std=c++11
SRC = ./src/apps/test.cpp ./src/core/StereoEyes.cpp
OBJ = test.o StereoEyes.o

all:
	#Compile
	$(CC) -c ./src/core/calibrateC.cpp `pkg-config --cflags opencv`
	$(CC) $(DIALECT) -c $(SRC) $(CFLAGS)

	#Link
	$(CC) -o TEST $(OBJ) calibrateC.o $(LIBS) `pkg-config --libs opencv`

	#Cleanup
	rm $(OBJ)
	

fresh:
	rm TEST
