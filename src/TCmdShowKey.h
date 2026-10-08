#ifndef TCMDSHOWKEY_H
#define TCMDSHOWKEY_H

#include <TCatCmd.h>

namespace art {
   class TCmdShowKey;
}

class art::TCmdShowKey : public TCatCmd {
public:
   TCmdShowKey();
   virtual ~TCmdShowKey();

   static TCmdShowKey* Instance();

   virtual Long_t Exec(TString &line);
   virtual Long_t Cmd(vector<TString> args);
   virtual void Help();

private:
   TCmdShowKey(const TCmdShowKey&); // undefined
   TCmdShowKey& operator=(const TCmdShowKey& rhs); // undefined

   ClassDef(TCmdShowKey,1) // list replacement keys in a steering YAML file
};

#endif
