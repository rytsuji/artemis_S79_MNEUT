#include "TCmdShowKey.h"

#include "CmdYAMLUtil.h"

#include <TString.h>

#include <vector>

using art::TCmdShowKey;

ClassImp(TCmdShowKey)

TCmdShowKey::TCmdShowKey()
{
   SetName("showkey");
   SetTitle("list replacement keys in a steering YAML file");
}

TCmdShowKey::~TCmdShowKey()
{
}

TCmdShowKey* TCmdShowKey::Instance()
{
   static TCmdShowKey instance;
   return &instance;
}

Long_t TCmdShowKey::Exec(TString &line)
{
   std::vector<TString> tokens;
   if (!art::cmd::TokenizeQuoted(line, tokens, "TCmdShowKey::Exec")) return 1;
   return Cmd(tokens);
}

Long_t TCmdShowKey::Cmd(std::vector<TString> args)
{
   if (args.size() < 2) {
      Help();
      return 1;
   }

   TString content;
   if (!art::cmd::ReadTextFile(args[1], content, "TCmdShowKey::Cmd")) return 1;

   const std::vector<TString> keys = art::cmd::ExtractMacroKeys(content);
   if (keys.empty()) {
      printf("No replacement keys found in %s\n", args[1].Data());
      return 1;
   }

   for (std::vector<TString>::const_iterator it = keys.begin(), itend = keys.end();
        it != itend; ++it) {
      printf("%s\n", it->Data());
   }

   return 1;
}

void TCmdShowKey::Help()
{
   printf("Usage   : %s [file]\n", GetName());
   printf("          %s \"[file]\"\n", GetName());
}
