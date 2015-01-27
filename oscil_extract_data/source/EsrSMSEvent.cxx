/* 
 * File:   EsrSMSEvent.cxx
 * Author: winckler
 * 
 * Created on August 5, 2014, 4:42 PM
 */

#include "EsrSMSEvent.h"

EsrSMSEvent::EsrSMSEvent() : TObject(),
    fChannelId(SIDS::kUnknownDecay),
    fDataType(SIDS::kUnknownData)

{
    
}

EsrSMSEvent::EsrSMSEvent(int DecayChannel, int DataType) : TObject(),
    fChannelId(DecayChannel),
    fDataType(DataType)
{
    
}
//EsrSMSEvent::EsrSMSEvent(const EsrSMSEvent& orig) {}

EsrSMSEvent::~EsrSMSEvent() 
{
    
}


ClassImp(EsrSMSEvent)

