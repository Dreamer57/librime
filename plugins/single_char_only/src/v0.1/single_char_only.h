//
// Copyright RIME Developers
// Distributed under the BSD License
//
// 2014-11-19 Chen Gong <chen.sst@gmail.com>
//
#ifndef RIME_SINGLE_CHAR_ONLY_H_
#define RIME_SINGLE_CHAR_ONLY_H_

#include <rime/filter.h>

namespace rime {

class SingleCharOnly : public Filter {
 public:
  explicit SingleCharOnly(const Ticket& ticket);

  virtual an<Translation> Apply(an<Translation> translation,
                                        CandidateList* candidates);
 protected:
  string option_name_;
};

}  // namespace rime

#endif  // RIME_SINGLE_CHAR_ONLY_H_
