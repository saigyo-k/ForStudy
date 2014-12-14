//
//  LOF.cpp
//  LOF
//
//  Created by saigyo on 2014/02/11.
//  Copyright (c) 2014年 saigyo. All rights reserved.
//

#include "LOF.h"

Lof::Lof(double lof_th, int nearest_num, cv::Mat_<float> &vector)
: LOF_TH_(lof_th), NEAREST_NUM_(nearest_num),
nn_index_(cv::flann::Index(vector, cv::flann::AutotunedIndexParams()))
{

}

Lof::~Lof()
{

}

void Lof::outlierDetect(const cv::Mat_<float> &vector, std::vector<int> &outlier)
{
    for(int i = 0; i < vector.rows; ++i)
    {
        double lof_value = calcLof(vector, i);
        //std::cout << lof_value << std::endl;
        if(lof_value > LOF_TH_)
        {
            outlier.push_back(i);
        }
    }
}

double Lof::calcLof(const cv::Mat_<float> &vector, int target_num)
{
    std::vector<int> nearest_neighbor;
    double lrd_target = calcLrd(vector, target_num, nearest_neighbor);

    double lrd_sum = 0.0f;
    for(auto nn : nearest_neighbor)
    {
        double lrd_each = calcLrd(vector, nn);

        lrd_sum += lrd_each / lrd_target;
    }

    double lof_value = lrd_sum / NEAREST_NUM_;

    return lof_value;
}

double Lof::calcLrd(const cv::Mat_<float> &vector, int target_num)
{
    std::vector<int> nearest_neighbor;
    detectNearestNeighbor(vector, target_num, NEAREST_NUM_, nearest_neighbor);

    cv::Mat_<float> target_vector = vector.row(target_num);
    cv::Mat_<float> nearest_neighbor_vector = vector.row(*(nearest_neighbor.end()-1));
    double dist = cv::norm(target_vector, nearest_neighbor_vector, cv::NORM_L2);

    double sum_dist = dist * nearest_neighbor.size();

    double lrd = NEAREST_NUM_ / sum_dist;

    return lrd;
}

double Lof::calcLrd(const cv::Mat_<float> &vector, int target_num, std::vector<int> &nearest_neighbor)
{
    detectNearestNeighbor(vector, target_num, NEAREST_NUM_, nearest_neighbor);

    cv::Mat_<float> target_vector = vector.row(target_num);
    cv::Mat_<float> nearest_neighbor_vector = vector.row(*(nearest_neighbor.end()-1));
    double dist = cv::norm(target_vector, nearest_neighbor_vector, cv::NORM_L2);

    double sum_dist = dist * NEAREST_NUM_;

    double lrd = NEAREST_NUM_ / sum_dist;

    return lrd;
}

void Lof::detectNearestNeighbor(const cv::Mat_<float> &vector, int target_num, int nearest_num, std::vector<int> &nearest_neighbor)
{
    cv::Mat_<float> query = vector.row(target_num);
    cv::Mat_<int> indexies;
    cv::Mat_<float> distances;
    indexies.setTo(0);
    distances.setTo(0);
    nn_index_.knnSearch(query, indexies, distances, nearest_num, cv::flann::SearchParams());

    nearest_neighbor.clear();
    for(int x = 0; x < nearest_num; ++x)
    {
        int index_num = indexies(0, x);
        //double dist = distances(0, x);
        //std::cout << index_num << ": " << dist << std::endl;

        nearest_neighbor.push_back(index_num);
    }
}