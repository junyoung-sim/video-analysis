
#include <iostream>
#include <opencv2/opencv.hpp>
#include "video-frame-analyze.hpp"

using namespace std;
using namespace cv;

int main()
{
	string video_file = "test.mp4"; // requires exact location of the file
	VideoCapture cam(video_file);

	if (!cam.isOpened())
	{
		cout << "ERROR: Failed to upload video source..." << endl;
		system("pause");
		return -1;
	}

	// Disgard this message -->
	// [ INFO:0] VIDEOIO: Enabled backends(6, sorted by priority): FFMPEG(1000); MSMF(990); DSHOW(980); VFW(970); CV_IMAGES(960); CV_MJPEG(950)

	FrameAnalyze analyzer;
	analyzer.analyze_video_frame(cam);

	waitKey(1);
	system("pause");
	return 0;
}
