#include<iostream>
#include"ImgHandler.h"
#include"GUI.h"
#include "Anaglyph.h"

//LUCAS DUBOUCHET 3DMT
int main(int argc, char* argv[])
{
	if (argc >= 2) //We check if we have an argument
	{
		GUI inter = GUI();
		inter.show();
		Mat_<Vec3f> test = imread(argv[1]);
		if (!test.empty()) //We check if you actually opened an image
		{
			int ana = inter.getAnaglyph();
			int filt = inter.getFilter();
			Anaglyph testAna = Anaglyph(test, ana); 
			testAna.ImgShow();
			testAna.process(filt);
			return 0;
		}
	}
	else
	{
		cout << "Not enough arguments" << endl;
		int i;
		cin >> i;
		return -1;
	}
}
