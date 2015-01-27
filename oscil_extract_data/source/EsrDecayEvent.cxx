/* 
 * File:   EsrDecayEvent.cxx
 * Author: winckler
 * 
 * Created on August 4, 2014, 12:13 PM
 */

#include "EsrDecayEvent.h"

EsrDecayEvent::EsrDecayEvent() : EsrSMSEvent(),
        fDecayTime(0.),
        fDecayFreq(0.),
        fArrivalTime(0.),
        fArrivalFreq(0.),
        fBinCentered(false),
        fPrintArrival(false)
{
    
}

EsrDecayEvent::EsrDecayEvent(int DecayChannel , int DataType) : EsrSMSEvent(DecayChannel,DataType),
        fDecayTime(0.),
        fDecayFreq(0.),
        fArrivalTime(0.),
        fArrivalFreq(0.),
        fBinCentered(false),
        fPrintArrival(false)
{
    
}


EsrDecayEvent::EsrDecayEvent(EsrBinDecayEvent BinEvent, float TimeResolution, float FreqResolution, float FreqOffset, bool Bincentered) 
{
    fBinCentered=Bincentered;
    fChannelId=BinEvent.GetDecayChannel();
    fDataType=BinEvent.GetDataType();
    fPrintArrival=false;
    if(fBinCentered)
    {
        fDecayTime=TimeResolution*((float)BinEvent.GetBinDecayTime()+0.5);
        fDecayFreq=FreqOffset+FreqResolution*((float)BinEvent.GetBinDecayFreq()+0.5);
        fArrivalTime=TimeResolution*((float)BinEvent.GetBinArrivalTime()+0.5);
        fArrivalFreq=FreqOffset+FreqResolution*((float)BinEvent.GetBinArrivalFreq()+0.5);
    }
    else
    {
        fDecayTime=TimeResolution*(float)BinEvent.GetBinDecayTime();
        fDecayFreq=FreqOffset+FreqResolution*(float)BinEvent.GetBinDecayFreq();
        fArrivalTime=TimeResolution*(float)BinEvent.GetBinArrivalTime();
        fArrivalFreq=FreqOffset+FreqResolution*(float)BinEvent.GetBinArrivalFreq();
    }
}

//EsrDecayEvent::EsrDecayEvent(const EsrDecayEvent& orig) {}

EsrDecayEvent::~EsrDecayEvent() 
{
    
}


void EsrDecayEvent::SetBinDecayEvent(EsrBinDecayEvent BinEvent, float TimeResolution, float FreqResolution, float FreqOffset, bool Bincentered)
{
    fBinCentered=Bincentered;
    fChannelId=BinEvent.GetDecayChannel();
    fDataType=BinEvent.GetDataType();
    
    if(fBinCentered)
    {
        fDecayTime=TimeResolution*((float)BinEvent.GetBinDecayTime()+0.5);
        fDecayFreq=FreqOffset+FreqResolution*((float)BinEvent.GetBinDecayFreq()+0.5);
        fArrivalTime=TimeResolution*((float)BinEvent.GetBinArrivalTime()+0.5);
        fArrivalFreq=FreqOffset+FreqResolution*((float)BinEvent.GetBinArrivalFreq()+0.5);
    }
    else
    {
        fDecayTime=TimeResolution*(float)BinEvent.GetBinDecayTime();
        fDecayFreq=FreqOffset+FreqResolution*(float)BinEvent.GetBinDecayFreq();
        fArrivalTime=TimeResolution*(float)BinEvent.GetBinArrivalTime();
        fArrivalFreq=FreqOffset+FreqResolution*(float)BinEvent.GetBinArrivalFreq();
    }
    
    if(fChannelId==SIDS::kBetaDecay || fChannelId==SIDS::kLossDecay)
    {
        fArrivalFreq=0.;
        fArrivalTime=0.;
    }
    
}

void EsrDecayEvent::PrintEvent()
{
    
    /*
    switch ( fDataType ) 
    {
        case SIDS::kUnknownData : 
        {
            std::cout<<"Data type : Unknown"<<std::endl;
            break;
        }

        case SIDS::kPickup : 
        {
            std::cout<<"Data type : Pickup"<<std::endl;
            break;
        }

        case SIDS::kRSA1 : 
        {
            std::cout<<"Data type : RSA type 1"<<std::endl;
            break;
        }

        case SIDS::kRSA2: 
        {
            std::cout<<"Data type : RSA type 2"<<std::endl;
            break;
        }
        
        case SIDS::kRSA3: 
        {
            std::cout<<"Data type : RSA type 3"<<std::endl;
            break;
        }

        case SIDS::kTCAP: 
        {
            std::cout<<"Data type : TCAP"<<std::endl;
            break;
        }
        
        default:
        {
            std::cout<<"WARNING : Unknown Data type"<<std::endl;
          break;
        }
    }// Data ID switch
    */
    std::cout<<std::endl;
    std::cout<<"***** Event Info *****"<<std::endl;
    switch ( fChannelId ) 
    {
        case SIDS::kUnknownDecay : 
        {
            std::cout<<"-Decay channel : Unknown"<<std::endl;
            break;
        }

        case SIDS::kECDecay : 
        {
            std::cout<<"-Decay channel : EC"<<std::endl;
            break;
        }

        case SIDS::kBetaDecay : 
        {
            std::cout<<"-Decay channel : Beta+"<<std::endl;
            break;
        }

        case SIDS::kLossDecay: 
        {
            std::cout<<"-Decay channel : Loss"<<std::endl;
            break;
        }


        default:
        {
            std::cout<<"WARNING : Decay channel not identified"<<std::endl;
          break;
        }
    }// ChannelID switch
    
    //std::cout<<std::endl;
    std::cout<<"  -decay time : "<<fDecayTime<<" (s)"<<std::endl;
    std::cout<<"  -decay frequency : "<<fDecayFreq<<" (Hz)"<<std::endl;
    if(fPrintArrival)
    {
        std::cout<<"  -Arrival time : "<<fArrivalTime<<" (s)"<<std::endl;
        std::cout<<"  -Arrival frequency : "<<fArrivalFreq<<" (Hz)"<<std::endl;
    }
}

ClassImp(EsrDecayEvent)