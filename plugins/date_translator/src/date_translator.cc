//
// Copyright RIME Developers
// Distributed under the BSD License
//
// 2014-11-19 Chen Gong <chen.sst@gmail.com>
//
// #include <iostream>
#include <time.h>
#include <boost/algorithm/string.hpp>
#include <rime/candidate.h>
#include <rime/engine.h>
#include <rime/schema.h>
#include <rime/segmentation.h>
#include <rime/ticket.h>
#include <rime/translation.h>
#include <rime/algo/syllabifier.h>
#include <rime/dict/dictionary.h>
#include <rime/gear/translator_commons.h>
#include <rime/gear/table_translator.h>

#include "date_translator.h"

namespace rime {

class DateTranslation : public Translation {
 public:
  DateTranslation(const string& input,
    const Segment& segment);

  bool Next();
  an<Candidate> Peek();

 protected:
  bool DateCandidate(an<Candidate> cand);

  void InitDateCandidate(const string& input,
                          const Segment& segment);

  bool GenerateDateCandidate(const string& text,
      const string& comment, const Segment& segment);

  string GetDateByFormat(const string& format);

  string GetWeekName(const string& week, const size_t& type = 1);

  string GetWeekCnNum(const string& week);

  string GetHourName(const size_t& hour);

  string GetHourTitle(const size_t& hour);

  CandidateQueue cache_;
};

bool DateTranslation::Next() {
  if (exhausted()) {
    return false;
  }
  if (!cache_.empty()) {
    cache_.pop_front();
  }
  if (cache_.empty()) {
    set_exhausted(true);
  }
  return true;
}

an<Candidate> DateTranslation::Peek() {
  if (exhausted()) {
    return nullptr;
  }
  if (!cache_.empty()) {
    return cache_.front();
  }
  return nullptr;
}

DateTranslation::DateTranslation(
    const string& input,
    const Segment& segment) {
    InitDateCandidate(input,segment);
}

void DateTranslation::InitDateCandidate(
    const string& input,
    const Segment& segment) {

  // 加 if 记得同时加在下面的 Query
  // 加 if 记得同时加在下面的 Query
  // 加 if 记得同时加在下面的 Query

  if (input == "date") {
    GenerateDateCandidate(GetDateByFormat("%Y年%m月%d日"), string(), segment);
    GenerateDateCandidate(GetDateByFormat("%Y.%m.%d"), string(), segment);
    GenerateDateCandidate(GetDateByFormat("%Y年%m月%d日 %H点%M分"), string(), segment);
    // GenerateDateCandidate(GetDateByFormat("%Y年%m月%d日 %H点%M分%S秒"), "年-月-日 时:分:秒", segment);
  } else if (input == "time") {
    GenerateDateCandidate(GetDateByFormat("%H点%M分"), string(), segment);
    GenerateDateCandidate(GetDateByFormat("%H:%M:%S"), string(), segment);
    GenerateDateCandidate(GetDateByFormat("%H点%M分%S秒"), string(), segment);
  } else if (input == "week") {
    GenerateDateCandidate(GetWeekName(GetDateByFormat("%w")), string(), segment);
    GenerateDateCandidate(GetWeekName(GetDateByFormat("%w"), 2), string(), segment);
    // GenerateDateCandidate(GetDateByFormat("%w"), string(), segment);
    // GenerateDateCandidate(GetWeekName(atoi(GetDateByFormat("%w").c_str())), string(), segment);
  } else if (input == "now") {
    size_t hours;
    size_t minute;
    std::ostringstream buffer;
    std::ostringstream bufferCool;

    hours = atoi(GetDateByFormat("%H").c_str());
    minute = atoi(GetDateByFormat("%M").c_str());

    buffer << GetDateByFormat("%Y年%m月%d日");
    buffer << " ";
    buffer << GetWeekName(GetDateByFormat("%w"));
    buffer << " ";

    bufferCool << buffer.str();

    buffer << GetHourTitle(hours);

    buffer << " ";
    buffer << GetDateByFormat("%H点%M分");

    if (minute >= 15 && (minute < 45 || hours >= 23)) {
      bufferCool << GetHourTitle(hours);
      bufferCool << " ";
      bufferCool << GetHourName(hours);
      bufferCool << "半";
    } else if (minute >= 45 && hours < 23) {
      bufferCool << GetHourTitle(hours + 1);
      bufferCool << " ";
      bufferCool << GetHourName(hours + 1);
    } else {
      bufferCool << GetHourTitle(hours);
      bufferCool << " ";
      bufferCool << GetHourName(hours);
    }

    GenerateDateCandidate(buffer.str(), string(), segment);
    GenerateDateCandidate(bufferCool.str(), string(), segment);
  }
}

string DateTranslation::GetDateByFormat(const string& format) {
  time_t t = time(0);
  char tmp[32] = { NULL };
  strftime(tmp, sizeof(tmp), format.c_str(), localtime(&t));
  string date(tmp);
  return tmp;
}

bool DateTranslation::GenerateDateCandidate(
      const string& text,
      const string& comment, const Segment& segment) {
  cache_.push_back(New<SimpleCandidate>("date",
                    segment.start,
                    segment.end,
                    text,
                    comment));
}

string DateTranslation::GetWeekName(const string& week
                    , const size_t& type) {
  string week_name;
  if (type == 2)
    week_name = "星期";
  else
    week_name = "周";
  
  return week_name + GetWeekCnNum(week);
}

string DateTranslation::GetWeekCnNum(const string& week) {
  string week_cn_num;
  // if (week == "7") week = "0";  // const 不能改值
  if (week == "0" || week == "7")
    week_cn_num = "日";
  else if (week == "1")
    week_cn_num = "一";
  else if (week == "2")
    week_cn_num = "二";
  else if (week == "3")
    week_cn_num = "三";
  else if (week == "4")
    week_cn_num = "四";
  else if (week == "5")
    week_cn_num = "五";
  else if (week == "6")
    week_cn_num = "六";
  return week_cn_num;
}

string DateTranslation::GetHourName(const size_t& hour) {
  string hour_name;
  // if (hour > 23) hour = 0;
  if (hour == 0 || hour == 12 || hour > 23)
    hour_name = "十二点";
  else if (hour == 1 || hour == 13)
    hour_name = "一点";
  else if (hour == 2 || hour == 14)
    hour_name = "二点";
  else if (hour == 3 || hour == 15)
    hour_name = "三点";
  else if (hour == 4 || hour == 16)
    hour_name = "四点";
  else if (hour == 5 || hour == 17)
    hour_name = "五点";
  else if (hour == 6 || hour == 18)
    hour_name = "六点";
  else if (hour == 7 || hour == 19)
    hour_name = "七点";
  else if (hour == 8 || hour == 20)
    hour_name = "八点";
  else if (hour == 9 || hour == 21)
    hour_name = "九点";
  else if (hour == 10 || hour == 22)
    hour_name = "十点";
  else if (hour == 11 || hour == 23)
    hour_name = "十一点";
  return hour_name;
}

string DateTranslation::GetHourTitle(const size_t& hour) {
  string hour_title;
  // if (hour > 23) hour = 0;
  if (hour >= 6 && hour < 9)
      hour_title = "早晨";
    else if (hour >= 9 && hour < 12)
      hour_title = "上午";
    else if (hour >= 12 && hour < 14)
      hour_title = "中午";
    else if (hour >= 14 && hour < 18)
      hour_title = "下午";
    else if (hour >= 18 && hour < 19)
      hour_title = "傍晚";
    else if (hour >= 19 && hour < 23)
      hour_title = "晚上";
    else if (hour >= 23 && hour < 24)
      hour_title = "深夜";
    else if ((hour >= 0 && hour < 4) || hour >= 24)
      hour_title = "午夜";
    else if (hour >= 4 && hour < 6)
      hour_title = "凌晨";
  return hour_title;
}


DateTranslator::DateTranslator(const Ticket& ticket)
    : Translator(ticket) {
  // if (ticket.name_space == "translator") {
  //   name_space_ = "reverse_lookup";
  // }
  // if (!ticket.schema)
  //   return;
  // Config* config = ticket.schema->config();
  // config->GetString(name_space_ + "/tag", &tag_);
}

an<Translation> DateTranslator::Query(const string& input,
                                      const Segment& segment) {
  // if (!segment.HasTag(tag_))
  //   return nullptr;
  
  // const string& preedit(input);

  if (input == "date" ||
      input == "time" ||
      input == "week" ||
      input == "now") {
    return New<DateTranslation>(input, segment);
    // return Cached<DateTranslation>(input, segment);
  }

  return nullptr;
}

}  // namespace rime
