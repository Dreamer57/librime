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
// #include <rime/translation.h>
// #include <rime/gear/translator_commons.h>
#include <rime/dict/vocabulary.h>
// #include <rime/gear/charset_filter.h>
#include <boost/locale/encoding.hpp>
#include <boost/algorithm/string.hpp>

#include "often_charset_filter.h"

namespace rime {

static inline size_t unistrlen(const string& text) {
  return utf8::unchecked::distance(
      text.c_str(), text.c_str() + text.length());
}

class OftenCharsetFilterTranslation : public ExtendedCharsetFilterTranslation {
 public:
  OftenCharsetFilterTranslation(an<Translation> translation,
                              const string& charset);

 protected:
  bool FilterCandidate(an<Candidate> cand);
};

OftenCharsetFilterTranslation::OftenCharsetFilterTranslation(
    an<Translation> translation, const string& charset)
    : ExtendedCharsetFilterTranslation(translation, charset) {
  LocateNextCandidate();
}

bool OftenCharsetFilterTranslation::FilterCandidate(an<Candidate> cand) {
  if (exhausted()) {
    return false;
  }
  if (unistrlen(cand->text()) == 1) {
    return true;
  }
  return false;
}

OftenCharsetFilter::OftenCharsetFilter(const Ticket& ticket)
    : ExtendedCharsetFilter(ticket) {
  if (name_space_ == "filter") {
    name_space_ = "single_char_only";
  }
  if (Config* config = engine_->schema()->config()) {
    config->GetString(name_space_ + "/option_name", &option_name_);
  }
  if (option_name_.empty()) {
    option_name_ = "often_charset_filter";  // default switcher option
  }
}

an<Translation> OftenCharsetFilter::Apply(
    an<Translation> translation, CandidateList* candidates) {
  if (!engine_->context()->get_option(option_name_)) {  // off
    return translation;
  }
  return New<OftenCharsetFilterTranslation>(translation);
}

}  // namespace rime
