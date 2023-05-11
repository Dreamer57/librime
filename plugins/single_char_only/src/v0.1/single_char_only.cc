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

#include "single_char_only.h"

namespace rime {

static inline size_t unistrlen(const string& text) {
  return utf8::unchecked::distance(
      text.c_str(), text.c_str() + text.length());
}

class SingleCharOnlyTranslation : public PrefetchTranslation {
 public:
  SingleCharOnlyTranslation(an<Translation> translation);

 protected:
  virtual bool Replenish();
};

SingleCharOnlyTranslation::SingleCharOnlyTranslation(
    an<Translation> translation)
    : PrefetchTranslation(translation) {
}

bool SingleCharOnlyTranslation::Replenish() {
  if (exhausted()) {
    return false;
  }
  CandidateQueue top;
  // CandidateQueue bottom;
  while (!translation_->exhausted()) {
    auto cand = translation_->Peek();
    auto phrase = As<Phrase>(Candidate::GetGenuineCandidate(cand));
    if (!phrase || (phrase->type() != "table" && phrase->type() != "user_table")) {
      // break;
    }
    if (unistrlen(cand->text()) == 1) {
      top.push_back(cand);
    }
    // else {
    //   bottom.push_back(cand);
    // }
    translation_->Next();
  }
  cache_.splice(cache_.end(), top);
  // cache_.splice(cache_.end(), bottom);
  return !cache_.empty();
}

SingleCharOnly::SingleCharOnly(const Ticket& ticket)
    : Filter(ticket) {
  if (name_space_ == "filter") {
    name_space_ = "single_char_only";
  }
  if (Config* config = engine_->schema()->config()) {
    config->GetString(name_space_ + "/option_name", &option_name_);
  }
  if (option_name_.empty()) {
    option_name_ = "single_char_only";  // default switcher option
  }
}

an<Translation> SingleCharOnly::Apply(
    an<Translation> translation, CandidateList* candidates) {
  if (!engine_->context()->get_option(option_name_)) {  // off
    return translation;
  }
  return New<SingleCharOnlyTranslation>(translation);
}

}  // namespace rime
