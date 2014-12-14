//
//  Lof.h
//  LOF
//
//  Created by kenta-n on 2014/02/11.
//  Copyright (c) 2014年 kenta-n. All rights reserved.
//

#ifndef __LOF__Lof__
#define __LOF__Lof__


#include <iostream>
#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/flann/flann.hpp>

//Class to detect outliers by LOF(Local Outlier Factor)
class Lof
{
    //*****Method*****
public:
    Lof(double lof_th, int nearest_num, cv::Mat_<float> &vector);
    ~Lof();
    
    //Detect outlier by LOF
    //vector: feature descriptors
    //outlier:
    void outlierDetect(const cv::Mat_<float> &vector, std::vector<int> &outlier);
private:
    //Calculate the likelihood by LOF
    //Return: likelihood by lof
    //vector: feature descriptors
    //target_num: number of target to calculate LOF
    double calcLof(const cv::Mat_<float> &vector, int target_num);
    
    //Calculate the LRD(local reachability density)
    //Return: local reachability density
    //vector: feature descriptors
    //target_num: number of target to calculate LRD
    //nearest_neighbor: nearest neighbor of target descriptor
    double calcLrd(const cv::Mat_<float> &vector, int target_num);
    double calcLrd(const cv::Mat_<float> &vector, int target_num, std::vector<int> &nearest_neighbor);
    
    void makeIndexNearestNeighbor(const cv::Mat_<float> &vector);
    
    void detectNearestNeighbor(const cv::Mat_<float> &vector, int target_num, int nearest_num, std::vector<int> &nearest_neighbor);
    
    //*****Data*****
private:
    //*****Konst*****
    const double LOF_TH_; //lof_th: lof >= lof_th -> outlier
    const int NEAREST_NUM_; //Nearest neigbor num
    //*****Valuable*****
    cv::flann::Index nn_index_; //index for NN
};

#endif /* defined(__LOF__Lof__) */
