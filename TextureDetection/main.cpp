#include <iostream>
#include <stdio.h>
#include "ParticleFilter.h"
#include <time.h>
#include <ctime>
#include <fstream>
#include <iomanip>

using namespace std;
using namespace cv;

int X1, X2, Y1, Y2;
bool set = false;
ParticleFilter* mFilter = NULL;
Mat frame;

//FYP Report
bool stuck = true;
int output_counter = 1;

void CallBackFunc(int event, int x, int y, int flags, void* userdata)
{
	if (event == EVENT_LBUTTONDOWN)
	{
		X1 = x;
		Y1 = y;
		cout << "Left button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
	}
	else if (event == EVENT_RBUTTONDOWN)
	{
		cout << "Right button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
		X2 = x;
		Y2 = y;

		if (mFilter == NULL)
			mFilter = new ParticleFilter(frame.cols, frame.rows);
		else {
			delete mFilter;
			mFilter = new ParticleFilter(frame.cols, frame.rows);
		}
		mFilter->tracking_window_width = X2 - X1;
		mFilter->tracking_window_height = Y2 - Y1;
		mFilter->initial_tracking_window_width = X2 - X1;
		mFilter->initial_tracking_window_height = Y2 - Y1;
		mFilter->image_width = frame.cols;
		mFilter->image_height = frame.rows;
		mFilter->calculate_particles_xy_mean();
		mFilter->mean_x_in_previous_frame = mFilter->mean_x;
		mFilter->mean_y_in_previous_frame = mFilter->mean_y;
		mFilter->set_from_initial_frame(frame, X1, Y1, X2, Y2);

		set = true;
	}
}

int main(void)
{
	VideoCapture capture;
	namedWindow("Video", 1);
	setMouseCallback("Video", CallBackFunc, NULL);

	srand(time(NULL));

	capture.open(0);
	if (!capture.isOpened()) { printf("--(!)Error opening video capture\n"); return -1; }

	while (capture.read(frame))
	{
		if (frame.empty())
		{
			printf(" --(!) No captured frame -- Break!");
			break;
		}

		if (mFilter != NULL && set) {
			mFilter->on_newFrame(&frame);
		}

		imshow("Video", frame);

		int c = waitKey(25);
		if ((char)c == 27) { break; } // escape
	}
	capture.release();
	cvDestroyAllWindows();

	return 0;
}
