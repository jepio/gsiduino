/* 
 * File:   EsrInjData.h
 * Author: winckler
 *
 * Created on August 6, 2014, 4:45 PM
 */

#ifndef ESRINJDATA_H
#define	ESRINJDATA_H

#include <iostream>
#include <vector>
#include <string>

#ifndef __CINT__
#include <boost/serialization/access.hpp>
#include <boost/serialization/base_object.hpp>
#endif //__CINT__

#include "TNamed.h"
#include "TString.h"

#include "EsrBinDecayEvent.h"
#include "EsrDecayEvent.h"
#include "EsrIdList.h"

using namespace std;

class EsrInjData : public TNamed
{
public:
    EsrInjData();
    //EsrInjData(TString Name, TString Title, TString FileName, int DataType=SIDS::kRSA1);
    
    EsrInjData(TString Name, TString Title, std::string FileName, int DataType, float TimeResolution, float FrequencyResolution, float FrequencyOffset=244000000.);
    EsrInjData(std::string FileName, int DataType, float TimeResolution, float FrequencyResolution, float FrequencyOffset=244000000.);

    virtual ~EsrInjData();
    
    /// frequency and time resolution
    void SetResolution(float TimeResolution, float FrequencyResolution, float FrequencyOffset);
    float GetTimeResolution() {return fTimeResolution;}
    float GetFreqResolution() {return fFreqResolution;}
    float GetFreqOffset() {return fFrequencyOffset;}
    
    ///File name corresponding to the decays 
    void SetFileName(std::string Filename) {fFileName=Filename;}
    std::string GetFileName() const {return fFileName;}
    
    /// handle decay events
    void AddEvent(EsrDecayEvent Evt);
    void AddEvent(EsrBinDecayEvent BinEvt);
    
    void SetDecayData(vector<EsrDecayEvent> DecayList);
    void SetDecayData(vector<EsrBinDecayEvent> BinDecayList);
    
    vector<EsrDecayEvent> GetECData() const {return fECDecayEvents;}
    vector<EsrDecayEvent> GetBetaData() const {return fBetaDecayEvents;}
    vector<EsrDecayEvent> GetLossData() const {return fLossDecayEvents;}
    vector<EsrDecayEvent> GetUnknownData() const {return fUnknownDecayEvents;}
    
    // additional information
    void SetUserName(std::string UserName){fUserName=UserName;}
    std::string GetUserName() const {return fUserName;}
    
    void SetFileComment(std::string FileComment){fFileComment=FileComment;}
    std::string GetFileComment() const {return fFileComment;}
    
    void SetQualityTag(int QualityTag){fQualityTag=QualityTag;}
    int GetQualityTag() const {return fQualityTag;}
    
    
    
    void SetNParentInit(int N) {fNp_init=N;}
    int GetNParentInit() {return fNp_init;}
    
    void SetNParentEnd(int N) {fNp_end=N;}
    int GetNParentEnd() {return fNp_end;}
    
    void SetNPDaughterInit(int N) {fNd_init=N;}
    int GetNPDaughterInit() {return fNd_init;}
    
    void SetNDaughterEnd(int N) {fNd_end=N;}
    int GetNDaughterEnd() {return fNd_end;}
    
    void SetNEC(int N) {fNEC=N;}
    int GetNEC() {
        fNEC=(int)fECDecayEvents.size();
        return fNEC;}
    
    void SetCoolParentFreq(float f) {fCoolFreqP=f;}
    float GetCoolParentFreq() {return fCoolFreqP;}
    
    void SetCoolDaughterFreq(float f) {fCoolFreqD=f;}
    float GetCoolDaughterFreq() {return fCoolFreqD;}
    
    
    
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version) 
    {
        ar & fFileName;
        ar & fDataID;
        ar & fTimeResolution;
        ar & fFreqResolution;
        ar & fFrequencyOffset;
        
        ar & fUserName;
        ar & fFileComment;
        ar & fQualityTag;

        ar & fNp_init;
        ar & fNp_end;
        ar & fNd_init;
        ar & fNd_end;
        ar & fNEC;
        ar & fCoolFreqP;
        ar & fCoolFreqD;
        
        ar & fECDecayEvents;
        ar & fBetaDecayEvents;
        ar & fLossDecayEvents;
        ar & fUnknownDecayEvents;
    }
    
    
protected:
    
    #ifndef __CINT__ // for BOOST serialization
    friend class boost::serialization::access;
    //std::string fstdFileName;
    #endif // for BOOST serialization
    std::string fFileName;
    int fDataID;
    float fTimeResolution;
    float fFreqResolution;
    float fFrequencyOffset;

    
    std::string fUserName;
    std::string fFileComment;
    int fQualityTag;
    
    int fNp_init;
    int fNp_end;
    int fNd_init;
    int fNd_end;
    int fNEC;
    float fCoolFreqP;
    float fCoolFreqD;
    
    vector<EsrDecayEvent> fECDecayEvents;
    vector<EsrDecayEvent> fBetaDecayEvents;
    vector<EsrDecayEvent> fLossDecayEvents;
    vector<EsrDecayEvent> fUnknownDecayEvents;
    
    ClassDef(EsrInjData,1);
};

#endif	/* ESRINJDATA_H */

