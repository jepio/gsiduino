#pragma once
// stdlib
#include <algorithm>
#include <sstream>
#include <string>
#include <locale>
#include <vector>
// boost
#include <boost/date_time/posix_time/posix_time.hpp>

using namespace boost::posix_time;

struct filter {
public:
    time_input_facet* ptrn;
    std::stringstream ss;
    ptime pt;
    filter(const std::string& time_str);
    bool process(const std::string& time_str);
    auto process(std::vector<std::string>& str_v) -> decltype(end(str_v));
};
