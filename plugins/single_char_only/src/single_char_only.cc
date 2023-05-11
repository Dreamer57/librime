//
// Copyright RIME Developers
// Distributed under the BSD License
//
// 2014-11-19 Chen Gong <chen.sst@gmail.com>
//
#include <utf8.h>
#include <rime/candidate.h>
#include <rime/config.h>
#include <rime/context.h>
#include <rime/engine.h>
#include <rime/schema.h>
#include <rime/translation.h>
#include <rime/gear/translator_commons.h>
#include <rime/gear/charset_filter.h>

#include "single_char_only.h"

namespace rime {

static inline size_t unistrlen(const string& text) {
  return utf8::unchecked::distance(
      text.c_str(), text.c_str() + text.length());
}

class SingleCharOnlyTranslation : public CharsetFilterTranslation {
 public:
  SingleCharOnlyTranslation(an<Translation> translation);

 protected:
  bool FilterCandidate(an<Candidate> cand);
};

SingleCharOnlyTranslation::SingleCharOnlyTranslation(
    an<Translation> translation)
    : CharsetFilterTranslation(translation) {
  LocateNextCandidate();
}

bool SingleCharOnlyTranslation::FilterCandidate(an<Candidate> cand) {
  if (exhausted()) {
    return false;
  }
  if (unistrlen(cand->text()) == 1) {
    return true;
  }
  return false;
}

SingleCharOnly::SingleCharOnly(const Ticket& ticket)
    : Filter(ticket), TagMatching(ticket) {
  if (name_space_ == "filter") {
    name_space_ = "single_char_only";
  }
  if (Config* config = engine_->schema()->config()) {
    config->GetString(name_space_ + "/option_name", &option_name_);
    config->GetBool(name_space_ + "/default_enable", &is_default_enable_);
  }
  if (option_name_.empty()) {
    option_name_ = "single_char_only";  // default switcher option
  }
}

an<Translation> SingleCharOnly::Apply(
    an<Translation> translation, CandidateList* candidates) {
  if (!engine_->context()->get_option(option_name_) &&
      !is_default_enable_) {  // off
    return translation;
  }
  return New<SingleCharOnlyTranslation>(translation);
}

}  // namespace rime
