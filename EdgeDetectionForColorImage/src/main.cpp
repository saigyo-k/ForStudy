#include <iostream>
#include <vector>
#include <memory>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "ColorEdge.h"

int main(int argc, char *argv[]) {
    if (argc < 2)
    {
        std::cerr << "ColorEdgeDetection.exe filename1 filename2 ..." << std::endl;
        return 1;
    }

    std::unique_ptr<ColorEdge> color_edge(new ColorEdge(2, 2));

    std::vector<std::string> filenames(argv + 1, argv + argc);
    for(auto filename : filenames)
    {
        cv::Mat_<cv::Vec3b> image = cv::imread(filename, 1);
        if(image.empty())
        {
            std::cout << "Not Found: " << filename << std::endl;
            continue;
        }

        cv::Mat_<uchar> gray;
        cv::cvtColor(image, gray, CV_BGR2GRAY);

        cv::Mat_<uchar> lap;
        cv::Laplacian(gray, lap, gray.depth());

        cv::Mat_<uchar> edge;
        color_edge->detectColorEdge(image, edge);

        cv::threshold(edge, edge, 170, 255, CV_THRESH_TOZERO);

        cv::imshow("original", image);
        cv::imshow("gray", gray);
        cv::imshow("lap", lap);
        cv::imshow("edge", edge);

        cv::imwrite("original.jpg", image);
        cv::imwrite("gray.jpg", gray);
        cv::imwrite("laplacian.jpg", lap);
        cv::imwrite("ColorEdge.jpg", edge);

        cv::waitKey(0);
    }

    return 0;
}