#ifndef TCMDLOOPADD_H
#define TCMDLOOPADD_H

#include <TCatCmd.h>

namespace art {
   class TCmdLoopAdd;
}

class art::TCmdLoopAdd : public TCatCmd {
protected:
   TCmdLoopAdd();

public:
   virtual ~TCmdLoopAdd();

   static TCmdLoopAdd* Instance();

   virtual Long_t Exec(TString &line);
   virtual Long_t Cmd(vector<TString> args);
   virtual void Help();

   ClassDef(TCmdLoopAdd,1) // add loop with quoted filename support
};

Long_t addq(const char *filename);
Long_t addq(const char *filename, const char *arg1);
Long_t addq(const char *filename, const char *arg1, const char *arg2);
Long_t addq(const char *filename, const char *arg1, const char *arg2, const char *arg3);

#endif
