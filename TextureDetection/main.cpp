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
ParticleFilter* mFilter = NULL;
Mat frame, pre_frame;

String cup = "C:/Users/TH WU/Downloads/seqB/Vid_B_cup.avi";
String ball = "C:/Users/TH WU/Downloads/seqA/Vid_A_ball.avi";
String person = "C:/Users/TH WU/Downloads/seqD/Vid_D_person.avi";
String persons = "C:/Users/TH WU/Downloads/seqI/Vid_I_person_crossing.avi";
bool stuck = true;

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

		if (mFilter == NULL) {
			//mFilter = new ParticleFilter(frame.cols, frame.rows);
			mFilter = new ParticleFilter((X1+X2)/2, (Y1+Y2)/2, (X2 - X1)/2, (Y2 - Y1)/2);
		}
		else {
			delete mFilter;
			//mFilter = new ParticleFilter(frame.cols, frame.rows);
			//mFilter = new ParticleFilter(X1, Y1, X2 - X1, Y2 - Y1);
			mFilter = new ParticleFilter((X1 + X2) / 2, (Y1 + Y2) / 2, (X2 - X1) / 2, (Y2 - Y1) / 2);
		}
		mFilter->initialization(X1, Y1, X2, Y2, frame);
	}
}

int main(void)
{
	VideoCapture capture;
	namedWindow("Video", 1);
	setMouseCallback("Video", CallBackFunc, NULL);

	srand(time(NULL));

	capture.open(person);
	if (!capture.isOpened()) { printf("--(!)Error opening video capture\n"); return -1; }

	//test!!!

	lbp_init(false);
	while (capture.read(pre_frame))
	{
		if (pre_frame.empty())
		{
			printf(" --(!) No captured frame -- Break!");
			break;
		}

		if (mFilter != NULL && mFilter->ready) {
			//pyrDown(pre_frame, frame, Size(pre_frame.cols / 2, pre_frame.rows / 2));
			frame = pre_frame;

			Mat gray;
			cvtColor(frame, gray, CV_BGR2GRAY);
			Mat lbp;
			lbp_from_gray(gray, lbp);

			mFilter->on_newFrame(&frame, lbp);

			//pyrUp(frame, frame);
		}
		else {
			//pyrDown(pre_frame, frame, Size(pre_frame.cols / 2, pre_frame.rows / 2));
			frame = pre_frame;
		}

		imshow("Video", frame);

		int c = waitKey(20);
		if ((char)c == 27) { break; } // escape

		if (stuck) {

			for (time_t t = time(0) + 8; time(0) < t; ) {}

			stuck = false;
		}
	}
	capture.release();
	cvDestroyAllWindows();

	return 0;
}
