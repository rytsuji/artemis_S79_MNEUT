#ifndef CMDYAMLUTIL_H
#define CMDYAMLUTIL_H

#include <TString.h>

#include <map>
#include <string>
#include <vector>

namespace art {
namespace cmd {

Bool_t TokenizeQuoted(const TString &line, std::vector<TString> &tokens,
                      const char *caller);
Bool_t ReadTextFile(const TString &filename, TString &content,
                    const char *caller);
Bool_t BuildReplacementMap(const std::vector<TString> &args, Int_t begin,
                           std::map<std::string, std::string> &replace,
                           const char *caller);
void ApplyReplacements(TString &content,
                       const std::map<std::string, std::string> &replace);
std::vector<TString> ExtractMacroKeys(const TString &content);
void PrintYAMLWithHighlights(const TString &content, const TString &keyword = "");

}
}

#endif
