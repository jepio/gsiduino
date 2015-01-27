/* 
 * File:   EsrDecayEvent.h
 * Author: winckler
 *
 * Created on August 4, 2014, 12:13 PM
 */

#ifndef ESRDECAYEVENT_H
#define	ESRDECAYEVENT_H

#include "EsrSMSEvent.h"
#include "EsrBinDecayEvent.h"

#include <string>
#ifndef __CINT__
#include <boost/serialization/access.hpp>
#include <boost/serialization/base_object.hpp>
#endif //__CINT__


class EsrDecayEvent : public EsrSMSEvent
{
public:
    
    
    EsrDecayEvent();
    EsrDecayEvent(int DecayChannel, int DataType=SIDS::kRSA51);
    EsrDecayEvent(EsrBinDecayEvent BinEvent, float TimeResolution, float FreqResolution, float FreqOffset=244000000, bool Bincentered=true);

    virtual ~EsrDecayEvent();
    
    void SetBinCentered(bool centered=true) {fBinCentered=centered;}
    bool GetBinCentered() {return fBinCentered;}
    
    void SetBinDecayEvent(EsrBinDecayEvent BinEvent, float TimeResolution, float FreqResolution, float FreqOffset=244000000, bool Bincentered=true);
    
    void SetDecay(float DecayTime, float DecayFrequency) {fDecayTime=DecayTime; fDecayFreq=DecayFrequency;}
    void SetArrival(float ArrivalTime, float ArrivalFrequency){fArrivalTime=ArrivalTime; fArrivalFreq=ArrivalFrequency;}
    
    void SetDecayTime(float DecayTime) {fDecayTime=DecayTime;}
    void SetDecayFreq(float DecayFrequency) {fDecayFreq=DecayFrequency;}
    void SetArrivalTime(float ArrivalTime) {fArrivalTime=ArrivalTime;}
    void SetArrivalFreq(float ArrivalFrequency) {fArrivalFreq=ArrivalFrequency;}
    
    float GetDecayTime() const {return fDecayTime;}
    float GetDecayFreq() const {return fDecayFreq;}
    float GetArrivalTime() const {return fArrivalTime;}
    float GetArrivalFreq() const {return fArrivalFreq;}
    
    
    void SetDecayComment(std::string Comment){fDecayComment=Comment;}
    std::string GetDecayComment() const {return fDecayComment;}
    
    void PrintEvent();
    
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & boost::serialization::base_object<EsrSMSEvent>(*this);
        ar & fDecayTime;
        ar & fDecayFreq;
        ar & fArrivalTime;
        ar & fArrivalFreq;
        ar & fDecayComment;
        ar & fBinCentered;
    }
    
protected:
    
    #ifndef __CINT__ // for BOOST serialization
    friend class boost::serialization::access;
    #endif // __CINT__
    //Data
    float fDecayTime;
    float fDecayFreq;
    float fArrivalTime;
    float fArrivalFreq;
    
    std::string fDecayComment;
    bool fBinCentered;
    bool fPrintArrival;
    
    
    //float fTimeResolutionID;
    //float fFreqResolutionID;
    

    ClassDef(EsrDecayEvent,1);
};

#endif	/* ESRDECAYEVENT_H */

