#include "filter.h"

filter::filter(const string & time_str): ptrn{new time_input_facet{1}}
{
	ptrn->format("/hera/sids/GO2014/Oscil/C1/C1_%Y.%m.%d.%H.%M.%S.csv");
	ss.imbue(std::locale(ss.getloc(), ptrn));
	ss << time_str;
	ss >> pt;
	ss.str("");
}

bool filter::process(const string& time_str)
{
	ptime temp;
	ss << time_str;
	ss >> temp;
	ss.str("");
	return temp <= pt;
}

vector<string>::iterator filter::process(vector<string>& str_v)
{
	auto e = std::remove_if(begin(str_v), end(str_v), [this](const string& str){
		return !process(str);
	});
	return e;
}