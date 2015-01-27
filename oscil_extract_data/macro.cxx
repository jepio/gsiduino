// stdlib
#include <vector>
#include <fstream>
#include <string>
#include <stdexcept>
// local
#include "EsrDecayEvent.h"
#include "EsrInjData.h"
// root
#include <TFile.h>
#include <TTree.h>

bool file_exists(std::string const& filename)
{
    std::fstream file{filename};
    return file.good();
}

int main(int argc, char** argv)
{
    if (argc != 2) {
        throw std::runtime_error{"File not passed as cmdline argument."};
    }
    std::string filename = argv[1];
    if (!file_exists(filename)) {
        throw std::invalid_argument{"File doesn't exist"};
    }
    TFile f(filename.c_str());
    TTree *t = static_cast<TTree*>(f.Get("SIDSdecayData"));
    EsrInjData* data = nullptr;
    t->SetBranchAddress("EsrInjData.", &data);

    for (int i = 0, N = t->GetEntries(); i < N; ++i){
        t->GetEntry(i);
        auto decays = data->GetECData();
        int j = 0;
        for (auto& decay: decays) {
            cout << i << " " << j++ << " " << decay.GetDecayTime() << endl;
        }
    }
    f.Close();
}
