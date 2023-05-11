//
// Copyright RIME Developers
// Distributed under the BSD License
//
// 2014-11-19 Chen Gong <chen.sst@gmail.com>
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

  virtual an<Translation> Apply(an<Translation> translation,
                                        CandidateList* candidates);
 protected:
  string option_name_;
};

}  // namespace rime

#endif  // RIME_OFTEN_CHARSET_FILTER_H_
