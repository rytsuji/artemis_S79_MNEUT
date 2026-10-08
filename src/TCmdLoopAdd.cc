#include "TCmdLoopAdd.h"

#include "CmdYAMLUtil.h"

#include <TError.h>
#include <TString.h>

#include <TLoopManager.h>

#include <fstream>
#include <map>
#include <string>
#include <vector>

using art::TCmdLoopAdd;

ClassImp(TCmdLoopAdd)

namespace {
   Long_t RunAddWithArgs(std::vector<TString> args)
   {
      return TCmdLoopAdd::Instance()->Cmd(args);
   }
}

TCmdLoopAdd::TCmdLoopAdd()
{
   SetName("add");
   SetTitle("Add a new loop to art::TLoopManager with quoted filename support");
}

TCmdLoopAdd::~TCmdLoopAdd()
{
}

TCmdLoopAdd* TCmdLoopAdd::Instance()
{
   static TCmdLoopAdd instance;
   return &instance;
}

Long_t TCmdLoopAdd::Exec(TString &line)
{
   std::vector<TString> tokens;
   if (!art::cmd::TokenizeQuoted(line, tokens, "TCmdLoopAdd::Exec")) return 1;
   return Cmd(tokens);
}

Long_t TCmdLoopAdd::Cmd(std::vector<TString> args)
{
   if (args.size() < 2) {
      Help();
      return 1;
   }

   TString filename = args[1];

   std::ifstream fin(filename.Data());
   if (!fin) {
      ::Error("TCmdLoopAdd::Cmd", "Cannot open %s", filename.Data());
      return 1;
   }
   fin.close();

   std::map<std::string, std::string> replace;
   if (!art::cmd::BuildReplacementMap(args, 2, replace, "TCmdLoopAdd::Cmd")) return 1;

   art::TLoopManager::Instance()->Add(filename, &replace);

   if (replace.size() > 0) {
      ::Info("TCmdLoopAdd::Cmd", "replacement is given");
   }

   return 1;
}

void TCmdLoopAdd::Help()
{
   ::Info("TCmdLoopAdd::Help", "NAME");
   ::Info("TCmdLoopAdd::Help", " ");
   ::Info("TCmdLoopAdd::Help", "     add -- prepare event loop from steering file");
   ::Info("TCmdLoopAdd::Help", "SYNOPSIS");
   ::Info("TCmdLoopAdd::Help", "     add [file] [key1=val1] [num]");
   ::Info("TCmdLoopAdd::Help", "     add \"[file]\" [key1=val1] [num]");
   ::Info("TCmdLoopAdd::Help", " ");
   ::Info("TCmdLoopAdd::Help", "DESCRIPTION");
   ::Info("TCmdLoopAdd::Help", "     This command is compatible with TCatCmdLoopAdd.");
   ::Info("TCmdLoopAdd::Help", "     Quoted filenames are also accepted, so ROOT/readline filename completion");
   ::Info("TCmdLoopAdd::Help", "     can be used inside the quoted filename argument.");
}

Long_t addq(const char *filename)
{
   std::vector<TString> args;
   args.push_back("add");
   args.push_back(filename);
   return RunAddWithArgs(args);
}

Long_t addq(const char *filename, const char *arg1)
{
   std::vector<TString> args;
   args.push_back("add");
   args.push_back(filename);
   args.push_back(arg1);
   return RunAddWithArgs(args);
}

Long_t addq(const char *filename, const char *arg1, const char *arg2)
{
   std::vector<TString> args;
   args.push_back("add");
   args.push_back(filename);
   args.push_back(arg1);
   args.push_back(arg2);
   return RunAddWithArgs(args);
}

Long_t addq(const char *filename, const char *arg1, const char *arg2, const char *arg3)
{
   std::vector<TString> args;
   args.push_back("add");
   args.push_back(filename);
   args.push_back(arg1);
   args.push_back(arg2);
   args.push_back(arg3);
   return RunAddWithArgs(args);
}
