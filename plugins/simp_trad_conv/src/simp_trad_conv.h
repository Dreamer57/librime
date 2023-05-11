//
// Copyright RIME Developers
// Distributed under the BSD License
//
// 2011-12-12 GONG Chen <chen.sst@gmail.com>
//
#ifndef RIME_SIMP_TRAD_CONV_H_
#define RIME_SIMP_TRAD_CONV_H_

#include <rime/filter.h>
#include <rime/algo/algebra.h>
#include <rime/gear/filter_commons.h>

namespace rime {

class Opencc2;

class SimpTradConv : public Filter, TagMatching {
 public:
  explicit SimpTradConv(const Ticket& ticket);

  virtual an<Translation> Apply(an<Translation> translation,
                                CandidateList* candidates);


  virtual bool AppliesToSegment(Segment* segment) {
    return TagsMatch(segment);
  }

  bool Convert(const an<Candidate>& original,
               CandidateQueue* result);

  static bool IsTrad(const string& text);

  static bool FilterText(const string& text,
                        const string& charset);

 protected:
  enum TipsLevel { kTipsNone, kTipsChar, kTipsAll };

  void Initialize();
  void PushBack(const an<Candidate>& original,
                CandidateQueue* result,
                const string& simpTradConv);

  bool initialized_ = false;
  the<Opencc2> opencc_;
  // settings
  TipsLevel tips_level_ =  kTipsNone;
  string option_name_;
  string opencc_config_;
  set<string> excluded_types_;
  bool show_in_comment_ = false;
  Projection comment_formatter_;
  bool first_ = false;
  bool is_first_trad_ = false;
  bool is_first_emoji_replace_ = false;
  bool is_default_enable_ = false;
  bool random_ = false;
};

}  // namespace rime

#endif  // RIME_SIMP_TRAD_CONV_H_
