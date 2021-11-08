
#ifndef __VIDEOFRAMEANALYZE_HPP_
#define __VIDEOFRAMEANALYZE_HPP_

#include <vector>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

enum RGB_CHANNEL { B = 0, G, R };

#define MASTER_FRAME 0
#define COLOR_CHANNELED 1

class Frame
{
public:
	Frame() {}
	friend class FrameAnalyze;

	Mat master_frame;
	vector<Mat> frame;
	int color_channel[3]; // 3 channels (RGB)
};

class FrameAnalyze
{
private:
	Frame previous_frame;
	Frame current_frame;

	vector<vector<int> > red_channel_diff;
	vector<vector<int> > green_channel_diff;
	vector<vector<int> > blue_channel_diff;
	vector<vector<int> > master_frame_diff;

public:

	FrameAnalyze()
	{
		previous_frame.frame.resize(3); // allocate three frames for RGB channels
		current_frame.frame.resize(3); // allocate three frames for RGB channels

		// set the color channels of each frame
		for (unsigned int i = 0; i < current_frame.frame.size(); i++)
		{
			previous_frame.color_channel[i] = i;
			current_frame.color_channel[i] = i;
		}
	}
	void analyze_video_frame(VideoCapture video);
};

#endif
