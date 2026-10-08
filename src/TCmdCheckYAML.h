#ifndef TCMDCHECKYAML_H
#define TCMDCHECKYAML_H

#include <TCatCmd.h>

namespace art {
   class TCmdCheckYAML;
}

class art::TCmdCheckYAML : public TCatCmd {
public:
   TCmdCheckYAML();
   virtual ~TCmdCheckYAML();

   static TCmdCheckYAML* Instance();

   virtual Long_t Exec(TString &line);
   virtual Long_t Cmd(vector<TString> args);
   virtual void Help();

private:
   TCmdCheckYAML(const TCmdCheckYAML&); // undefined
   TCmdCheckYAML& operator=(const TCmdCheckYAML& rhs); // undefined

   ClassDef(TCmdCheckYAML,1) // check steering YAML syntax and unresolved replacement keys
};

#endif
