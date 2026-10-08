#ifndef LINKDEF_USER_H
#define LINKDEF_USER_H

#ifdef __CINT__

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ class art::TCmdCheckYAML+;
#pragma link C++ class art::TCmdLoopAdd+;
#pragma link C++ class art::TCmdShowKey+;
#pragma link C++ class art::TCmdShowYAML+;
#pragma link C++ class art::TModuleDecoderMDPPQDC+;
#pragma link C++ function addq(const char*);
#pragma link C++ function addq(const char*, const char*);
#pragma link C++ function addq(const char*, const char*, const char*);
#pragma link C++ function addq(const char*, const char*, const char*, const char*);

#endif

#endif
