//
// Copyright RIME Developers
// Distributed under the BSD License
//
// 2014-11-19 Chen Gong <chen.sst@gmail.com>
//
#ifndef RIME_SINGLE_CHAR_ONLY_H_
#define RIME_SINGLE_CHAR_ONLY_H_

#include <rime/filter.h>
#include <rime/gear/filter_commons.h>

namespace rime {

class SingleCharOnly : public Filter, TagMatching {
 public:
  explicit SingleCharOnly(const Ticket& ticket);

  virtual an<Translation> Apply(an<Translation> translation,
                                        CandidateList* candidates);

  virtual bool AppliesToSegment(Segment* segment) override {
    return TagsMatch(segment);
  }

 protected:
  string option_name_;
  bool is_default_enable_ = false;
};

}  // namespace rime

#endif  // RIME_SINGLE_CHAR_ONLY_H_
