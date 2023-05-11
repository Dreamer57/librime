//
// Copyright RIME Developers
// Distributed under the BSD License
//
#ifndef RIME_OFTEN_CHARSET_FILTER_H_
#define RIME_OFTEN_CHARSET_FILTER_H_

#include <rime_api.h>
#include <rime/filter.h>
#include <rime/translation.h>
#include <rime/gear/filter_commons.h>

namespace rime {

class OftenCharsetFilter : public Filter, TagMatching {
 public:
  explicit OftenCharsetFilter(const Ticket& ticket);

  an<Translation> Apply(an<Translation> translation,
                        CandidateList* candidates) override;

  virtual bool AppliesToSegment(Segment* segment) override {
    return TagsMatch(segment);
  }
    
  static bool FilterText(const string& text,
                         const string& charset);

 protected:
  string charset_;
  string often_;
  string rare_;
  bool is_advanced_ = false;
  bool is_default_enable_ = false;
  bool is_difficult_ = false;
};

}  // namespace rime

#endif  // RIME_OFTEN_CHARSET_FILTER_H_
