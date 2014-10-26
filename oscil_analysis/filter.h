#pragma once

#include <algorithm>
#include <sstream>
#include <string>
#include <locale>
#include <vector>
#include <boost/date_time/posix_time/posix_time.hpp>

using std::vector;
using std::string;
using std::stringstream;
using boost::posix_time::time_input_facet;
using boost::posix_time::ptime;

struct filter{
public:
	time_input_facet *ptrn;
	stringstream ss;
	ptime pt;
	filter(const string & time_str);
	bool process(const string& time_str);
	auto process(vector<string>& str_v) -> decltype(end(str_v));
};

