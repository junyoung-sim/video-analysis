# Video Frame Analysis (High Motion Detector Algorithm)
------------------------------------------------------------------------

This is an implementation of detecting motion in a video stream 
by comparing separate frames in a video using C++ and OpenCV.

------------------------------------------------------------------------

# How to compile

Depending on how OpenCV is built in your system, the way to compile
this code will vary.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
g++ $(pkg-config --cflags --libs opencv) -std=c++11 video-frame-analysis.cpp main.cpp -o exec
./exec
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

------------------------------------------------------------------------

#What does it do?

This algorithm compares every single frame in a video stream,
then compare how much the frame has changed from the previous frame.
The algorithm computes the frame difference by computing how much every
single pixel (for all RGB color channels) in a frame has changed. 

------------------------------------------------------------------------

#Is it useful?

Well, possibly. As the algorithm computes the overall frame difference,
high frame difference values can suggest that a major movement has been
caused in the video file. This technology can be used to detect motions
in a surveillance monitoring system, miltary image processing, and
even some video editing automation. 

------------------------------------------------------------------------
