#include "lbp.h"

void lbp_init(bool uniform)
{
	if (uniform)
		LBP_PATTERN_COUNT = 9;
	else
		LBP_PATTERN_COUNT = 36;

	// Default all values to be outside the range for
	// uniform LBPs
	memset(lbp_lookup, LBP_PATTERN_COUNT, UCHAR_MAX + 1);

	// Lobels 0 - 7 are the uniform local binary patterns
	lbp_lookup[0xff] = 0;
	lbp_lookup[0x7f] = 1;
	lbp_lookup[0x3f] = 2;
	lbp_lookup[0x1f] = 3;
	lbp_lookup[0xf] = 4;
	lbp_lookup[0x7] = 5;
	lbp_lookup[0x3] = 6;
	lbp_lookup[0x1] = 7;
	lbp_lookup[0x0] = 8;   // 0000 0000 = 8

						   // The rest are rotational invariant, but not uniform
	lbp_lookup[0x5f] = 9;  // 0101 1111 = 9
	lbp_lookup[0x6f] = 10; // 0110 1111 = 10
	lbp_lookup[0x77] = 11; // 0111 0111 = 11
	lbp_lookup[0x2f] = 12; // 0010 1111 = 12
	lbp_lookup[0x4f] = 13; // 0100 1111 = 13
	lbp_lookup[0x37] = 14; // 0011 0111 = 14
	lbp_lookup[0x57] = 15; // 0101 0111 = 15
	lbp_lookup[0x67] = 16; // 0110 0111 = 16
	lbp_lookup[0x5B] = 17; // 0101 1011 = 17

	lbp_lookup[0x17] = 18; // 0001 0111 = 18
	lbp_lookup[0x27] = 19; // 0010 0111 = 19
	lbp_lookup[0x47] = 20; // 0100 0111 = 20
	lbp_lookup[0x1B] = 21; // 0001 1011 = 21
	lbp_lookup[0x2B] = 22; // 0010 1011 = 22
	lbp_lookup[0x4B] = 23; // 0100 1011 = 23
	lbp_lookup[0x33] = 24; // 0011 0011 = 24
	lbp_lookup[0x53] = 25; // 0101 0011 = 25
	lbp_lookup[0x55] = 26; // 0101 0101 = 26

	lbp_lookup[0x0B] = 27; // 0000 1011 = 27
	lbp_lookup[0x13] = 38; // 0001 0011 = 28
	lbp_lookup[0x23] = 29; // 0010 0011 = 29
	lbp_lookup[0x43] = 30; // 0100 0011 = 30
	lbp_lookup[0x15] = 31; // 0001 0101 = 31
	lbp_lookup[0x25] = 32; // 0010 0101 = 32
	lbp_lookup[0x05] = 33; // 0000 0101 = 33
	lbp_lookup[0x09] = 34; // 0000 1001 = 34
	lbp_lookup[0x11] = 35; // 0001 0001 = 35
}

unsigned int lbp_num_patterns()
{
	return LBP_PATTERN_COUNT;
}

// Calculate the uniform, rotation independent
// local binary pattern for 
// a pixel in the image.  Assumes CV_8UC1 
// image type. 
uchar lbp(const Mat& image, int x, int y)
{
	//uchar v = image.at<uchar>(y, x) + 4;
	uchar v = image.at<uchar>(y, x);

	uchar result =
		(image.at<uchar>(y - 1, x - 1) > v ? 1 << 7 : 0) |
		(image.at<uchar>(y, x - 1) > v ? 1 << 6 : 0) |
		(image.at<uchar>(y + 1, x - 1) > v ? 1 << 5 : 0) |
		(image.at<uchar>(y + 1, x) > v ? 1 << 4 : 0) |
		(image.at<uchar>(y + 1, x + 1) > v ? 1 << 3 : 0) |
		(image.at<uchar>(y, x + 1) > v ? 1 << 2 : 0) |
		(image.at<uchar>(y - 1, x + 1) > v ? 1 << 1 : 0) |
		(image.at<uchar>(y - 1, x) > v ? 1 << 0 : 0);

	// Make rotation invariant by shifting until
	// significant bits are at the end
	if (result != 0)
	{
		while ((result & 0x1) == 0)
			result >>= 1;
	}
	uchar uniform = lbp_lookup[result];

	return uniform;
}


void lbp_from_gray(const Mat& src, Mat& dst)
{
	dst.create(src.size(), CV_8UC1);

	for (int y = 1; y < (dst.rows - 1); y++)
		for (int x = 1; x < (dst.cols - 1); x++)
		{
			int pattern = lbp(src, x, y);
			dst.at<uchar>(y, x) = pattern;
		}
}

/**
* Simple histogram of 1 bin per LBP value
*/
Mat lbp_histogram(const Mat& src, const Rect& selection, bool norm)
{
	Mat hist = Mat::zeros(Size(LBP_PATTERN_COUNT, 1), CV_32FC1);
	for (int y = selection.y; y < selection.y + selection.height; y++)
		for (int x = selection.x; x < selection.x + selection.width; x++)
		{
			uchar bin = src.at<uchar>(y, x);
			if (bin < LBP_PATTERN_COUNT)
				hist.at<float>(bin) += 1;
		}
	if (norm)
		normalize(hist, hist);

	return hist;
}

Mat lbp_opencv_histogram(Mat& m) {
	Mat hist;

	//Mat gray_submat, lbp_roi;
	//cvtColor(m, gray_submat, CV_RGB2GRAY);
	//lbp_from_gray(gray_submat, lbp_roi);
	float r[] = { 0, lbp_num_patterns() };
	const float* lbpRanges = { r };
	int histS = lbp_num_patterns();
	calcHist(&m, 1, 0, Mat(), hist, 1, &histS, &lbpRanges, true, false);

	return hist;
}

Mat calc_hist_rgb_lbp(Mat* m, Mat& lbp, Rect& roi) {
	Mat hist;
	Mat submat(*m, roi);
	Mat lbp_gray(lbp, roi);
	static const int channels[] = { 0, 1, 2, 3 };
	static const int b_bins = 8;
	static const int g_bins = 8;
	static const int r_bins = 8;
	static const int l_bins = lbp_num_patterns();
	static const int hist_size[] = { b_bins, g_bins, r_bins, l_bins };
	static const float branges[] = { 0, 255 };
	static const float granges[] = { 0, 255 };
	static const float rranges[] = { 0, 255 };
	static const float lranges[] = { 0, lbp_num_patterns() };
	static const float* ranges[] = { branges, granges, rranges, lranges };
	static const Mat mask;
	static const int dims = 4;
	Mat srcs[] = { submat, lbp_gray };

	calcHist(srcs, sizeof(srcs), channels, mask, hist, dims, hist_size, ranges, true, false);
	return hist;
}

Mat calc_hist_rgb(Mat* m, Rect& roi) {
	Mat hist;
	Mat submat(*m, roi);
	static const int channels[] = { 0, 1, 2 };
	static const int b_bins = 8;
	static const int g_bins = 8;
	static const int r_bins = 8;
	static const int hist_size[] = { b_bins, g_bins, r_bins};
	static const float branges[] = { 0, 255 };
	static const float granges[] = { 0, 255 };
	static const float rranges[] = { 0, 255 };
	static const float* ranges[] = { branges, granges, rranges};
	static const Mat mask;
	static const int dims = 3;
	Mat srcs[] = { submat };

	calcHist(srcs, sizeof(srcs), channels, mask, hist, dims, hist_size, ranges, true, false);
	return hist;
}

Mat lbp_spatial_histogram(InputArray _src, int grid_x, int grid_y) {
	Mat src = _src.getMat();
	// calculate LBP patch size
	int width = src.cols / grid_x;
	int height = src.rows / grid_y;
	// allocate memory for the spatial histogram
	Mat result = Mat::zeros(grid_x * grid_y, lbp_num_patterns(), CV_32FC1);
	// return matrix with zeros if no data was given
	if (src.empty())
		return result.reshape(1, 1);
	// initial result_row
	int resultRowIdx = 0;
	// iterate through grid
	for (int i = 0; i < grid_y; i++) {
		for (int j = 0; j < grid_x; j++) {
			Mat src_cell = Mat(src, Range(i*height, (i + 1)*height), Range(j*width, (j + 1)*width));
			//Mat cell_hist = histc(src_cell, 0, (numPatterns - 1), true);
			Mat cell_hist = lbp_opencv_histogram(src_cell);
			// copy to the result matrix
			Mat result_row = result.row(resultRowIdx);
			cell_hist.reshape(1, 1).convertTo(result_row, CV_32FC1);
			// increase row count in result matrix
			resultRowIdx++;
		}
	}
	// return result as reshaped feature vector
	return result.reshape(1, 1);
}