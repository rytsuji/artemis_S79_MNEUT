#include "TCmdShowYAML.h"

#include "CmdYAMLUtil.h"

#include <TString.h>

#include <vector>

using art::TCmdShowYAML;

ClassImp(TCmdShowYAML)

TCmdShowYAML::TCmdShowYAML()
{
   SetName("showyaml");
   SetTitle("show a steering YAML file with replacement keys highlighted");
}

TCmdShowYAML::~TCmdShowYAML()
{
}

TCmdShowYAML* TCmdShowYAML::Instance()
{
   static TCmdShowYAML instance;
   return &instance;
}

Long_t TCmdShowYAML::Exec(TString &line)
{
   std::vector<TString> tokens;
   if (!art::cmd::TokenizeQuoted(line, tokens, "TCmdShowYAML::Exec")) return 1;
   return Cmd(tokens);
}

Long_t TCmdShowYAML::Cmd(std::vector<TString> args)
{
   if (args.size() < 2) {
      Help();
      return 1;
   }

   TString content;
   if (!art::cmd::ReadTextFile(args[1], content, "TCmdShowYAML::Cmd")) return 1;

   TString keyword;
   if (args.size() >= 3) keyword = args[2];

   art::cmd::PrintYAMLWithHighlights(content, keyword);
   return 1;
}

void TCmdShowYAML::Help()
{
   printf("Usage   : %s [file]\n", GetName());
   printf("          %s \"[file]\"\n", GetName());
   printf("          %s [file] [keyword]\n", GetName());
}
