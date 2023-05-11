//
// Copyright RIME Developers
// Distributed under the BSD License
//
// 2014-11-19 Chen Gong <chen.sst@gmail.com>
//
#ifndef RIME_DATE_TRANSLATOR_H_
#define RIME_DATE_TRANSLATOR_H_

#include <rime/common.h>
#include <rime/translator.h>
#include <rime/algo/algebra.h>

namespace rime {

class DateTranslator : public Translator {
 public:
  explicit DateTranslator(const Ticket& ticket);

    virtual an<Translation> Query(const string& input,
                                        const Segment& segment);
 // protected:
};

}  // namespace rime

#endif  // RIME_DATE_TRANSLATOR_H_
