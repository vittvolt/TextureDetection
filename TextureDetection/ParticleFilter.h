#pragma once
#include "Particle.h"
#include "RandomGenerator.h"
#include <math.h>
#include <vector>
#include <thread>
#include "opencv2/objdetect.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

class ParticleFilter {
public:
	const int NUMBER_OF_PARTICLES = 180;
	int step = 0;
	vector<Particle> particles;

	Mat initial_frame_hist_b;
	Mat initial_frame_hist_g;
	Mat initial_frame_hist_r;

	int image_width, image_height;
	int tracking_window_width, tracking_window_height, initial_tracking_window_width, initial_tracking_window_height;
	int feature_point_count = 0;
	int feature_point_count_l = 0;
	int feature_point_count_s = 0;

	int mean_x;
	int mean_y;

	int mean_x_in_previous_frame;
	int mean_y_in_previous_frame;

	ParticleFilter(int Width, int Height);
	ParticleFilter(int X, int Y, int Width, int Height);
	void on_newFrame(Mat* m);
	void set_from_initial_frame(Mat m, int x1, int y1, int x2, int y2);
	Mat calculate_histogram(Mat m);
	void calculate_particles_xy_mean();
	double calc_weight_for_particle(Particle* p, Mat m);
	vector<double> get_weighted_distribution(vector<Particle> p);
	Particle* get_new_particle(vector<double> weighted_distribution);
	void move_particle();
	void set_image_size(int w, int h);
	void set_tracking_window(int w, int h);
	void background_task(Mat* m);
};