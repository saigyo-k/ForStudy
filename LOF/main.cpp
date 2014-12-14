//
//  main.cpp
//  LOF
//
//  Created by saigyo on 2014/02/11.
//  Copyright (c) 2014年 saigyo. All rights reserved.
//

#include <iostream>
#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "Lof.h"

std::vector<cv::Point> vec_pt;

void on_mouse(int event, int x, int y, int flags, void *param = NULL)
{
    cv::Point pt;
    
    switch (event) {
        case CV_EVENT_LBUTTONDOWN:
            pt = cv::Point(x, y);
            vec_pt.push_back(pt);
            break;
        case CV_EVENT_RBUTTONDOWN:
            //no process
            break;
        default:
            //no process
            break;
    }
    
}

int main(int argc, const char * argv[])
{
    const cv::Size size(320, 240);
    cv::Mat_<cv::Vec3b> image(size);
    
    cv::namedWindow("window");
    cv::setMouseCallback("window", on_mouse, 0);
    
    while(1)
    {
        image.setTo(0);
        for(auto pt : vec_pt)
        {
            cv::circle(image, pt, 1, cv::Scalar(0, 255, 0), -1);
        }
        
        cv::imshow("window", image);
        
        int key = cv::waitKey(1);
        if(key == 'q')
        {
            break;
        }
        else if(key == 'l')
        {
            int vec_size = static_cast<int>(vec_pt.size());
            cv::Mat_<float> vec(vec_size, 2);
            vec.setTo(0);
            for(int y = 0; y < vec.rows; ++y)
            {
                for(int x = 0; x < vec.cols; ++x)
                {
                    if(x == 0) vec(y, x) = static_cast<float>(vec_pt[y].x);
                    else vec(y, x) = static_cast<float>(vec_pt[y].y);
                }
            }
            
            std::vector<int> outlier;
            const int nearest_num = 10;
            std::shared_ptr<Lof> lof(new Lof(1.6, nearest_num, vec));
            lof->outlierDetect(vec, outlier);
            for(int i = 0; i < vec_pt.size(); ++i)
            {
                bool is_outlier = !(std::find(outlier.begin(), outlier.end(), i) == outlier.end());
                if(is_outlier)
                {
                    cv::circle(image, vec_pt[i], 3, cv::Scalar(0, 0, 255), -1);
                }
                else
                {
                    cv::circle(image, vec_pt[i], 1, cv::Scalar(0, 255, 0), -1);
                }
            }
            
            cv::imshow("window", image);
            cv::waitKey(0);
        }
    }
    
    return 0;
}
