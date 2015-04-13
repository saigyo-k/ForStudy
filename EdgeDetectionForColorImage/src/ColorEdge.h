//
// Created by kenta-n on 15/03/29.
//

#ifndef _COLOREDGEDETECTION_COLOEDGE_H_
#define _COLOREDGEDETECTION_COLOEDGE_H_

#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
#include <math.h>
#include <assert.h>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
class ColorEdge {
    typedef std::pair<cv::Vec3b, double> VecDist;

public:
    explicit ColorEdge(int param_k, int param_l, int filter_size = 3);
    ~ColorEdge();
    void detectColorEdge(const cv::Mat_<cv::Vec3b> &image, cv::Mat_<uchar> &edge);

private:
    double calculateMVD(const cv::Mat_<cv::Vec3b> &roi);
    double calcDistToOtherVector(std::vector<VecDist> &vec, int num);
    double calculateDist(const cv::Vec3b &a, const cv::Vec3b &b);

    const int param_k_;
    const int param_l_;
    const int filter_size_;
};


#endif //_COLOREDGEDETECTION_COLOEDGE_H_
