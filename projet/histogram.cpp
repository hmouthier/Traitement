#include "histogram.h"
#include "initializer_list"


/**
 * @brief calc_back_project : encapsulate calcHist, provide friendly api, all of the parameters meaning are same as
 * the calcHist
 */
void calc_histogram(initializer_list<Mat> images, OutputArray output, initializer_list<int> channels,
                    initializer_list<int> hist_sizes, initializer_list<float[2]> ranges,
InputArray mask, bool uniform, bool accumulate)
{
    size_t const sizes = ranges.size();
    vector<float const*> d_ranges(sizes);
    for(size_t i = 0; i != sizes; ++i){
        d_ranges[i] = *(begin(ranges) + i);
    }

    calcHist(begin(images), images.size(), begin(channels), mask, output,
                 channels.size(), begin(hist_sizes), &d_ranges[0], uniform ,accumulate);
}
