#include "Anaglyph.h"
//LUCAS DUBOUCHET 3DMT
using namespace cv;

void Anaglyph::mouseEventCallbackAna(int evt, int x, int y, int flags, void * param)
{
	Anaglyph* ptrAna = (Anaglyph*)param;
	ptrAna->MouseEventAna(evt, x, y, flags);
}

Anaglyph::Anaglyph()
{

}

void Anaglyph::MouseEventAna(int evt, int x, int y, int flags)
{
	if (evt == EVENT_LBUTTONDOWN) //Left click of the mouse 
	{
		shift -= 20;
		shiftAnaglyph(shift);
	}
	if (evt == EVENT_RBUTTONDOWN) //Left click of the mouse 
	{
		shift += 20;
		shiftAnaglyph(shift);
	}
}

Anaglyph::Anaglyph(cv::Mat_<Vec3f> data,int type):ImgHandler(data) //This one is long and tedious, but not complicated
{
	left = Mat_<float>(3, 3); //We create two 3*3 matrices
	left = 0;
	right = Mat_<float>(3, 3);
	right = 0;
	shift = 0;
	//Actually, I took the formulas and associated matrix on this webpage http://www.3dtv.at/knowhow/anaglyphcomparison_en.aspx
	//So now, we have matrices filled with zeros. We just have to insert the right values for each type of anaglyph (remembering that they work in RGB and we work in BGR so we have to reverse the values)
	switch (type)
	{
	case TRUE:
		left(2, 2) = 0.299f;
		left(2, 1) = 0.587f;
		left(2, 0) = 0.114f;
		right(0, 0) = 0.114f;
		right(0,1) = 0.587f;
		right(0, 2) = 0.299f;
		break;

	case GRAY:
		left(2, 2) = 0.299f;
		left(2, 1) = 0.587f;
		left(2, 0) = 0.114f;
		right(0, 0) = 0.114f;
		right(0, 1) = 0.587f;
		right(0, 2) = 0.299f;
		right(1, 0) = 0.114f;
		right(1, 1) = 0.587f;
		right(1, 2) = 0.299f;
		break;

	case COLOR:
		left(2, 0) = 1;
		right(1, 1) = 1;
		right(0, 2) = 1;
		break;
	case HALF:
		left(2, 2) = 0.299f;
		left(2, 1) = 0.587f;
		left(2, 0) = 0.114f;
		right(1, 1) = 1;
		right(0, 2) = 1;
		break;
	case OPTI:
		left(2, 0) = 0.3f;
		left(2, 1) = 0.7f;
		right(1, 1) = 1;
		right(0, 2) = 1;
		break;
	default:
		break;
	}

	
}


Anaglyph::~Anaglyph()
{
}

void Anaglyph::ApplyAnaglyph(int width, int height, Vec2i begin1, Vec2i begin2) //This one is the most important function of the program
{
	//Parameters: height, width, the top left corner of our left image and the top left corner of our right image
	resultblue = Mat_<Vec3f>(height, width); //We create our two result matrices
	resultred = Mat_<Vec3f>(height, width);
	cout << "Computing Anaglyph..." << endl;
	for (int i = 0; i < width; i++) //Going through the height and width
	{
		for (int j = 0; j < height; j++)
		{
			int ind1 = begin1(0) + j; //This looks a bit awkward because of the indices in openCV: we compute the index of the pixel we want to change on the left and right images
									  // We take the index of the top left corner of each image and add i and j in order to know which pixels we're using on the main image.
			int ind2 = begin1(1) + i;
			int ind3 = begin2(0) + j;
			int ind4 = begin2(1) + i;
			Mat_<float> temp(3, 1);
			Mat_<float> temp2(3, 1);
			//We use Matrices in order to use the built-in matrix multiplication operator between matrices and vector
			temp = left*getImgAt(ind1, ind2); //Simple matrix multiplication to get left and right values

			temp2 = right*getImgAt(ind3, ind4); 
			resultred(j, i) = Vec3f(temp(0, 0), temp(1, 0), temp(2, 0)); //We convert the matrix we got to BGR float vectors, having the red and blue results
			resultblue(j, i) = Vec3f(temp2(0, 0), temp2(1, 0), temp2(2, 0));
		}
	}
	finalresult = Mat_<Vec3f>(height, width);
	finalresult = resultblue + resultred;
}

void Anaglyph::ShowAnaglyph()
{
	 //We show the final result
	imshow("Anaglyph Display", finalresult);
	waitKey(0);
}

void Anaglyph::shiftAnaglyph(int scale) //WIP: shifting the red anaglyph to make it "fit" for the user: for the moment the issue is that our image is the same size has our results so the borders are left out and unicolor
{ //For the moment it only works with mouse events
	resultblue.copyTo(finalresult); //We take the blue result
	
	if (scale > 0)
	{
		for (int i = scale; i < resultblue.cols; i++) //We add the red result with a shift
		{
			for (int j = 0; j < resultblue.rows; j++)
			{
				finalresult(j, i) += resultred(j,i - scale);
			}
		}
	}
	if (scale < 0)
	{
		for (int i = 0; i < resultblue.cols+scale; i++)
		{
			for (int j = 0; j < resultblue.rows; j++)
			{
				finalresult(j, i) += resultred(j, i - scale);
			}
		}
	}
	if (scale == 0) //No shift if the value is 0
	{
		finalresult += resultred;
	}
	//We show the image
	this->ShowAnaglyph();
}

void Anaglyph::process(int filt) //Function which sets up the filtering and the anaglyph
{
	vector<Vec2i> ret = getRet(); //We get the points selected by the user
	if (ret.size() >= 3) //If there are enough points
	{
		Vec2i point1 = ret[0]; //First point is the top left corner of the left image
		Vec2i point2 = ret[2]; //Third point is the top left corner of the right image
		Vec2i diff = ret[1] - ret[0]; //We get the width and the height by taking the difference between the second point ( the bottom right corner of the left image) and the first point (top left corner)
		Vec2i bounds = getSize();
		if ((point2[0] + diff[0] < bounds[0] && point2[1] + diff[1] < bounds[1]) && point2[1] > point1[1]) //If the two frames are in the image and don't go too far
		{
			int height = diff[0];
			int width = diff[1]; //we get the height and width
			ApplyFilter(filt); //Apply the filter
			ApplyAnaglyph(width, height, point1, point2); //Appply the anaglyph
			namedWindow("Anaglyph Display", WINDOW_NORMAL); //Generate the window, set the mouse callback
			setMouseCallback("Anaglyph Display", mouseEventCallbackAna, this);
			ShowAnaglyph();
		}
		else cout << "Out of bounds: please select a size that remains in the image" << endl;
	}
	else cout << "Not enough points: please select more points" << endl;
}

