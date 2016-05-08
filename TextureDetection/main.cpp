#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <stdio.h>
#include <time.h>
#include <ctime>
#include <fstream>
#include <iomanip>

#include <sdkddkver.h>
#include <conio.h>
#include <string>

#include <WinSock2.h>
#include <Windows.h>

#include "ParticleFilter.h"


#pragma comment(lib, "Ws2_32.lib")

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

bool web_cam = false;

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

int main()
{
	VideoCapture capture;
	namedWindow("Video", 1);
	setMouseCallback("Video", CallBackFunc, NULL);

	srand(time(NULL));

	if (web_cam) {
		capture.open(persons);
		if (!capture.isOpened()) { printf("--(!)Error opening video capture\n"); return -1; }

		//test!!!
		/*capture.read(frame);
		cvtColor(frame, frame, CV_BGR2GRAY);

		Mat lbp;
		lbp_init(false);
		lbp_from_gray(frame, lbp);
		imwrite("C:/Users/TH WU/Dropbox/FYP Documents/ppt_frame.jpg", frame);
		imwrite("C:/Users/TH WU/Dropbox/FYP Documents/ppt_lbp.jpg", lbp); */


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
	}
	else {
		char eduroam_pc_ip[] = "10.89.131.94";

		char window_name[] = "Video";

		WSADATA wsaData;
		WORD version;
		int error;

		version = MAKEWORD(2, 2);
		error = WSAStartup(version, &wsaData);
		if (error == 0) {
			printf("Wsa init success!\n");
		}
		else {
			printf("Wsa init failed\n");
			return -1;
		}

		SOCKET server;
		server = socket(AF_INET, SOCK_STREAM, 0);

		SOCKADDR_IN sin;

		memset(&sin, 0, sizeof sin);

		sin.sin_family = AF_INET;
		sin.sin_addr.s_addr = inet_addr(eduroam_pc_ip);
		sin.sin_port = htons(8080);

		//use::bind to avoid wrong calling of another function
		if (::bind(server, (SOCKADDR*)&sin, sizeof sin) == SOCKET_ERROR)
		{
			printf("Cannot start server!");
			WSACleanup();
			return -1;
		}

		//Listen for incoming requests
		listen(server, SOMAXCONN);
		SOCKET client;
		int length = sizeof sin;

		while (true) {
			SOCKET client;
			client = socket(AF_INET, SOCK_STREAM, NULL);
			client = accept(server, (SOCKADDR*)&sin, &length);

			//send values
			double command1_t = -200;
			double command2_t = 300;
			double command3_t = 400;
			command1_t = htonl(command1_t);
			command2_t = htonl(command2_t);
			command3_t = htonl(command3_t);

			//Receive values
			int32_t size_t;
			int32_t info1_t;
			int32_t info2_t;

			error = recv(client, (char *)&size_t, sizeof int32_t, 0);
			error = recv(client, (char *)&info1_t, sizeof int32_t, 0);
			error = recv(client, (char *)&info2_t, sizeof int32_t, 0);

			size_t = ntohl(size_t);
			info1_t = ntohl(info1_t);
			info2_t = ntohl(info2_t);
			int size = size_t;
			int info1 = info1_t;
			int info2 = info2_t;
			printf("Received integers: \n");
			cout << info1 << " " << info2 << endl;
			cout << "Received long size: ";
			cout << size << endl;

			//Receive image frame
			//vector<char> buffer(size);
			vector<char> buffer;
			char buf[1024];
			int size_read = 0;
			while (size > 0) {
				size_read = recv(client, buf, min(size, 1024), 0);
				//buffer.data()[i] = buf;
				buffer.insert(buffer.end(), buf, buf + size_read);
				size = size - size_read;
			}

			imdecode(buffer, CV_LOAD_IMAGE_COLOR, &pre_frame);

			//Send values
			error = send(client, (char *)&command1_t, sizeof(double), 0);
			error = send(client, (char *)&command2_t, sizeof(double), 0);
			error = send(client, (char *)&command3_t, sizeof(double), 0);

			//Show the image frame
			pyrDown(pre_frame, frame);
			//imshow(window_name, frame);

			//Process
			if (mFilter != NULL && mFilter->ready) {

				Mat gray;
				cvtColor(frame, gray, CV_BGR2GRAY);
				Mat lbp;
				lbp_from_gray(gray, lbp);

				mFilter->on_newFrame(&frame, lbp);

				//pyrUp(frame, frame);
			}

			imshow(window_name, frame);

			int c = waitKey(5);
			if ((char)c == 27) { break; } // escape 
		}

		WSACleanup();
	}

	capture.release();
	cvDestroyAllWindows();

	return 0;
}
