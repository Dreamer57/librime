//
// Copyright RIME Developers
// Distributed under the BSD License
//
// 2014-03-31 Chongyu Zhu <i@lembacon.com>
//
#include <stdint.h> // for uint32_t
#include <utf8.h>
#include <rime/candidate.h>
#include <rime/common.h>
#include <rime/config.h>
#include <rime/context.h>
#include <rime/engine.h>
#include <rime/schema.h>
#include <rime/dict/vocabulary.h>
#include <rime/gear/charset_filter.h>
#include <boost/locale/encoding.hpp>
#include <boost/algorithm/string.hpp>

#include "often_charset_filter.h"

namespace rime {

// OftenCharsetFilterTranslation

class OftenCharsetFilterTranslation : public ExtendedCharsetFilterTranslation {
 public:
  OftenCharsetFilterTranslation(an<Translation> translation,
                                   const string& charset,
                                   bool is_often,
                                   const string& ns);

 protected:
  bool FilterCandidate(an<Candidate> cand) override;


  bool FilterChar(const string& text,
                  const string& charset = "",
                  bool is_often = true,
                  bool is_emoji_enabled = false);

  an<Candidate> Peek() override;

  string charset_;
  bool is_often_;
  string ns_;
  bool is_emoji_enabled_ = false;
};

an<Candidate> OftenCharsetFilterTranslation::Peek() {
  return translation_->Peek();
  // an<Candidate> cand = translation_->Peek();
  // return New<ShadowCandidate>(
  //         cand,
  //         cand->type(),
  //         charset_,
  //         ns_);
}

OftenCharsetFilterTranslation::OftenCharsetFilterTranslation(
    an<Translation> translation, const string& charset,
    bool is_often, const string& ns)
    : ExtendedCharsetFilterTranslation(translation, charset) {
  // ParseParameters(charset);
  // LocateNextCandidate();
      charset_ = charset;
      is_often_ = is_often;
      ns_ = ns;
}

bool OftenCharsetFilterTranslation::FilterCandidate(an<Candidate> cand) {
  return FilterChar(cand->text(),
                     charset_,
                     is_often_,
                     is_emoji_enabled_);
}

bool OftenCharsetFilterTranslation::FilterChar(const string& text,
                                    const string& charset,
                                    bool is_often,
                                    bool is_emoji_enabled) {
  // 第一 FilterText 不行，因为先执行了，这样第一个繁体肯定不出来。
  // 第二 这里找到一个就next也没问题。只有第一个有问题。
  // 第三 没有实现我的需求：没有简再出繁，要做到这个效果
  //      rare 就不做判断了。取代 next
  //      1、判断第一个是不是简体，是就often，不是就rare，之后一切照旧。
  //      2、放一个容器 cache_ 判断之前的数据都放里面，修改Peek，直到用完再从上层取。
  // 这个又要改 next 又要改 Peek，就完全自己重新写吧。
  // 本来我就可以自己写的，并且自己写还简单一些的，我是想要熟悉它
  // 之前的东西，才会想着要弄个复用的。完全自己写还简单一些，会少
  // 掉很多坑，现在消耗的很多时间都可以省下了。至少可以省半天时间。
  // 我费了很多时间与精力都因为它之前的东西我不知道，我要是自己写
  // 就不用管它之前的了，也不会再有我不知道的了。
  while (!translation_->exhausted()) {
    auto cand = translation_->Peek();
    if (cand && OftenCharsetFilter::FilterText(cand->text(),
                                              charset,
                                              is_emoji_enabled))
      return true;
    translation_->Next();
  }
  set_exhausted(true);
  return false;
}

// OftenCharsetFilter

OftenCharsetFilter::OftenCharsetFilter(const Ticket& ticket)
      : ExtendedCharsetFilter(ticket) {
  if (name_space_ == "filter") {
    name_space_ = "often_charset_filter";
  }
  if (Config* config = engine_->schema()->config()) {
    config->GetString(name_space_ + "/charset", &charset_);
    config->GetString(name_space_ + "/often", &often_);
    config->GetString(name_space_ + "/rare", &rare_);
  }
  if (charset_.empty()) {
    charset_ = "gb2312";  // default switcher option
  }
  if (often_.empty()) {
    often_ = "often";  // default switcher option
  }
  if (rare_.empty()) {
    rare_ = "rare";  // default switcher option
  }
}

an<Translation> OftenCharsetFilter::Apply(
    an<Translation> translation, CandidateList* candidates) {
  if (name_space_.empty()) {
    return CharsetFilter::Apply(translation, candidates);
  }
  if (engine_->context()->get_option(often_) ||
      engine_->context()->get_option(rare_)) {
    return New<OftenCharsetFilterTranslation>(translation,
                        charset_,
                        !engine_->context()->get_option(rare_),
                        name_space_);
  }
  return translation;
}

}  // namespace rime
