#pragma once
#include "opencv2/objdetect.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/opencv.hpp"
#include "opencv2/imgcodecs.hpp"

using namespace std;
//using namespace cv;

static uchar lbp_lookup[UCHAR_MAX + 1];
static uchar LBP_PATTERN_COUNT = 0;
	
void lbp_init(bool);
void lbp_from_gray(const cv::Mat&, cv::Mat&);
unsigned int lbp_num_patterns();
uchar lbp(const cv::Mat& image, int x, int y);
cv::Mat lbp_histogram(const cv::Mat& src, const cv::Rect& selection, bool norm);
cv::Mat lbp_opencv_histogram(cv::Mat&);
cv::Mat calc_hist_rgb_lbp(cv::Mat* m, cv::Mat& lbp, cv::Rect& roi);
cv::Mat calc_hist_rgb(cv::Mat* m, cv::Rect& roi);
cv::Mat lbp_spatial_histogram(cv::InputArray _src, int grid_x, int grid_y);