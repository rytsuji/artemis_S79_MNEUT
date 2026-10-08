/*
 * @file TModuleDecoderMDPPQDC.cc
 * @date  Created : 2026.02.04 (T.Yano)
 *  Last Modified : 2026.06.07 (T.Yano)
 */
#include "TModuleDecoderMDPPQDC.h"
#include <TRawDataTimingCharge.h>

using art::TModuleDecoderMDPPQDC;
using art::TRawDataTimingCharge;

typedef TRawDataTimingCharge MDPPQDCRaw_t;

ClassImp(TModuleDecoderMDPPQDC)

TModuleDecoderMDPPQDC::TModuleDecoderMDPPQDC()
  : TModuleDecoder(kID, MDPPQDCRaw_t::Class()) {}

TModuleDecoderMDPPQDC::TModuleDecoderMDPPQDC(Int_t id)
  : TModuleDecoder(id, MDPPQDCRaw_t::Class()) {}

TModuleDecoderMDPPQDC::~TModuleDecoderMDPPQDC() {}

Int_t TModuleDecoderMDPPQDC::Decode(char* buf, const int &size, TObjArray *seg)
{
  UInt_t *evtData = (UInt_t*) buf;
  UInt_t evtSize = size/sizeof(UInt_t);
  Int_t igeo = 0;
  Bool_t evtFlag = kFALSE;

  fPendingLongQDC.assign(32, {});
  fPendingShortQDC.assign(32, {});
  fPendingTDC.assign(32, {});

  // Structure of header
  // bits
  //  0- 9 : number of 32 bit data words
  // 10-12 : unused
  // 13-15 : TDC resolution
  // 16-23 : module ID (geo)
  //    24 : Sampling mode (SA)
  //       : (Currently this decoder does not support SA=1)
  // 25-27 : unused
  // 28-29 : subheader (0b00)
  // 30-31 : header signature (0b01)
  //
  // Structure of data
  // bits
  //  0-15 : ADC value
  // 16-22 : channel
  //       : ch  0-- 31: ADC long int.
  //       : ch 32-- 63: TDC time diff.
  //       : ch 64-- 95: (unused)
  //       : ch 96--127: ADC short int.
  //    23 : overflow
  // 24-27 : unused
  // 28-29 : subheader (0b01)
  // 30-31 : data signature (0b00)
  //
  // Structure of end of event
  //  0-29 : event counter / time stamp
  // 30-31 : end of event signature (0b11)

  for (size_t i = 0; i != evtSize; i++) {
    const UInt_t headerID = (evtData[i] & kMaskHeader) >> kShiftHeader;
    UInt_t subheaderID;

    switch (headerID) {
      case kHeader:
        igeo = (evtData[i] & kMaskGeometry) >> kShiftGeometry;
        evtFlag = kTRUE;
        break;
      case kMeasure:
        subheaderID = (evtData[i] & kMaskSubheader) >> kShiftSubheader;
        if (evtFlag && subheaderID == kValue) {
          const UInt_t ch      = (evtData[i] & kMaskChannel) >> kShiftChannel;
          const UInt_t measure = (evtData[i] & kMaskMeasure);
          if (ch < 32) {
            fPendingLongQDC[ch].push_back(measure);
          } else if (ch < 64) {
            fPendingTDC[ch - 32].push_back(measure);
          } else if (ch >= 96 && ch < 128) {
            fPendingShortQDC[ch - 96].push_back(measure);
          }
        }
        break;
      case kEOB:
        evtFlag = kFALSE;
        break;
    }
  }

  // pair TDC with long/short QDC for each physical channel
  for (Int_t ch = 0; ch < 32; ch++) {
    const Int_t nTDC      = fPendingTDC[ch].size();
    const Int_t nLongQDC  = fPendingLongQDC[ch].size();
    const Int_t nShortQDC = fPendingShortQDC[ch].size();

    if (nTDC != nLongQDC) {
      Warning("Decode", "geo=%d ch=%d: nTDC=%d nLongQDC=%d — skipping",
              igeo, ch, nTDC, nLongQDC);
      continue;
    }

    for (Int_t i = 0; i < nTDC; i++) {
      auto obj = static_cast<MDPPQDCRaw_t*>(this->New());
      obj->SetSegInfo(seg->GetUniqueID(), igeo, ch);
      obj->SetTiming(fPendingTDC[ch][i]);
      obj->SetCharge(fPendingLongQDC[ch][i]);
      seg->Add(obj);
    }

    if (nShortQDC > 0 && nShortQDC != nTDC) {
      Warning("Decode", "geo=%d ch=%d: nTDC=%d nShortQDC=%d — skipping short",
              igeo, ch, nTDC, nShortQDC);
      continue;
    }

    for (Int_t i = 0; i < nShortQDC; i++) {
      auto obj_short = static_cast<MDPPQDCRaw_t*>(this->New());
      obj_short->SetSegInfo(seg->GetUniqueID(), igeo, ch + 96);
      obj_short->SetTiming(fPendingTDC[ch][i]);
      obj_short->SetCharge(fPendingShortQDC[ch][i]);
      seg->Add(obj_short);
    }
  }

  return 0;
}
