#include "CmdYAMLUtil.h"

#include <TError.h>
#include <TObjArray.h>
#include <TObjString.h>

#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <string>

namespace {
   const char *kMacroBegin = "\033[1;36m";
   const char *kCommentBegin = "\033[90m";
   const char *kSearchBegin = "\033[1;30;43m";
   const char *kErrorBegin = "\033[1;31m";
   const char *kMacroEnd = "\033[0m";

   Bool_t PushToken(std::vector<TString> &tokens, TString &token)
   {
      if (!token.IsNull()) {
         tokens.push_back(token);
         token.Clear();
      }
      return kTRUE;
   }

   Bool_t IsMacroFirstChar(char c)
   {
      return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == '_';
   }

   Bool_t IsMacroChar(char c)
   {
      return IsMacroFirstChar(c) || (c >= '0' && c <= '9');
   }

   Bool_t PrintMacroIfFound(const std::string &text, std::string::size_type &i)
   {
      if (text[i] != '@') return kFALSE;

      const std::string::size_type keyBegin = i + 1;
      if (keyBegin >= text.length() || !IsMacroFirstChar(text[keyBegin])) return kFALSE;

      std::string::size_type keyEnd = keyBegin + 1;
      while (keyEnd < text.length() && IsMacroChar(text[keyEnd])) {
         ++keyEnd;
      }

      if (keyEnd >= text.length() || text[keyEnd] != '@') return kFALSE;

      std::cout << kMacroBegin << text.substr(i, keyEnd - i + 1) << kMacroEnd;
      i = keyEnd;
      return kTRUE;
   }

   Bool_t PrintYAMLLineWithHighlights(const std::string &line, const std::string &keyword)
   {
      Bool_t matched = kFALSE;

      for (std::string::size_type i = 0; i < line.length(); ++i) {
         if (!keyword.empty() && line.compare(i, keyword.length(), keyword) == 0) {
            std::cout << kSearchBegin << keyword << kMacroEnd;
            i += keyword.length() - 1;
            matched = kTRUE;
            continue;
         }

         if (PrintMacroIfFound(line, i)) continue;

         if (line[i] == '#') {
            std::cout << kCommentBegin << line.substr(i) << kMacroEnd;
            break;
         }

         std::cout << line[i];
      }
      std::cout << "\n";
      return matched;
   }
}

Bool_t art::cmd::TokenizeQuoted(const TString &line, std::vector<TString> &tokens,
                                const char *caller)
{
   TString token;
   Bool_t inQuote = kFALSE;
   char quote = '\0';
   Bool_t escape = kFALSE;

   for (Ssiz_t i = 0; i < line.Length(); ++i) {
      const char c = line[i];

      if (escape) {
         token.Append(c);
         escape = kFALSE;
         continue;
      }

      if (c == '\\') {
         escape = kTRUE;
         continue;
      }

      if (inQuote) {
         if (c == quote) {
            inQuote = kFALSE;
         } else {
            token.Append(c);
         }
         continue;
      }

      if (c == '"' || c == '\'') {
         inQuote = kTRUE;
         quote = c;
         continue;
      }

      if (c == ' ' || c == '\t') {
         PushToken(tokens, token);
         continue;
      }

      token.Append(c);
   }

   if (escape) token.Append('\\');

   if (inQuote) {
      ::Error(caller, "unterminated quote in: %s", line.Data());
      return kFALSE;
   }

   PushToken(tokens, token);
   return kTRUE;
}

Bool_t art::cmd::ReadTextFile(const TString &filename, TString &content,
                              const char *caller)
{
   std::ifstream fin(filename.Data());
   if (!fin) {
      ::Error(caller, "Cannot open %s", filename.Data());
      return kFALSE;
   }
   content.ReadFile(fin);
   return kTRUE;
}

Bool_t art::cmd::BuildReplacementMap(const std::vector<TString> &args, Int_t begin,
                                     std::map<std::string, std::string> &replace,
                                     const char *caller)
{
   for (Int_t i = begin, n = args.size(); i < n; ++i) {
      if (args[i].Contains('=')) {
         TObjArray *tokens = args[i].Tokenize('=');
         if (tokens->GetEntriesFast() != 2) {
            ::Error(caller, "replace pair contains more than one '=' : %s", args[i].Data());
            delete tokens;
            return kFALSE;
         }
         const std::string key(((TObjString*)tokens->At(0))->String().Data());
         const std::string value(((TObjString*)tokens->At(1))->String().Data());
         replace.insert(std::map<std::string,std::string>::value_type(key,value));
         delete tokens;
      } else {
         if (!args[i].IsDigit()) {
            ::Error(caller, "NUM replacement is given but contains non-digits : %s", args[i].Data());
            return kFALSE;
         }
         replace.insert(std::map<std::string,std::string>::value_type("NUM", args[i].Data()));
      }
   }

   return kTRUE;
}

void art::cmd::ApplyReplacements(TString &content,
                                 const std::map<std::string, std::string> &replace)
{
   for (std::map<std::string,std::string>::const_iterator it = replace.begin(), itend = replace.end();
        it != itend; ++it) {
      content.ReplaceAll(TString::Format("@%s@", it->first.c_str()), it->second.c_str());
   }
}

std::vector<TString> art::cmd::ExtractMacroKeys(const TString &content)
{
   std::set<std::string> keys;
   const std::string text(content.Data());

   for (std::string::size_type i = 0; i < text.length(); ++i) {
      if (text[i] != '@') continue;

      const std::string::size_type keyBegin = i + 1;
      if (keyBegin >= text.length() || !IsMacroFirstChar(text[keyBegin])) continue;

      std::string::size_type keyEnd = keyBegin + 1;
      while (keyEnd < text.length() && IsMacroChar(text[keyEnd])) {
         ++keyEnd;
      }

      if (keyEnd < text.length() && text[keyEnd] == '@') {
         keys.insert(text.substr(keyBegin, keyEnd - keyBegin));
         i = keyEnd;
      }
   }

   std::vector<TString> output;
   for (std::set<std::string>::const_iterator it = keys.begin(), itend = keys.end();
        it != itend; ++it) {
      output.push_back(it->c_str());
   }
   return output;
}

void art::cmd::PrintYAMLWithHighlights(const TString &content, const TString &keyword)
{
   const std::string text(content.Data());
   const std::string key(keyword.Data());
   std::string::size_type lineBegin = 0;
   Bool_t found = kFALSE;

   while (lineBegin <= text.length()) {
      std::string::size_type lineEnd = text.find('\n', lineBegin);
      if (lineEnd == std::string::npos) lineEnd = text.length();
      const std::string line = text.substr(lineBegin, lineEnd - lineBegin);

      if (PrintYAMLLineWithHighlights(line, key)) found = kTRUE;

      if (lineEnd == text.length()) break;
      lineBegin = lineEnd + 1;
   }

   if (!key.empty() && !found) {
      printf("%sNo lines matching '%s'%s\n", kErrorBegin, key.c_str(), kMacroEnd);
   }

   std::cout.flush();
}
