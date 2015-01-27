// stdlib
#include <vector>
// local
#include "EsrDecayEvent.h"
#include "EsrInjData.h"
// root
#include <Riostream.h>
#include <TFile.h>
#include <TTree.h>

int main()
{
    TFile f("SidsVisualDecayResults.root");
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
