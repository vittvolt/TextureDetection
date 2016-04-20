#pragma once
#include "opencv2/objdetect.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/opencv.hpp"
#include "opencv2/imgcodecs.hpp"

using namespace std;
using namespace cv;

static uchar lbp_lookup[UCHAR_MAX + 1];
static uchar LBP_PATTERN_COUNT = 0;
	
void lbp_init(bool);
void lbp_from_gray(const Mat&, Mat&);
unsigned int lbp_num_patterns();
uchar lbp(const Mat& image, int x, int y);
Mat lbp_histogram(const Mat& src, const Rect& selection, bool norm);
Mat lbp_opencv_histogram(Mat&);
Mat calc_hist_rgb_lbp(Mat* m, Mat& lbp, Rect& roi);