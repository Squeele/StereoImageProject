#include "ImgHandler.h"
//LUCAS DUBOUCHET 3DMT
void ImgHandler::installCallback(void)
{
	setMouseCallback("ImgHandler Display", mouseEventCallback, this); //Mouse event callback again
}

void ImgHandler::mouseEventCallback(int evt, int x, int y, int flags, void * param)
{
	ImgHandler* ptrImgHandler = (ImgHandler*)param;
	ptrImgHandler->MouseEvent(evt, x, y, flags);
}

ImgHandler::ImgHandler()
{
}

ImgHandler::ImgHandler(String filename) //Create an ImgHandler from a filename (used early for tests)
{
	img = imread(filename);
	img = img / 255;
	namedWindow("ImgHandler Display", WINDOW_NORMAL);
	installCallback();
}

ImgHandler::ImgHandler(Mat_<Vec3f> data) //Create an ImgHandler from a Matrix
{
	img = data;
	img = img / 255; //normalizing the data
	namedWindow("ImgHandler Display", WINDOW_NORMAL);
	installCallback();
}



ImgHandler::~ImgHandler()
{
}

void ImgHandler::ApplyFilter(int type)
{
	int rows = img.rows;
	int cols = img.cols;
	Mat_<Vec3f> dest(rows,cols); 
	switch (type)
	{
	case GAUSSIAN:
		cout << "Computing Gaussian Blur..." << endl;
		GaussianBlur(img, dest, cv::Size(5, 5), 0, 0); //Using the blur
		for (int i = 0; i < rows; i++) //Copying it back on the first image
		{
			for (int j = 0; j < cols; j++)
			{
				img(i, j) = dest(i, j);
			}
		}
		break;
	case MEDIAN:
		cout << "Computing Median Blur..." << endl;
		medianBlur(img, dest, 5); //Same thing that the gaussian blur
		for (int i = 0; i < rows; i++)
		{
			for (int j = 0; j < cols; j++)
			{
				img(i, j) = dest(i, j);
			}
		}
		break;
	default:
		break;
	}
}

void ImgHandler::ImgShow()
{
	imshow("ImgHandler Display", img);
	waitKey(0);
}


void ImgHandler::MouseEvent(int evt, int x, int y, int flags)
{
	if (evt == EVENT_LBUTTONDOWN) //Left click of the mouse 
	{
		ret.push_back(Vec2i(y, x)); //putting the coordinates of the click in the point vector
		rectangle(img, Point(x,y)- Point(5, 5), Point(x, y)+ Point(5, 5), Scalar(0, 0, 1), 15, 3, 0); //Drawing a little red point so the user knows where his "current" clicks are
		this->ImgShow();
	}
	if (evt == EVENT_RBUTTONDOWN) //Right click
	{
		if (!ret.empty()) //Let's avoid some segfaults obviously
		{
			Vec2i temp = ret.back(); //Get the top of the vector
			rectangle(img, Point(temp[1], temp[0]) - Point(10, 10), Point(temp[1], temp[0]) + Point(10, 10), Scalar(1, 1, 1), 15, 3, 0); //Draw it in white so the user knows where are his previous clicks
			ret.pop_back(); //Then, deleting that point from the actual points that will be returned
			this->ImgShow();
		}
	}

}

Vec2i ImgHandler::getSize()
{
	return Vec2i(img.rows,img.cols);
}

Mat_<float> ImgHandler::getImgAt(int x, int y) //Get the BGR coordinates at the coordinates x,y (useful for the Anaglyph child class) 
{
	Mat_<float> ret(3, 1);
	Vec3f t = img(x, y);
	ret(0, 0) = t[0]; //We convert them as vectors in order to be able to use maltrix multiplication in the function Anaglyph::ApplyAnaglyph
	ret(1, 0) = t[1];
	ret(2, 0) = t[2];
	return ret;
}
