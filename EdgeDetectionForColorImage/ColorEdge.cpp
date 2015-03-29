//
// Created by kenta-n on 15/03/29.
//

#include "ColorEdge.h"

ColorEdge::ColorEdge(int param_k, int param_l)
: param_k_(param_k), param_l_(param_l)
{
}

ColorEdge::~ColorEdge() {
}

void ColorEdge::detectColorEdge(const cv::Mat_<cv::Vec3b> &image, cv::Mat_<uchar> &edge)
{
    cv::Mat_<double> edge_map(image.size());
    for(int y = 1; y < (edge_map.rows - 1); ++y)
    {
        for(int x = 1; x < (edge_map.cols - 1); ++x)
        {
            cv::Mat_<cv::Vec3b> roi(image, cv::Rect(x - 1, y - 1, 3, 3));
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

        double v = std::fabs(vec_and_dist[n - j + 1].second - sum_l);
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