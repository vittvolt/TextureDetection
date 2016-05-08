#pragma once
#include "Particle.h"
#include "RandomGenerator.h"
#include "lbp.h"
#include <math.h>
#include <vector>
#include <thread>
#include <random>

using namespace std;
//using namespace cv;

class ParticleFilter {
public:
	const int NUMBER_OF_PARTICLES = 30;
	int step = 0;
	vector<Particle> particles;
	bool ready = false;
	default_random_engine generator;

	cv::Mat initial_frame_hist_b;
	cv::Mat initial_frame_hist_g;
	cv::Mat initial_frame_hist_r;
	cv::Mat initial_lbp_hist;
	cv::Mat initial_rgb_hist;
	cv::Mat initial_total_hist;

	int image_width, image_height;
	int tracking_window_width, tracking_window_height, initial_tracking_window_width, initial_tracking_window_height;
	double roi_width = 0;
	double roi_height = 0;

	int mean_x;
	int mean_y;
	double mean_scale = 0;

	int mean_x_in_previous_frame;
	int mean_y_in_previous_frame;

	ParticleFilter(int Width, int Height);
	ParticleFilter(int X, int Y, int Width, int Height);
	void on_newFrame(cv::Mat* m, cv::Mat&);
	void set_from_initial_frame(cv::Mat& m, cv::Mat& lbp, int x1, int y1, int x2, int y2);
	cv::Mat calculate_histogram(cv::Mat m);
	void calculate_particles_xy_mean();
	double calc_weight_for_particle(Particle* p, cv::Mat& m, cv::Mat& lbp);
	vector<double> get_weighted_distribution(vector<Particle> p);
	Particle* get_new_particle(vector<double> weighted_distribution);
	void move_particle();
	void set_image_size(int w, int h);
	void set_tracking_window(int w, int h);
	void background_task(cv::Mat* m);
	void initialization(int, int, int, int, cv::Mat&);
};