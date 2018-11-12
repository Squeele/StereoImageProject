#pragma once
#include<iostream>
#include "opencv\cv.hpp"
#include <vector>
#include "ImgHandler.h"
//LUCAS DUBOUCHET 3DMT
#define TRUE 2
#define GRAY 3
#define	COLOR 4
#define	HALF 5
#define	OPTI 6
//Class that handles the anaglyph part
class Anaglyph: public ImgHandler
{
private:
	cv::Mat_<float> left; //Left matrix for the formula
	cv::Mat_<float> right;//Right matrix for the formula
	cv::Mat_<Vec3f> resultblue; //Result for the "blue" anaglyph
	cv::Mat_<Vec3f> resultred; //Result for the "red" anaglyph
	cv::Mat_<Vec3f> finalresult; //Final result(obviously): resultblue+resultred
	int shift;
	static void mouseEventCallbackAna(int evt, int x, int y, int flags, void* param);
public:
	Anaglyph();
	void MouseEventAna(int evt, int x, int y, int flags);
	Anaglyph(cv::Mat_<Vec3f> data, int type); //Create an Anaglyph object from an image and a type (see definition for more details)
	~Anaglyph();
	void ApplyAnaglyph(int width, int height,Vec2i begin1, Vec2i begin2); //Apply the formula for each pixel, eventually getting our anaglyph image
	void ShowAnaglyph();
	void shiftAnaglyph(int scale); //WIP (hope it will be finished) Shifting the anaglyph red in order to make it "fit"
	void process(int filt); //Function that handles the main part of the progral(see definition)
};

