#pragma once
//LUCAS DUBOUCHET 3DMT
/**Class Handling the GUI**/

#include "opencv/cv.hpp"

#include <iostream>
#include <vector>
#include<string>
using namespace std;
using namespace cv;
#define NUMBERFILTERS 2
class GUI
{

	int parameterFilter; //integer that gives the category of filter selected by the user
	int parameterAnaglyph;  //integer that gives the category of anaglyph selected by the user
	Mat_<Vec3f> graph; // Main window
	Mat_<Vec3f> selectionFilt; //Shows the choice of the user for the filter on the main window
	Mat_<Vec3f> selectionAna; //Shows the choice of the user for the anaglyph on the main window
	static void mouseEventCallback(int evt, int x, int y, int flags, void * param);
public:
	GUI();  
	void installCallback(void); //Setting the mouse callback
	
	void MouseEvent(int evt, int x, int y, int flags); //What happens when the user clicks on the window
	~GUI();
	void show(); //Shows the GUI

	void addButton(String text, Point center); //Add a button containing the string text at the position center
	int getFilter(); //Returns the ID of the filter selected by the user
	int getAnaglyph(); //Returns the ID of the anaglyph selected by the user
};

