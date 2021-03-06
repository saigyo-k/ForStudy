//
// Created by kenta-n on 15/03/29.
//

#include "ColorEdge.h"

ColorEdge::ColorEdge(int param_k, int param_l, int filter_size)
: param_k_(param_k), param_l_(param_l), filter_size_(filter_size)
{
    assert(filter_size >= 3 && filter_size % 2 == 1);
    assert(param_k < (filter_size * filter_size));
    assert(param_l < (filter_size * filter_size));
}

ColorEdge::~ColorEdge() {
}

void ColorEdge::detectColorEdge(const cv::Mat_<cv::Vec3b> &image, cv::Mat_<uchar> &edge)
{
    cv::Mat_<double> edge_map(image.size());
    const int filter_half = static_cast<int>(filter_size_ / 2);

    for(int y = filter_half; y < (edge_map.rows - filter_half); ++y)
    {
        for(int x = filter_half; x < (edge_map.cols - filter_half); ++x)
        {
            cv::Mat_<cv::Vec3b> roi(image, cv::Rect(x - filter_half, y - filter_half, filter_size_, filter_size_));
            edge_map(y, x) = calculateMVD(roi);
        }
    }
    
    edge_map.convertTo(edge, edge.type());
}

double ColorEdge::calculateMVD(const cv::Mat_<cv::Vec3b> &roi) {
    std::vector<std::pair<cv::Vec3b, double>> vec_and_dist;
    for(int y = 0; y < roi.rows; ++y)
    {
        for(int x = 0; x < roi.cols; ++x)
        {
            vec_and_dist.push_back(std::pair<cv::Vec3b, double>(roi(y, x), 0.0));
        }
    }

    for(int i = 0; i < vec_and_dist.size(); ++i)
    {
        double value = calcDistToOtherVector(vec_and_dist, i);
        vec_and_dist[i].second = value;
    }

    std::sort(vec_and_dist.begin(), vec_and_dist.end(),
            [](std::pair<cv::Vec3b, double> a, std::pair<cv::Vec3b, double> b)
            {
                return (a.second < b.second);
            });

    int n = vec_and_dist.size();
    std::vector<double> values;
    for(int j = 0; j < param_k_; ++j)
    {
        double sum_l = 0.0;
        for(int i = 0; i < param_l_; ++i)
        {
            sum_l += vec_and_dist[i].second / param_l_;
        }

        double v = std::fabs(vec_and_dist[n - j - 1].second - sum_l);
        values.push_back(v);
    }

    auto min = std::min_element(values.begin(), values.end());
    return (*min);
}

inline double ColorEdge::calcDistToOtherVector(std::vector<VecDist> &vec, int num)
{
    double value = 0.0;
    for(int i = 0; i < vec.size(); ++i)
    {
        if(num != i)
        {
            value += calculateDist(vec[i].first, vec[num].first);
        }
    }

    return value;
}

inline double ColorEdge::calculateDist(const cv::Vec3b &a, const cv::Vec3b &b)
{
    double value = 0.0;
    for(int i = 0; i < 3; ++i)
    {
        value += (a[i] - b[i]) * (a[i] - b[i]);
    }

    return sqrt(value);
}