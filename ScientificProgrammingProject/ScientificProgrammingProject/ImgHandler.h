#pragma once
#include<iostream>
#include<vector>
#include"opencv\cv.hpp"
#include "GUI.h"
//LUCAS DUBOUCHET 3DMT
#define GAUSSIAN 0
#define MEDIAN 1

//Class that handles the images in the "first step" (Opening it and filtering it)
using namespace std;
using namespace cv;
class ImgHandler
{
private:
	Mat_<Vec3f> img; //The actual data
	vector<Vec2i> ret; //The points that are used in order to apply the filter
	void installCallback(void);
	static void mouseEventCallback(int evt, int x, int y, int flags, void* param);
public:
	ImgHandler();
	ImgHandler(String filename); 
	ImgHandler(Mat_<Vec3f> data);
	~ImgHandler();
	void ApplyFilter(int type);
	void ImgShow();
	void dispSize() { cout << img.rows << endl << img.cols << endl; } //Little debug function, not actually used in this version
	void MouseEvent(int evt, int x, int y, int flags);
	vector<Vec2i> getRet() { return ret; } 
	Vec2i getSize();
	Mat_<float> getImgAt(int x, int y);
};

