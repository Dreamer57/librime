//
// Copyright RIME Developers
// Distributed under the BSD License
//
#ifndef RIME_OFTEN_CHARSET_FILTER_H_
#define RIME_OFTEN_CHARSET_FILTER_H_

#include <rime_api.h>
#include <rime/filter.h>
#include <rime/translation.h>
#include <rime/gear/charset_filter.h>

#include "extended_charset_filter.h"

namespace rime {

class OftenCharsetFilter : public ExtendedCharsetFilter {
 public:
  explicit OftenCharsetFilter(const Ticket& ticket);

  an<Translation> Apply(an<Translation> translation,
                        CandidateList* candidates) override;

 protected:
  string charset_;
  string often_;
  string rare_;
};

}  // namespace rime

#endif  // RIME_OFTEN_CHARSET_FILTER_H_
