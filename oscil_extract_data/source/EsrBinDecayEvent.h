/* 
 * File:   EsrBinDecayEvent.h
 * Author: winckler
 *
 * Created on August 5, 2014, 4:53 PM
 */

#ifndef ESRBINDECAYEVENT_H
#define	ESRBINDECAYEVENT_H

#include "EsrSMSEvent.h"

class EsrBinDecayEvent : public EsrSMSEvent
{
public:
    
    
    EsrBinDecayEvent();
    EsrBinDecayEvent(int DecayChannel, int DataType=SIDS::kRSA51);
    //EsrBinDecayEvent(const EsrBinDecayEvent& orig);
    virtual ~EsrBinDecayEvent();
    
    
    void SetBinDecay(int DecayTime, int DecayFrequency) {fBinDecayTime=DecayTime; fBinDecayFreq=DecayFrequency;}
    void SetBinArrival(int ArrivalTime, int ArrivalFrequency){fBinArrivalTime=ArrivalTime; fBinArrivalFreq=ArrivalFrequency;}
    
    void SetBinDecayTime(int DecayTime) {fBinDecayTime=DecayTime;}
    void SetBinDecayFreq(int DecayFrequency) {fBinDecayFreq=DecayFrequency;}
    void SetBinArrivalTime(int ArrivalTime) {fBinArrivalTime=ArrivalTime;}
    void SetBinArrivalFreq(int ArrivalFrequency) {fBinArrivalFreq=ArrivalFrequency;}
    
    
    
    int GetBinDecayTime() const {return fBinDecayTime;}
    int GetBinDecayFreq() const {return fBinDecayFreq;}
    int GetBinArrivalTime() const {return fBinArrivalTime;}
    int GetBinArrivalFreq() const {return fBinArrivalFreq;}
    
protected:
    
    //Data
    int fBinDecayTime;
    int fBinDecayFreq;
    int fBinArrivalTime;
    int fBinArrivalFreq;
    
    
    //int fTimeResolutionID;
    //int fFreqResolutionID;
    

    ClassDef(EsrBinDecayEvent,1);
};


#endif	/* ESRBINDECAYEVENT_H */

