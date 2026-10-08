#ifndef TCMDSHOWYAML_H
#define TCMDSHOWYAML_H

#include <TCatCmd.h>

namespace art {
   class TCmdShowYAML;
}

class art::TCmdShowYAML : public TCatCmd {
public:
   TCmdShowYAML();
   virtual ~TCmdShowYAML();

   static TCmdShowYAML* Instance();

   virtual Long_t Exec(TString &line);
   virtual Long_t Cmd(vector<TString> args);
   virtual void Help();

private:
   TCmdShowYAML(const TCmdShowYAML&); // undefined
   TCmdShowYAML& operator=(const TCmdShowYAML& rhs); // undefined

   ClassDef(TCmdShowYAML,1) // show a steering YAML file with keys highlighted
};

#endif
