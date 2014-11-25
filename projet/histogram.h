#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include <array>
#include <vector>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>

#include <initializer_list>
using namespace cv;
using namespace std;

/**
 * @brief encapsulation of caclHist, all of the meaning of the parameters are same as the caclHist, but these api
 * are easier to use. ex : Mat hist = calc_histogram({hsv}, {0, 1}, {180, 256}, {{ {0, 180}, {0, 256} }});
 */
void calc_histogram(initializer_list<Mat> images, OutputArray output, initializer_list<int> channels,
                    initializer_list<int> hist_sizes, initializer_list<float[2]> ranges,
                    InputArray mask = Mat(), bool uniform = true,
                    bool accumulate = false);

/**
 * @brief encapsulation of caclHist, all of the meaning of the parameters are same as the caclHist, but these api
 * are easier to use. ex : Mat hist = calc_histogram({hsv}, {0, 1}, {180, 256}, {{ {0, 180}, {0, 256} }});
 *
 * @return histogram
 */
inline
MatND calc_histogram(initializer_list<Mat> images, initializer_list<int> channels,
                         initializer_list<int> hist_sizes, initializer_list<float[2]> ranges,
                         InputArray mask = Mat(), bool uniform = true,
                         bool accumulate = false)
{
    MatND output;
    calc_histogram(images, output, channels, hist_sizes, ranges, mask, uniform, accumulate);

    return output;
}

/**
 * @brief encapsulation of caclHist, all of the meaning of the parameters are same as the caclHist, but these api
 * are easier to use. ex : Mat hist = calc_histogram({hsv}, {0, 1}, {180, 256}, {{ {0, 180}, {0, 256} }});This api
 * could reduce the times of memory allocation when compare to non-template version
 */
template<size_t N>
void calc_histogram(initializer_list<Mat> images, OutputArray output, initializer_list<int> channels,
                    initializer_list<int> hist_sizes, array<float[2], N> const &ranges,
                    InputArray mask = Mat(), bool uniform = true,
                    bool accumulate = false)
{
    array<float const*, N> d_ranges;
    for(size_t i = 0; i != N; ++i){
        d_ranges[i] = ranges[i];
    }

    calcHist(begin(images), images.size(), begin(channels), mask, output, channels.size(), begin(hist_sizes), &d_ranges[0], uniform ,accumulate);
}

template<size_t N>
void calc_histogram(initializer_list<Mat> images, OutputArray output, initializer_list<int> channels,
                    initializer_list<int> hist_sizes, initializer_list<float[2]> ranges,
                    InputArray mask = Mat(), bool uniform = true,
                    bool accumulate = false)
{
    array<float const*, N> d_ranges;
    for(size_t i = 0; i != N; ++i){
        d_ranges[i] = *(begin(ranges) + i);
    }

    calcHist(begin(images), images.size(), begin(channels), mask, output, channels.size(), begin(hist_sizes), &d_ranges[0], uniform ,accumulate);
}

/**
 * @brief encapsulation of caclHist, all of the meaning of the parameters are same as the caclHist, but these api
 * are easier to use. ex : Mat hist = calc_histogram({hsv}, {0, 1}, {180, 256}, {{ {0, 180}, {0, 256} }});This api
 * could reduce the times of memory allocation when compare to non-template version
 *
 * @return histogram
 */
template<size_t N>
inline
MatND calc_histogram(initializer_list<Mat> images, initializer_list<int> channels,
                         initializer_list<int> hist_sizes, array<float[2], N> const &ranges,
                         InputArray mask = Mat(), bool uniform = true,
                         bool accumulate = false)
{
    MatND output;
    calc_histogram<N>(images, output, channels, hist_sizes, ranges, mask, uniform, accumulate);

    return output;
}

template<size_t N>
inline
MatND calc_histogram(initializer_list<Mat> images, initializer_list<int> channels,
                         initializer_list<int> hist_sizes, initializer_list<float[2]> ranges,
                         InputArray mask = Mat(), bool uniform = true,
                         bool accumulate = false)
{
    MatND output;
    calc_histogram<N>(images, output, channels, hist_sizes, ranges, mask, uniform, accumulate);

    return output;
}


#endif // HISTOGRAM_H
