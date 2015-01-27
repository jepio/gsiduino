/* 
 * File:   EsrBinDecayEvent.cxx
 * Author: winckler
 * 
 * Created on August 5, 2014, 4:53 PM
 */

#include "EsrBinDecayEvent.h"

EsrBinDecayEvent::EsrBinDecayEvent() : EsrSMSEvent(),
        fBinDecayTime(0),
        fBinDecayFreq(0),
        fBinArrivalTime(0),
        fBinArrivalFreq(0)
{
    
}

EsrBinDecayEvent::EsrBinDecayEvent(int DecayChannel, int DataType) : 
        EsrSMSEvent(DecayChannel, DataType),
        fBinDecayTime(0),
        fBinDecayFreq(0),
        fBinArrivalTime(0),
        fBinArrivalFreq(0)
{
    
}

//EsrBinDecayEvent::EsrBinDecayEvent(const EsrBinDecayEvent& orig) {}

EsrBinDecayEvent::~EsrBinDecayEvent() 
{
    
}




ClassImp(EsrBinDecayEvent)