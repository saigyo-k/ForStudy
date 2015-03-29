#include <iostream>
#include <vector>
#include <memory>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "ColorEdge.h"

int main(int argc, char *argv[]) {
    cv::Mat_<cv::Vec3b> image(240, 320);
    image.setTo(255);

    cv::Scalar b(20.0 / 0.114, 0, 0);
    cv::Scalar g(0, 20.0 / 0.587, 0);
    cv::Scalar r(0, 0, 20.0 / 0.299);

    cv::rectangle(image, cv::Rect(0, 0, 240, 80), b, -1);
    cv::rectangle(image, cv::Rect(0, 80, 240, 80), g, -1);
    cv::rectangle(image, cv::Rect(0, 160, 240, 80), r, -1);

    cv::Mat_<uchar> gray;
    cv::cvtColor(image, gray, CV_BGR2GRAY);

    cv::Mat_<uchar> lap;
    cv::Laplacian(gray, lap, gray.depth());

    cv::Mat_<uchar> edge;
    std::unique_ptr<ColorEdge> color_edge(new ColorEdge(3, 3));
    color_edge->detectColorEdge(image, edge);

    cv::imshow("original", image);
    cv::imshow("gray", gray);
    cv::imshow("lap", lap);
    cv::imshow("edge", edge);

    cv::imwrite("original.jpg", image);
    cv::imwrite("gray.jpg", gray);
    cv::imwrite("laplacian.jpg", lap);
    cv::imwrite("ColorEdge.jpg", edge);

    cv::waitKey(0);

    return 0;
}