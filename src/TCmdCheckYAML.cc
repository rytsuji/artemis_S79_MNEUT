#include "TCmdCheckYAML.h"

#include "CmdYAMLUtil.h"

#include <TString.h>

#include <algorithm>
#include <map>
#include <string>
#include <vector>
#include <yaml-cpp/yaml.h>

using art::TCmdCheckYAML;

ClassImp(TCmdCheckYAML)

namespace {
   const char *kOkBegin = "\033[1;32m";
   const char *kWarnBegin = "\033[1;33m";
   const char *kErrorBegin = "\033[1;31m";
   const char *kEnd = "\033[0m";

   std::vector<std::string> SplitLines(const TString &content)
   {
      std::vector<std::string> lines;
      const std::string text(content.Data());
      std::string::size_type lineBegin = 0;

      while (lineBegin <= text.length()) {
         std::string::size_type lineEnd = text.find('\n', lineBegin);
         if (lineEnd == std::string::npos) lineEnd = text.length();
         lines.push_back(text.substr(lineBegin, lineEnd - lineBegin));
         if (lineEnd == text.length()) break;
         lineBegin = lineEnd + 1;
      }

      return lines;
   }

   void PrintYAMLErrorContext(const TString &content, const YAML::Mark &mark)
   {
      if (mark.line < 0 || mark.column < 0) return;

      const std::vector<std::string> lines = SplitLines(content);
      const Int_t line = mark.line;
      const Int_t column = mark.column;
      if (line < 0 || line >= static_cast<Int_t>(lines.size())) return;

      const Int_t first = std::max(0, line - 1);
      const Int_t last = std::min(static_cast<Int_t>(lines.size()) - 1, line + 1);

      for (Int_t i = first; i <= last; ++i) {
         printf("%s%5d |%s %s\n",
                i == line ? kErrorBegin : "",
                i + 1,
                i == line ? kEnd : "",
                lines[i].c_str());
         if (i == line) {
            printf("      | ");
            for (Int_t j = 0; j < column; ++j) {
               putchar(j < static_cast<Int_t>(lines[i].size()) && lines[i][j] == '\t' ? '\t' : ' ');
            }
            printf("%s^%s\n", kErrorBegin, kEnd);
         }
      }
   }
}

TCmdCheckYAML::TCmdCheckYAML()
{
   SetName("chkyaml");
   SetTitle("check steering YAML syntax and unresolved replacement keys");
}

TCmdCheckYAML::~TCmdCheckYAML()
{
}

TCmdCheckYAML* TCmdCheckYAML::Instance()
{
   static TCmdCheckYAML instance;
   return &instance;
}

Long_t TCmdCheckYAML::Exec(TString &line)
{
   std::vector<TString> tokens;
   if (!art::cmd::TokenizeQuoted(line, tokens, "TCmdCheckYAML::Exec")) return 1;
   return Cmd(tokens);
}

Long_t TCmdCheckYAML::Cmd(std::vector<TString> args)
{
   if (args.size() < 2) {
      Help();
      return 1;
   }

   TString content;
   if (!art::cmd::ReadTextFile(args[1], content, "TCmdCheckYAML::Cmd")) return 1;

   std::map<std::string, std::string> replace;
   if (!art::cmd::BuildReplacementMap(args, 2, replace, "TCmdCheckYAML::Cmd")) return 1;
   art::cmd::ApplyReplacements(content, replace);

   Bool_t ok = kTRUE;
   const std::vector<TString> keys = art::cmd::ExtractMacroKeys(content);
   if (!keys.empty()) {
      ok = kFALSE;
      printf("%sUnresolved replacement keys in %s:%s\n", kWarnBegin, args[1].Data(), kEnd);
      for (std::vector<TString>::const_iterator it = keys.begin(), itend = keys.end();
           it != itend; ++it) {
         printf("  %s\n", it->Data());
      }
   }

   try {
      YAML::Load(content.Data());
      printf("%sYAML syntax OK: %s%s\n", kOkBegin, args[1].Data(), kEnd);
   } catch (YAML::Exception &e) {
      ok = kFALSE;
      printf("%sYAML syntax error in %s:%s\n", kErrorBegin, args[1].Data(), kEnd);
      printf("  %s\n", e.what());
      PrintYAMLErrorContext(content, e.mark);
   }

   if (ok) {
      printf("%scheckyaml OK%s\n", kOkBegin, kEnd);
   }

   return 1;
}

void TCmdCheckYAML::Help()
{
   printf("Usage   : %s [file] [key=value] [num]\n", GetName());
   printf("          %s \"[file]\" [key=value] [num]\n", GetName());
}
