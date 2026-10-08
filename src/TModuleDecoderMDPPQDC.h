/*
 * @file TModuleDecoderMDPPQDC.h
 * @date  Created : 2026.02.04 (T.Yano)
 *  Last Modified : 2026.02.04 (T.Yano)
 */
#ifndef TMODULEDECODERMDPPQDC_H
#define TMODULEDECODERMDPPQDC_H
#include <TModuleDecoder.h>
#include <vector>

namespace art {
  class TModuleDecoderMDPPQDC;
}

class art::TModuleDecoderMDPPQDC  : public TModuleDecoder {
  public:
    static const int kID = 39;
    static const int kChannel = 32*4;

    TModuleDecoderMDPPQDC();
    explicit TModuleDecoderMDPPQDC(Int_t id);
    virtual ~TModuleDecoderMDPPQDC();
    virtual Int_t Decode(char* buffer, const int &size, TObjArray *seg);

    static const UInt_t kHeader         = 0b01;
    static const UInt_t kMeasure        = 0b00;
    static const UInt_t kEOB            = 0b11;
    static const UInt_t kValue          = 0b01;
    static const UInt_t kSample         = 0b11;
    static const UInt_t kMaskHeader     = 0xc0000000;
    static const UInt_t kMaskSubheader  = 0x30000000;
    static const UInt_t kMaskGeometry   = 0x00ff0000;
    static const UInt_t kMaskChannel    = 0x007f0000;
    static const UInt_t kMaskMeasure    = 0x0080ffff; // Overflow bit is included
    static const UInt_t kMaskOverflow   = 0x00800000;
    static const  Int_t kShiftHeader    = 30;
    static const  Int_t kShiftSubheader = 28;
    static const  Int_t kShiftGeometry  = 16;
    static const  Int_t kShiftChannel   = 16;
    static const  Int_t kShiftMeasure   = 0;

  protected:
    std::vector<std::vector<UInt_t>> fPendingLongQDC;
    std::vector<std::vector<UInt_t>> fPendingShortQDC;
    std::vector<std::vector<UInt_t>> fPendingTDC;

  private:
    TModuleDecoderMDPPQDC(const TModuleDecoderMDPPQDC&);
    TModuleDecoderMDPPQDC& operator=(const TModuleDecoderMDPPQDC&);

    ClassDef(TModuleDecoderMDPPQDC,0) 
};
#endif // TMODULEDECODERMDPPQDC_H
