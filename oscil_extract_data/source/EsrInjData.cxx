/* 
 * File:   EsrInjData.cxx
 * Author: winckler
 * 
 * Created on August 6, 2014, 4:45 PM
 */

#include "EsrInjData.h"

EsrInjData::EsrInjData() : TNamed(),
        fFileName(), 
        fDataID(0), 
        fTimeResolution(0.), 
        fFreqResolution(0.),
        fFrequencyOffset(0.),
        fUserName(),
        fFileComment(),
        fQualityTag(0),
        fNp_init(0),
        fNp_end(0),
        fNd_init(0),
        fNd_end(0),
        fNEC(0),
        fCoolFreqP(0.),
        fCoolFreqD(0.)
{
    
}

EsrInjData::EsrInjData(TString Name, TString Title, std::string FileName, 
        int DataType, float TimeResolution, float FrequencyResolution, float FrequencyOffset) : 
        TNamed(Name,Title), 
        fFileName(FileName), 
        fDataID(DataType), 
        fTimeResolution(TimeResolution), 
        fFreqResolution(FrequencyResolution),
        fFrequencyOffset(FrequencyOffset),
        fUserName(),
        fFileComment(),
        fQualityTag(0),
        fNp_init(0),
        fNp_end(0),
        fNd_init(0),
        fNd_end(0),
        fNEC(0),
        fCoolFreqP(0.),
        fCoolFreqD(0.)
{
    
}



EsrInjData::EsrInjData(std::string FileName, int DataType, 
        float TimeResolution, float FrequencyResolution, float FrequencyOffset) : 
        TNamed(), 
        fFileName(FileName), 
        fDataID(DataType), 
        fTimeResolution(TimeResolution), 
        fFreqResolution(FrequencyResolution),
        fFrequencyOffset(FrequencyOffset),
        fUserName(),
        fFileComment(),
        fQualityTag(0),
        fNp_init(0),
        fNp_end(0),
        fNd_init(0),
        fNd_end(0),
        fNEC(0),
        fCoolFreqP(0.),
        fCoolFreqD(0.)
{
    
}


EsrInjData::~EsrInjData() 
{
    
}

void EsrInjData::SetResolution(float TimeResolution, float FrequencyResolution, float FrequencyOffset) 
{
    fTimeResolution=TimeResolution;
    fFreqResolution=FrequencyResolution;
    fFrequencyOffset=FrequencyOffset;
}


void EsrInjData::AddEvent(EsrDecayEvent Evt)
{
    int ChannelID=Evt.GetDecayChannel();
    int DataType=Evt.GetDataType();
    
    if(DataType==fDataID)
    {
        switch ( ChannelID ) 
        {

            case SIDS::kUnknownDecay : 
            {
                fUnknownDecayEvents.push_back(Evt);
                break;
            }

            case SIDS::kECDecay : 
            {
                fECDecayEvents.push_back(Evt);
                break;
            }

            case SIDS::kBetaDecay : 
            {
                fBetaDecayEvents.push_back(Evt);
                break;
            }

            case SIDS::kLossDecay: 
            {
                fBetaDecayEvents.push_back(Evt);
                break;
            }


            default:
            {
                cout<<"WARNING : Decay channel not identified"<<endl;
                fUnknownDecayEvents.push_back(Evt);
              break;
            }
        }// ChannelID switch
    }
    else
    {
        cout<<"ERROR : Try to mix decay times coming from different Detector or format ID"<<endl;
    }
}



void EsrInjData::AddEvent(EsrBinDecayEvent BinEvt)
{
    EsrDecayEvent DecayEvent(BinEvt, fTimeResolution, fFreqResolution, fFrequencyOffset);
    AddEvent(DecayEvent);
}


void EsrInjData::SetDecayData(vector<EsrDecayEvent> DecayList)
{
    for(unsigned int i(0);i<DecayList.size();i++)
        AddEvent(DecayList[i]);
}


void EsrInjData::SetDecayData(vector<EsrBinDecayEvent> BinDecayList)
{
    for(unsigned int i(0);i<BinDecayList.size();i++)
        AddEvent(BinDecayList[i]);
}

ClassImp(EsrInjData)
