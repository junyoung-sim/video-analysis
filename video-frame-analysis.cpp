
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include "video-frame-analyze.hpp"
#include <cmath>
#include <thread>
#include <vector>
#include <mutex>
#include <utility>

using namespace std;
using namespace cv;

// comparing color channel previous-current frames' by each row
// set color_channel as anything when comparing two master frames (not the color channeled)
void compare_frame_data(vector<vector<int> > &frame_data_diff, Frame previous_frame, Frame current_frame, int frame_type, int row, int color_channel)
{
	int current_pixel_val = 0, previous_pixel_val = 0;

	for (int col = 0; col < current_frame.frame[color_channel].cols; col++) {

		// compute the frame pixel difference (current - previous)

		if (frame_type == COLOR_CHANNELED) {
			current_pixel_val = current_frame.frame[color_channel].at<uchar>(row, col);
			previous_pixel_val = previous_frame.frame[color_channel].at<uchar>(row, col);
		}
		else { // when comparing two master frames of current and previous
			current_pixel_val = current_frame.master_frame.at<uchar>(row, col);
			previous_pixel_val = previous_frame.master_frame.at<uchar>(row, col);
		}

		frame_data_diff[row][col] = abs(previous_pixel_val - current_pixel_val);
	}
}

void FrameAnalyze::analyze_video_frame(VideoCapture video)
{
	Mat frame;
	int frame_num = 1;

	for (;;)
	{
		video >> frame;

		if (frame.empty()) break;
		else {}

		if (frame_num == 1) {
			cout << endl << "Frame size = " << frame.rows << " x " << frame.cols << endl;

			red_channel_diff.resize(frame.rows); green_channel_diff.resize(frame.rows); 
			blue_channel_diff.resize(frame.rows); master_frame_diff.resize(frame.rows);

			for (unsigned int i = 0; i < frame.rows; i++) {
				red_channel_diff[i].resize(frame.cols);
				green_channel_diff[i].resize(frame.cols);
				blue_channel_diff[i].resize(frame.cols);
				master_frame_diff[i].resize(frame.cols);
			}
		}
		else
		{
			for (unsigned int i = 0; i < frame.rows; i++) {
				for (unsigned int j = 0; j < frame.cols; j++) {
					red_channel_diff[i][i] = 0;
					green_channel_diff[i][j] = 0;
					blue_channel_diff[i][j] = 0;
					master_frame_diff[i][j] = 0;
				}
			}

			/* move the current frame to previous and insert new frame to current */
			previous_frame.frame[R] = current_frame.frame[R].clone();
			previous_frame.frame[G] = current_frame.frame[G].clone();
			previous_frame.frame[B] = current_frame.frame[B].clone();
			previous_frame.master_frame = current_frame.master_frame.clone();
		}

		current_frame.master_frame = frame.clone();
		split(current_frame.master_frame, current_frame.frame); // split the three color channel frames

		imwrite("previous_frame.png", previous_frame.master_frame);
		imwrite("current_frame.png", current_frame.master_frame);

		if (previous_frame.frame[R].empty()) {}
		else
		{
			double red_avg_diff = 0.00;
			double green_avg_diff = 0.00;
			double blue_avg_diff = 0.00;
			double total_avg_diff = 0.00;

			double frame_diff_percentage = 0.00;

			vector<thread> t;

			// allocate a thread every time while comparing frame data for all three channels 
			for (unsigned int i = 0; i < frame.rows; i++) {
				for (unsigned int color = 0; color < 4; color++) {

					// multitask frame comparison multithreading on each row of a color channel
					if (color == R) { // RED
						t.push_back(thread(compare_frame_data, ref(red_channel_diff), previous_frame, current_frame, COLOR_CHANNELED, i, R));
					}
					else if (color == G) { // GREEN
						t.push_back(thread(compare_frame_data, ref(green_channel_diff), previous_frame, current_frame, COLOR_CHANNELED, i, G));
					}
					else if (color == B) { // BLUE
						t.push_back(thread(compare_frame_data, ref(blue_channel_diff), previous_frame, current_frame, COLOR_CHANNELED, i, B));
					}
					else if(color == 3){ // MASTER FRAME
						t.push_back(thread(compare_frame_data, ref(master_frame_diff), previous_frame, current_frame, MASTER_FRAME, i, 0));
					}
				}
			}

			for (unsigned int i = 0; i < t.size(); i++) t[i].join();

			for (unsigned int row = 0; row < frame.rows; row++)
			{
				for (unsigned int col = 0; col < frame.cols; col++)
				{
					red_avg_diff += red_channel_diff[row][col];
					green_avg_diff += green_channel_diff[row][col];
					blue_avg_diff += blue_channel_diff[row][col];
				}
			}

			red_avg_diff /= frame.rows * frame.cols; // compute the average red channel difference
			green_avg_diff /= frame.rows * frame.cols; // compute the average green channel difference
			blue_avg_diff /= frame.rows * frame.cols; // compute the average blue channel difference

			cout << "Frame #" << frame_num << "Channel Diff. = [" << "R = " << red_avg_diff << ", " << "G = " << green_avg_diff << ", "
				<< "B = " << blue_avg_diff << "] ------> Overall Frame Difference: " << total_avg_diff << endl;
		}

		frame_num++;
	}
}
