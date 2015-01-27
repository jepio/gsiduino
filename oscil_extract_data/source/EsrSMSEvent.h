/* 
 * File:   EsrSMSEvent.h
 * Author: winckler
 *
 * Created on August 5, 2014, 4:42 PM
 */

#ifndef ESRSMSEVENT_H
#define	ESRSMSEVENT_H

#include "TObject.h"
#include "EsrIdList.h"
#include <iostream>

#ifndef __CINT__ // for BOOST serialization
#include <boost/serialization/access.hpp>
#include <boost/serialization/base_object.hpp>
#endif //__CINT__
//#include "FairMQLogger.h"

class EsrSMSEvent : public TObject
{
public:
    EsrSMSEvent();
    EsrSMSEvent(int DecayChannel, int DataType);
    //EsrSMSEvent(const EsrSMSEvent& orig);
    virtual ~EsrSMSEvent();
    
    void SetDecayChannel(int DecayChannel) {fChannelId=DecayChannel;}
    int GetDecayChannel() const {return fChannelId;}
    
    void SetDataType(int DataType){fDataType=DataType;}
    int GetDataType() const {return fDataType;}
    
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version) 
    {
        ar & fChannelId;
        ar & fDataType;
    } 
    
protected:
    
    #ifndef __CINT__ // for BOOST serialization
    friend class boost::serialization::access;
    #endif 
    //Flags
    int fChannelId;
    int fDataType;
    

    ClassDef(EsrSMSEvent,1);
};

#endif	/* ESRSMSEVENT_H */

