// vim: number:ai:si:ts=4:et:sw=4:st=4
// stdlib
#include <fstream>
#include <algorithm>
#include <vector>
#include <string>
#include <iostream>
#include <utility>
#include <iomanip>
// local
#include "filter.h"
// root
#include <TNtuple.h>
#include <TFile.h>
// boost
#include <boost/filesystem.hpp>
#include <boost/algorithm/string/replace.hpp>
// omp
#ifdef _OPENMP
#include <omp.h>
#endif

using std::vector;
using std::string;

vector<string> get_list(const string& dir)
{
    auto i = boost::filesystem::directory_iterator{dir};
    auto e = boost::filesystem::directory_iterator{};
    vector<string> list;
    std::for_each(i,e,[&list](boost::filesystem::directory_entry& e){
        list.emplace_back(e.path().native());
    });
    return list;
}

template <typename T>
float read_file(const T& name)
{
    std::ifstream ifs{name};
    float x,y;
    {
        string comma;
        for (int i=0;i<5;i++){
            std::getline(ifs, comma);
        }
    }
    char c;
    ifs >> x >> c >> y;
    float max = y;
    while (ifs.good()){
        if (max < y)
            max = y;
        ifs >> x >> c >> y;
    }
    ifs.close();
    return max;
}

string get_type(const string& c)
{
    return c.substr(50,3);
}

string mod_path(const string &c, int i)
{
    auto res = c;
    char pattern[] = "C0";
    pattern[1] += i;
    if (i != 1)
        boost::replace_all(res, "C1", pattern);
    return res;
}

int main()
{
    // Create vector of directories
    vector<string> dirs = {"C1", "C2", "C3", "C4"};
    std::transform(begin(dirs),end(dirs),begin(dirs),[](string &c){
        return "/hera/sids/GO2014/Oscil/" + c;
    });
    // Print directory paths
    for (auto& s: dirs)
        std::cout << s << std::endl;	
    // Get vector of files in first directory
    auto list = get_list(dirs[0]);
    string last = "/hera/sids/GO2014/Oscil/C1/C1_2014.10.12.16.00.58_ext.csv";
    filter filt{last};
    auto e = filt.process(list);
    int N = e - begin(list);
    float f[4];

    TNtuple inj{"inj", "Injection kicks","c1:c2:c3:c4"};
    TNtuple ext{"ext", "Extraction kicks","c1:c2:c3:c4"};
    TNtuple *curr;

    #pragma omp parallel
    {
        #pragma omp single
        {
#ifdef _OPENMP
            std::cout << "Have " << omp_get_num_threads() << " threads\n";
#endif
        }
    }
    std::cout << "Total files: " << N << std::endl;

    std::cout << std::fixed << std::setprecision(2) << std::setw(6)
              << std::setfill(' ') << std::right;

    #pragma omp parallel for schedule(dynamic,4) default(none) private(curr,f) shared(list,N,inj,ext,std::cout)
    for(int i = 0;i < N; ++i) {

        //Decide which ntuple to fill
        auto type = get_type(list[i]);
        if (type == "ext")
            curr = &ext;
        else if (type == "inj")
            curr = &inj;
        else
            curr = nullptr;
        f[0] = read_file(list[i]);
        for (int j = 2; j <= 4; ++j) {
            auto s = mod_path(list[i],j);
            f[j-1] = read_file(s);
        }

        if (curr){
            #pragma omp critical
            {
                curr->Fill(f);
            }
        }
        if (i % 10 == 0){
            std::cout << 100.*i/N << "%\r" << std::flush;
        }
    };

    TFile ofile{"oscilloscope.root", "recreate"};
    inj.Write();
    ext.Write();
    ofile.Close();
    std::cout << "\n" << list[0]<< std::endl;
}

