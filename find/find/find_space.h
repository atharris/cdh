#ifndef _FINDSTAR_H_
#define _FINDSTAR_H 

#define FSTARS 1
#define FSTREAKS 2

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include <string>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>

using namespace cv;
using namespace std;

// Handles command line parsing
class Commandline {
	public:
		Commandline(){};
		void displayError(string, int);
		bool validFileCheck(char* filepath);
		int convertPosInt(char* str);
};

// Handles converting image
class SpaceImage {
	private:
		Mat img, img_b;
		int thresh;
	public: 
		SpaceImage(Mat, int);
		void convertImageToBinary();
		Mat getBinaryImage();
};

// Findstreaks
class Findstreaks {
	private: 
		Mat img, img_b, img_edge; 
		int lineThresh, minLineLength, maxLineGap; 
		vector<vector<float> > streakVec; 
		vector<Vec4i> lines;

	public: 
		// Constructor
		Findstreaks(string, int, int, int, int);
		bool outputStreakData();
		void outputImage();
	private:
		void draw(Mat&, Point, Point, Point);
		bool gatherStreakData();
		void setupImage();
		float brightnessCalc(Point);
};

// Findstars
class Findstars {
	private:
		Mat img_b, img;
		vector<Point2f> mc;

	public:
		Findstars(string, int);
		vector<complex <float> > getStarVec();
		bool outputStarLocations();
		void outputImage();
	private:
		void draw(Mat&, Point, Point);
		bool gatherStarDataFromImage();
		string numToStr(float);
};

// Constellations
class Constellations {
private:
	vector<complex <float> > starVec;
	vector<complex <float> > constellationVec;
	vector<string> fileVec;

public:
	Constellations(	vector<complex <float> >);

private:
	void constellationCheck();
	bool updateConstellationVec(string);
	bool initFileVec();
	float convertToken(string);
};
#endif

