#include "GUI.h"
//LUCAS DUBOUCHET 3DMT

GUI::GUI()
{
	graph = Mat_<Vec3f>(440, 700); //We create a window of 440*700
	selectionFilt = Mat_<Vec3f>(440, 700);
	selectionAna = Mat_<Vec3f>(440, 700);
	graph = Vec3f(0.9f,0.9f,0.9f); //Set the Background
	selectionFilt = Vec3f(0, 0, 0);
	selectionAna = Vec3f(0, 0, 0);
	parameterFilter = 0;
	parameterAnaglyph = 2;
	Point pos(349, 0); //First position: at the half of the width, on top of the image
	vector<String> names = { "Gaussian","Median","True Anaglyph", "Gray Anaglyph","Color Anaglyph", "Half Color Anaglyph","Optimized Anaglyph" };
	//For each filter and anaglyph, we create a button
	for (int i = 0; i < names.size();i++)
	{
		pos.y += 55; //Each time, we create a button under the previous one
		addButton(names[i], pos);
	}
	
	namedWindow("Lucas Dubouchet Project", WINDOW_AUTOSIZE);
	installCallback();

}
void GUI::installCallback(void)
{
	setMouseCallback("Lucas Dubouchet Project", mouseEventCallback, this);
}

void GUI::mouseEventCallback(int evt, int x, int y, int flags, void * param)
{
	GUI* ptrGUI = (GUI*)param; //Same function as in the class
	ptrGUI->MouseEvent(evt, x, y, flags);
}




void GUI::MouseEvent(int evt, int x, int y, int flags)
{
	if (evt == EVENT_LBUTTONDOWN)
	{
		if (x >= 260 && x <= 440) //If the user clicks on the side, it is useless and nothing happens
		{
			if (y >= 55 && y <= 405) //If the user clicks on the zone where are all the buttons, we check which button he pressed
			{
				int n = (y+27) / 55; //This one is a little trick: we know that each button (and the zone below it) has a height of 55 pixels.
									 // Knowing that, we deduce the number of the button clicked by dividing the actual y of the mouse click by 55 
									 // We add 27 because otherwise the "hitbox" for each button would stop at the half of the height of the button
				if (n > 0 && n <= NUMBERFILTERS) //If we click a filter
				{
					parameterFilter = n-1; //we get the filter
					selectionFilt = Vec3f(0, 0, 0); //We reset the filter selection fot the GUI
					rectangle(selectionFilt, Point(349, n*55) - Point(90, 5), Point(349, n*55) + Point(90, 5), Scalar(0.4, 0.4, 0.4), 10, 3, 0); //We set it again on the right button
					this->show();
				}
				if (n > NUMBERFILTERS) //If we click an anaglyph
				{
					parameterAnaglyph =  n-1; //We get the anaglyph
					selectionAna = Vec3f(0, 0, 0); //We reset the anaglyph selection fot the GUI
					rectangle(selectionAna, Point(349, n * 55) - Point(90, 5), Point(349, n * 55) + Point(90, 5), Scalar(0.4, 0.4, 0.4), 10, 3, 0); //We set it on the reight button
					this->show();
				}
			}
		}
	}
}



GUI::~GUI()
{
}

void GUI::show()
{
	imshow("Lucas Dubouchet Project", graph+selectionAna+selectionFilt); //We show the main window+the selections in order to highlight the current selection of the user
	waitKey(0);
}


void GUI::addButton(String text, Point center)
{
	int sizeS = text.size();
	rectangle(graph, center - Point(90, 5), center + Point(90, 5), Scalar(0.75, 0.75, 0.75), 15, CV_FILLED, 0);
	putText(graph, text, center + Point(-sizeS * 10 / 2, 2.5), FONT_HERSHEY_PLAIN, 1, Scalar(0, 0, 0));
}

int GUI::getFilter()
{
	return parameterFilter;
}

int GUI::getAnaglyph()
{
	return parameterAnaglyph;
}
