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
#include <boost/locale/encoding.hpp>
#include <boost/algorithm/string.hpp>

#include "often_charset_filter.h"

namespace rime {


static bool is_emoji(uint32_t ch) {

  // emoji data from http://www.unicode.org/Public/emoji/12.0/emoji-data.txt
  if ((ch == 0x0023) ||
      (ch == 0x002A) ||
      (ch >= 0x0030 && ch <= 0x0039) ||
      (ch == 0x00A9) ||
      (ch == 0x00AE) ||
      (ch == 0x203C) ||
      (ch == 0x2049) ||
      (ch == 0x2122) ||
      (ch == 0x2139) ||
      (ch >= 0x2194 && ch <= 0x2199) ||
      (ch >= 0x21A9 && ch <= 0x21AA) ||
      (ch >= 0x231A && ch <= 0x231B) ||
      (ch == 0x2328) ||
      (ch == 0x23CF) ||
      (ch >= 0x23E9 && ch <= 0x23F3) ||
      (ch >= 0x23F8 && ch <= 0x23FA) ||
      (ch == 0x24C2) ||
      (ch >= 0x25AA && ch <= 0x25AB) ||
      (ch == 0x25B6) ||
      (ch == 0x25C0) ||
      (ch >= 0x25FB && ch <= 0x25FE) ||
      (ch >= 0x2600 && ch <= 0x2604) ||
      (ch == 0x260E) ||
      (ch == 0x2611) ||
      (ch >= 0x2614 && ch <= 0x2615) ||
      (ch == 0x2618) ||
      (ch == 0x261D) ||
      (ch == 0x2620) ||
      (ch >= 0x2622 && ch <= 0x2623) ||
      (ch == 0x2626) ||
      (ch == 0x262A) ||
      (ch >= 0x262E && ch <= 0x262F) ||
      (ch >= 0x2638 && ch <= 0x263A) ||
      (ch == 0x2640) ||
      (ch == 0x2642) ||
      (ch >= 0x2648 && ch <= 0x2653) ||
      (ch >= 0x265F && ch <= 0x2660) ||
      (ch == 0x2663) ||
      (ch >= 0x2665 && ch <= 0x2666) ||
      (ch == 0x2668) ||
      (ch == 0x267B) ||
      (ch >= 0x267E && ch <= 0x267F) ||
      (ch >= 0x2692 && ch <= 0x2697) ||
      (ch == 0x2699) ||
      (ch >= 0x269B && ch <= 0x269C) ||
      (ch >= 0x26A0 && ch <= 0x26A1) ||
      (ch >= 0x26AA && ch <= 0x26AB) ||
      (ch >= 0x26B0 && ch <= 0x26B1) ||
      (ch >= 0x26BD && ch <= 0x26BE) ||
      (ch >= 0x26C4 && ch <= 0x26C5) ||
      (ch == 0x26C8) ||
      (ch == 0x26CE) ||
      (ch == 0x26CF) ||
      (ch == 0x26D1) ||
      (ch >= 0x26D3 && ch <= 0x26D4) ||
      (ch >= 0x26E9 && ch <= 0x26EA) ||
      (ch >= 0x26F0 && ch <= 0x26F5) ||
      (ch >= 0x26F7 && ch <= 0x26FA) ||
      (ch == 0x26FD) ||
      (ch == 0x2702) ||
      (ch == 0x2705) ||
      (ch >= 0x2708 && ch <= 0x2709) ||
      (ch >= 0x270A && ch <= 0x270B) ||
      (ch >= 0x270C && ch <= 0x270D) ||
      (ch == 0x270F) ||
      (ch == 0x2712) ||
      (ch == 0x2714) ||
      (ch == 0x2716) ||
      (ch == 0x271D) ||
      (ch == 0x2721) ||
      (ch == 0x2728) ||
      (ch >= 0x2733 && ch <= 0x2734) ||
      (ch == 0x2744) ||
      (ch == 0x2747) ||
      (ch == 0x274C) ||
      (ch == 0x274E) ||
      (ch >= 0x2753 && ch <= 0x2755) ||
      (ch == 0x2757) ||
      (ch >= 0x2763 && ch <= 0x2764) ||
      (ch >= 0x2795 && ch <= 0x2797) ||
      (ch == 0x27A1) ||
      (ch == 0x27B0) ||
      (ch == 0x27BF) ||
      (ch >= 0x2934 && ch <= 0x2935) ||
      (ch >= 0x2B05 && ch <= 0x2B07) ||
      (ch >= 0x2B1B && ch <= 0x2B1C) ||
      (ch == 0x2B50) ||
      (ch == 0x2B55) ||
      (ch == 0x3030) ||
      (ch == 0x303D) ||
      (ch == 0x3297) ||
      (ch == 0x3299) ||
      (ch == 0x1F004) ||
      (ch == 0x1F0CF) ||
      (ch >= 0x1F170 && ch <= 0x1F171) ||
      (ch == 0x1F17E) ||
      (ch == 0x1F17F) ||
      (ch == 0x1F18E) ||
      (ch >= 0x1F191 && ch <= 0x1F19A) ||
      (ch >= 0x1F1E6 && ch <= 0x1F1FF) ||
      (ch >= 0x1F201 && ch <= 0x1F202) ||
      (ch == 0x1F21A) ||
      (ch == 0x1F22F) ||
      (ch >= 0x1F232 && ch <= 0x1F23A) ||
      (ch >= 0x1F250 && ch <= 0x1F251) ||
      (ch >= 0x1F300 && ch <= 0x1F320) ||
      (ch == 0x1F321) ||
      (ch >= 0x1F324 && ch <= 0x1F32C) ||
      (ch >= 0x1F32D && ch <= 0x1F32F) ||
      (ch >= 0x1F330 && ch <= 0x1F335) ||
      (ch == 0x1F336) ||
      (ch >= 0x1F337 && ch <= 0x1F37C) ||
      (ch == 0x1F37D) ||
      (ch >= 0x1F37E && ch <= 0x1F37F) ||
      (ch >= 0x1F380 && ch <= 0x1F393) ||
      (ch >= 0x1F396 && ch <= 0x1F397) ||
      (ch >= 0x1F399 && ch <= 0x1F39B) ||
      (ch >= 0x1F39E && ch <= 0x1F39F) ||
      (ch >= 0x1F3A0 && ch <= 0x1F3C4) ||
      (ch == 0x1F3C5) ||
      (ch >= 0x1F3C6 && ch <= 0x1F3CA) ||
      (ch >= 0x1F3CB && ch <= 0x1F3CE) ||
      (ch >= 0x1F3CF && ch <= 0x1F3D3) ||
      (ch >= 0x1F3D4 && ch <= 0x1F3DF) ||
      (ch >= 0x1F3E0 && ch <= 0x1F3F0) ||
      (ch >= 0x1F3F3 && ch <= 0x1F3F5) ||
      (ch == 0x1F3F7) ||
      (ch >= 0x1F3F8 && ch <= 0x1F3FF) ||
      (ch >= 0x1F400 && ch <= 0x1F43E) ||
      (ch == 0x1F43F) ||
      (ch == 0x1F440) ||
      (ch == 0x1F441) ||
      (ch >= 0x1F442 && ch <= 0x1F4F7) ||
      (ch == 0x1F4F8) ||
      (ch >= 0x1F4F9 && ch <= 0x1F4FC) ||
      (ch == 0x1F4FD) ||
      (ch == 0x1F4FF) ||
      (ch >= 0x1F500 && ch <= 0x1F53D) ||
      (ch >= 0x1F549 && ch <= 0x1F54A) ||
      (ch >= 0x1F54B && ch <= 0x1F54E) ||
      (ch >= 0x1F550 && ch <= 0x1F567) ||
      (ch >= 0x1F56F && ch <= 0x1F570) ||
      (ch >= 0x1F573 && ch <= 0x1F579) ||
      (ch == 0x1F57A) ||
      (ch == 0x1F587) ||
      (ch >= 0x1F58A && ch <= 0x1F58D) ||
      (ch == 0x1F590) ||
      (ch >= 0x1F595 && ch <= 0x1F596) ||
      (ch == 0x1F5A4) ||
      (ch == 0x1F5A5) ||
      (ch == 0x1F5A8) ||
      (ch >= 0x1F5B1 && ch <= 0x1F5B2) ||
      (ch == 0x1F5BC) ||
      (ch >= 0x1F5C2 && ch <= 0x1F5C4) ||
      (ch >= 0x1F5D1 && ch <= 0x1F5D3) ||
      (ch >= 0x1F5DC && ch <= 0x1F5DE) ||
      (ch == 0x1F5E1) ||
      (ch == 0x1F5E3) ||
      (ch == 0x1F5E8) ||
      (ch == 0x1F5EF) ||
      (ch == 0x1F5F3) ||
      (ch == 0x1F5FA) ||
      (ch >= 0x1F5FB && ch <= 0x1F5FF) ||
      (ch == 0x1F600) ||
      (ch >= 0x1F601 && ch <= 0x1F610) ||
      (ch == 0x1F611) ||
      (ch >= 0x1F612 && ch <= 0x1F614) ||
      (ch == 0x1F615) ||
      (ch == 0x1F616) ||
      (ch == 0x1F617) ||
      (ch == 0x1F618) ||
      (ch == 0x1F619) ||
      (ch == 0x1F61A) ||
      (ch == 0x1F61B) ||
      (ch >= 0x1F61C && ch <= 0x1F61E) ||
      (ch == 0x1F61F) ||
      (ch >= 0x1F620 && ch <= 0x1F625) ||
      (ch >= 0x1F626 && ch <= 0x1F627) ||
      (ch >= 0x1F628 && ch <= 0x1F62B) ||
      (ch == 0x1F62C) ||
      (ch == 0x1F62D) ||
      (ch >= 0x1F62E && ch <= 0x1F62F) ||
      (ch >= 0x1F630 && ch <= 0x1F633) ||
      (ch == 0x1F634) ||
      (ch >= 0x1F635 && ch <= 0x1F640) ||
      (ch >= 0x1F641 && ch <= 0x1F642) ||
      (ch >= 0x1F643 && ch <= 0x1F644) ||
      (ch >= 0x1F645 && ch <= 0x1F64F) ||
      (ch >= 0x1F680 && ch <= 0x1F6C5) ||
      (ch >= 0x1F6CB && ch <= 0x1F6CF) ||
      (ch == 0x1F6D0) ||
      (ch >= 0x1F6D1 && ch <= 0x1F6D2) ||
      (ch == 0x1F6D5) ||
      (ch >= 0x1F6E0 && ch <= 0x1F6E5) ||
      (ch == 0x1F6E9) ||
      (ch >= 0x1F6EB && ch <= 0x1F6EC) ||
      (ch == 0x1F6F0) ||
      (ch == 0x1F6F3) ||
      (ch >= 0x1F6F4 && ch <= 0x1F6F6) ||
      (ch >= 0x1F6F7 && ch <= 0x1F6F8) ||
      (ch == 0x1F6F9) ||
      (ch == 0x1F6FA) ||
      (ch >= 0x1F7E0 && ch <= 0x1F7EB) ||
      (ch >= 0x1F90D && ch <= 0x1F90F) ||
      (ch >= 0x1F910 && ch <= 0x1F918) ||
      (ch >= 0x1F919 && ch <= 0x1F91E) ||
      (ch == 0x1F91F) ||
      (ch >= 0x1F920 && ch <= 0x1F927) ||
      (ch >= 0x1F928 && ch <= 0x1F92F) ||
      (ch == 0x1F930) ||
      (ch >= 0x1F931 && ch <= 0x1F932) ||
      (ch >= 0x1F933 && ch <= 0x1F93A) ||
      (ch >= 0x1F93C && ch <= 0x1F93E) ||
      (ch == 0x1F93F) ||
      (ch >= 0x1F940 && ch <= 0x1F945) ||
      (ch >= 0x1F947 && ch <= 0x1F94B) ||
      (ch == 0x1F94C) ||
      (ch >= 0x1F94D && ch <= 0x1F94F) ||
      (ch >= 0x1F950 && ch <= 0x1F95E) ||
      (ch >= 0x1F95F && ch <= 0x1F96B) ||
      (ch >= 0x1F96C && ch <= 0x1F970) ||
      (ch == 0x1F971) ||
      (ch >= 0x1F973 && ch <= 0x1F976) ||
      (ch == 0x1F97A) ||
      (ch == 0x1F97B) ||
      (ch >= 0x1F97C && ch <= 0x1F97F) ||
      (ch >= 0x1F980 && ch <= 0x1F984) ||
      (ch >= 0x1F985 && ch <= 0x1F991) ||
      (ch >= 0x1F992 && ch <= 0x1F997) ||
      (ch >= 0x1F998 && ch <= 0x1F9A2) ||
      (ch >= 0x1F9A5 && ch <= 0x1F9AA) ||
      (ch >= 0x1F9AE && ch <= 0x1F9AF) ||
      (ch >= 0x1F9B0 && ch <= 0x1F9B9) ||
      (ch >= 0x1F9BA && ch <= 0x1F9BF) ||
      (ch == 0x1F9C0) ||
      (ch >= 0x1F9C1 && ch <= 0x1F9C2) ||
      (ch >= 0x1F9C3 && ch <= 0x1F9CA) ||
      (ch >= 0x1F9CD && ch <= 0x1F9CF) ||
      (ch >= 0x1F9D0 && ch <= 0x1F9E6) ||
      (ch >= 0x1F9E7 && ch <= 0x1F9FF) ||
      (ch >= 0x1FA70 && ch <= 0x1FA73) ||
      (ch >= 0x1FA78 && ch <= 0x1FA7A) ||
      (ch >= 0x1FA80 && ch <= 0x1FA82) ||
      (ch >= 0x1FA90 && ch <= 0x1FA95) ||
      (ch >= 0x231A && ch <= 0x231B) ||
      (ch >= 0x23E9 && ch <= 0x23EC) ||
      (ch == 0x23F0) ||
      (ch == 0x23F3) ||
      (ch >= 0x25FD && ch <= 0x25FE) ||
      (ch >= 0x2614 && ch <= 0x2615) ||
      (ch >= 0x2648 && ch <= 0x2653) ||
      (ch == 0x267F) ||
      (ch == 0x2693) ||
      (ch == 0x26A1) ||
      (ch >= 0x26AA && ch <= 0x26AB) ||
      (ch >= 0x26BD && ch <= 0x26BE) ||
      (ch >= 0x26C4 && ch <= 0x26C5) ||
      (ch == 0x26CE) ||
      (ch == 0x26D4) ||
      (ch == 0x26EA) ||
      (ch >= 0x26F2 && ch <= 0x26F3) ||
      (ch == 0x26F5) ||
      (ch == 0x26FA) ||
      (ch == 0x26FD) ||
      (ch == 0x2705) ||
      (ch >= 0x270A && ch <= 0x270B) ||
      (ch == 0x2728) ||
      (ch == 0x274C) ||
      (ch == 0x274E) ||
      (ch >= 0x2753 && ch <= 0x2755) ||
      (ch == 0x2757) ||
      (ch >= 0x2795 && ch <= 0x2797) ||
      (ch == 0x27B0) ||
      (ch == 0x27BF) ||
      (ch >= 0x2B1B && ch <= 0x2B1C) ||
      (ch == 0x2B50) ||
      (ch == 0x2B55) ||
      (ch == 0x1F004) ||
      (ch == 0x1F0CF) ||
      (ch == 0x1F18E) ||
      (ch >= 0x1F191 && ch <= 0x1F19A) ||
      (ch >= 0x1F1E6 && ch <= 0x1F1FF) ||
      (ch == 0x1F201) ||
      (ch == 0x1F21A) ||
      (ch == 0x1F22F) ||
      (ch >= 0x1F232 && ch <= 0x1F236) ||
      (ch >= 0x1F238 && ch <= 0x1F23A) ||
      (ch >= 0x1F250 && ch <= 0x1F251) ||
      (ch >= 0x1F300 && ch <= 0x1F320) ||
      (ch >= 0x1F32D && ch <= 0x1F32F) ||
      (ch >= 0x1F330 && ch <= 0x1F335) ||
      (ch >= 0x1F337 && ch <= 0x1F37C) ||
      (ch >= 0x1F37E && ch <= 0x1F37F) ||
      (ch >= 0x1F380 && ch <= 0x1F393) ||
      (ch >= 0x1F3A0 && ch <= 0x1F3C4) ||
      (ch == 0x1F3C5) ||
      (ch >= 0x1F3C6 && ch <= 0x1F3CA) ||
      (ch >= 0x1F3CF && ch <= 0x1F3D3) ||
      (ch >= 0x1F3E0 && ch <= 0x1F3F0) ||
      (ch == 0x1F3F4) ||
      (ch >= 0x1F3F8 && ch <= 0x1F3FF) ||
      (ch >= 0x1F400 && ch <= 0x1F43E) ||
      (ch == 0x1F440) ||
      (ch >= 0x1F442 && ch <= 0x1F4F7) ||
      (ch == 0x1F4F8) ||
      (ch >= 0x1F4F9 && ch <= 0x1F4FC) ||
      (ch == 0x1F4FF) ||
      (ch >= 0x1F500 && ch <= 0x1F53D) ||
      (ch >= 0x1F54B && ch <= 0x1F54E) ||
      (ch >= 0x1F550 && ch <= 0x1F567) ||
      (ch == 0x1F57A) ||
      (ch >= 0x1F595 && ch <= 0x1F596) ||
      (ch == 0x1F5A4) ||
      (ch >= 0x1F5FB && ch <= 0x1F5FF) ||
      (ch == 0x1F600) ||
      (ch >= 0x1F601 && ch <= 0x1F610) ||
      (ch == 0x1F611) ||
      (ch >= 0x1F612 && ch <= 0x1F614) ||
      (ch == 0x1F615) ||
      (ch == 0x1F616) ||
      (ch == 0x1F617) ||
      (ch == 0x1F618) ||
      (ch == 0x1F619) ||
      (ch == 0x1F61A) ||
      (ch == 0x1F61B) ||
      (ch >= 0x1F61C && ch <= 0x1F61E) ||
      (ch == 0x1F61F) ||
      (ch >= 0x1F620 && ch <= 0x1F625) ||
      (ch >= 0x1F626 && ch <= 0x1F627) ||
      (ch >= 0x1F628 && ch <= 0x1F62B) ||
      (ch == 0x1F62C) ||
      (ch == 0x1F62D) ||
      (ch >= 0x1F62E && ch <= 0x1F62F) ||
      (ch >= 0x1F630 && ch <= 0x1F633) ||
      (ch == 0x1F634) ||
      (ch >= 0x1F635 && ch <= 0x1F640) ||
      (ch >= 0x1F641 && ch <= 0x1F642) ||
      (ch >= 0x1F643 && ch <= 0x1F644) ||
      (ch >= 0x1F645 && ch <= 0x1F64F) ||
      (ch >= 0x1F680 && ch <= 0x1F6C5) ||
      (ch == 0x1F6CC) ||
      (ch == 0x1F6D0) ||
      (ch >= 0x1F6D1 && ch <= 0x1F6D2) ||
      (ch == 0x1F6D5) ||
      (ch >= 0x1F6EB && ch <= 0x1F6EC) ||
      (ch >= 0x1F6F4 && ch <= 0x1F6F6) ||
      (ch >= 0x1F6F7 && ch <= 0x1F6F8) ||
      (ch == 0x1F6F9) ||
      (ch == 0x1F6FA) ||
      (ch >= 0x1F7E0 && ch <= 0x1F7EB) ||
      (ch >= 0x1F90D && ch <= 0x1F90F) ||
      (ch >= 0x1F910 && ch <= 0x1F918) ||
      (ch >= 0x1F919 && ch <= 0x1F91E) ||
      (ch == 0x1F91F) ||
      (ch >= 0x1F920 && ch <= 0x1F927) ||
      (ch >= 0x1F928 && ch <= 0x1F92F) ||
      (ch == 0x1F930) ||
      (ch >= 0x1F931 && ch <= 0x1F932) ||
      (ch >= 0x1F933 && ch <= 0x1F93A) ||
      (ch >= 0x1F93C && ch <= 0x1F93E) ||
      (ch == 0x1F93F) ||
      (ch >= 0x1F940 && ch <= 0x1F945) ||
      (ch >= 0x1F947 && ch <= 0x1F94B) ||
      (ch == 0x1F94C) ||
      (ch >= 0x1F94D && ch <= 0x1F94F) ||
      (ch >= 0x1F950 && ch <= 0x1F95E) ||
      (ch >= 0x1F95F && ch <= 0x1F96B) ||
      (ch >= 0x1F96C && ch <= 0x1F970) ||
      (ch == 0x1F971) ||
      (ch >= 0x1F973 && ch <= 0x1F976) ||
      (ch == 0x1F97A) ||
      (ch == 0x1F97B) ||
      (ch >= 0x1F97C && ch <= 0x1F97F) ||
      (ch >= 0x1F980 && ch <= 0x1F984) ||
      (ch >= 0x1F985 && ch <= 0x1F991) ||
      (ch >= 0x1F992 && ch <= 0x1F997) ||
      (ch >= 0x1F998 && ch <= 0x1F9A2) ||
      (ch >= 0x1F9A5 && ch <= 0x1F9AA) ||
      (ch >= 0x1F9AE && ch <= 0x1F9AF) ||
      (ch >= 0x1F9B0 && ch <= 0x1F9B9) ||
      (ch >= 0x1F9BA && ch <= 0x1F9BF) ||
      (ch == 0x1F9C0) ||
      (ch >= 0x1F9C1 && ch <= 0x1F9C2) ||
      (ch >= 0x1F9C3 && ch <= 0x1F9CA) ||
      (ch >= 0x1F9CD && ch <= 0x1F9CF) ||
      (ch >= 0x1F9D0 && ch <= 0x1F9E6) ||
      (ch >= 0x1F9E7 && ch <= 0x1F9FF) ||
      (ch >= 0x1FA70 && ch <= 0x1FA73) ||
      (ch >= 0x1FA78 && ch <= 0x1FA7A) ||
      (ch >= 0x1FA80 && ch <= 0x1FA82) ||
      (ch >= 0x1FA90 && ch <= 0x1FA95) ||
      (ch >= 0x1F3FB && ch <= 0x1F3FF) ||
      (ch == 0x261D) ||
      (ch == 0x26F9) ||
      (ch >= 0x270A && ch <= 0x270B) ||
      (ch >= 0x270C && ch <= 0x270D) ||
      (ch == 0x1F385) ||
      (ch >= 0x1F3C2 && ch <= 0x1F3C4) ||
      (ch == 0x1F3C7) ||
      (ch == 0x1F3CA) ||
      (ch >= 0x1F3CB && ch <= 0x1F3CC) ||
      (ch >= 0x1F442 && ch <= 0x1F443) ||
      (ch >= 0x1F446 && ch <= 0x1F450) ||
      (ch >= 0x1F466 && ch <= 0x1F478) ||
      (ch == 0x1F47C) ||
      (ch >= 0x1F481 && ch <= 0x1F483) ||
      (ch >= 0x1F485 && ch <= 0x1F487) ||
      (ch == 0x1F48F) ||
      (ch == 0x1F491) ||
      (ch == 0x1F4AA) ||
      (ch >= 0x1F574 && ch <= 0x1F575) ||
      (ch == 0x1F57A) ||
      (ch == 0x1F590) ||
      (ch >= 0x1F595 && ch <= 0x1F596) ||
      (ch >= 0x1F645 && ch <= 0x1F647) ||
      (ch >= 0x1F64B && ch <= 0x1F64F) ||
      (ch == 0x1F6A3) ||
      (ch >= 0x1F6B4 && ch <= 0x1F6B6) ||
      (ch == 0x1F6C0) ||
      (ch == 0x1F6CC) ||
      (ch == 0x1F90F) ||
      (ch == 0x1F918) ||
      (ch >= 0x1F919 && ch <= 0x1F91E) ||
      (ch == 0x1F91F) ||
      (ch == 0x1F926) ||
      (ch == 0x1F930) ||
      (ch >= 0x1F931 && ch <= 0x1F932) ||
      (ch >= 0x1F933 && ch <= 0x1F939) ||
      (ch >= 0x1F93C && ch <= 0x1F93E) ||
      (ch >= 0x1F9B5 && ch <= 0x1F9B6) ||
      (ch >= 0x1F9B8 && ch <= 0x1F9B9) ||
      (ch == 0x1F9BB) ||
      (ch >= 0x1F9CD && ch <= 0x1F9CF) ||
      (ch >= 0x1F9D1 && ch <= 0x1F9DD) ||
      (ch == 0x0023) ||
      (ch == 0x002A) ||
      (ch >= 0x0030 && ch <= 0x0039) ||
      (ch == 0x200D) ||
      (ch == 0x20E3) ||
      (ch == 0xFE0F) ||
      (ch >= 0x1F1E6 && ch <= 0x1F1FF) ||
      (ch >= 0x1F3FB && ch <= 0x1F3FF) ||
      (ch >= 0x1F9B0 && ch <= 0x1F9B3) ||
      (ch >= 0xE0020 && ch <= 0xE007F) ||
      (ch == 0x00A9) ||
      (ch == 0x00AE) ||
      (ch == 0x203C) ||
      (ch == 0x2049) ||
      (ch == 0x2122) ||
      (ch == 0x2139) ||
      (ch >= 0x2194 && ch <= 0x2199) ||
      (ch >= 0x21A9 && ch <= 0x21AA) ||
      (ch >= 0x231A && ch <= 0x231B) ||
      (ch == 0x2328) ||
      (ch == 0x2388) ||
      (ch == 0x23CF) ||
      (ch >= 0x23E9 && ch <= 0x23F3) ||
      (ch >= 0x23F8 && ch <= 0x23FA) ||
      (ch == 0x24C2) ||
      (ch >= 0x25AA && ch <= 0x25AB) ||
      (ch == 0x25B6) ||
      (ch == 0x25C0) ||
      (ch >= 0x25FB && ch <= 0x25FE) ||
      (ch >= 0x2600 && ch <= 0x2605) ||
      (ch >= 0x2607 && ch <= 0x2612) ||
      (ch >= 0x2614 && ch <= 0x2615) ||
      (ch >= 0x2616 && ch <= 0x2617) ||
      (ch == 0x2618) ||
      (ch == 0x2619) ||
      (ch >= 0x261A && ch <= 0x266F) ||
      (ch >= 0x2670 && ch <= 0x2671) ||
      (ch >= 0x2672 && ch <= 0x267D) ||
      (ch >= 0x267E && ch <= 0x267F) ||
      (ch >= 0x2680 && ch <= 0x2685) ||
      (ch >= 0x2690 && ch <= 0x2691) ||
      (ch >= 0x2692 && ch <= 0x269C) ||
      (ch == 0x269D) ||
      (ch >= 0x269E && ch <= 0x269F) ||
      (ch >= 0x26A0 && ch <= 0x26A1) ||
      (ch >= 0x26A2 && ch <= 0x26B1) ||
      (ch == 0x26B2) ||
      (ch >= 0x26B3 && ch <= 0x26BC) ||
      (ch >= 0x26BD && ch <= 0x26BF) ||
      (ch >= 0x26C0 && ch <= 0x26C3) ||
      (ch >= 0x26C4 && ch <= 0x26CD) ||
      (ch == 0x26CE) ||
      (ch >= 0x26CF && ch <= 0x26E1) ||
      (ch == 0x26E2) ||
      (ch == 0x26E3) ||
      (ch >= 0x26E4 && ch <= 0x26E7) ||
      (ch >= 0x26E8 && ch <= 0x26FF) ||
      (ch == 0x2700) ||
      (ch >= 0x2701 && ch <= 0x2704) ||
      (ch == 0x2705) ||
      (ch >= 0x2708 && ch <= 0x2709) ||
      (ch >= 0x270A && ch <= 0x270B) ||
      (ch >= 0x270C && ch <= 0x2712) ||
      (ch == 0x2714) ||
      (ch == 0x2716) ||
      (ch == 0x271D) ||
      (ch == 0x2721) ||
      (ch == 0x2728) ||
      (ch >= 0x2733 && ch <= 0x2734) ||
      (ch == 0x2744) ||
      (ch == 0x2747) ||
      (ch == 0x274C) ||
      (ch == 0x274E) ||
      (ch >= 0x2753 && ch <= 0x2755) ||
      (ch == 0x2757) ||
      (ch >= 0x2763 && ch <= 0x2767) ||
      (ch >= 0x2795 && ch <= 0x2797) ||
      (ch == 0x27A1) ||
      (ch == 0x27B0) ||
      (ch == 0x27BF) ||
      (ch >= 0x2934 && ch <= 0x2935) ||
      (ch >= 0x2B05 && ch <= 0x2B07) ||
      (ch >= 0x2B1B && ch <= 0x2B1C) ||
      (ch == 0x2B50) ||
      (ch == 0x2B55) ||
      (ch == 0x3030) ||
      (ch == 0x303D) ||
      (ch == 0x3297) ||
      (ch == 0x3299) ||
      (ch >= 0x1F000 && ch <= 0x1F02B) ||
      (ch >= 0x1F02C && ch <= 0x1F02F) ||
      (ch >= 0x1F030 && ch <= 0x1F093) ||
      (ch >= 0x1F094 && ch <= 0x1F09F) ||
      (ch >= 0x1F0A0 && ch <= 0x1F0AE) ||
      (ch >= 0x1F0AF && ch <= 0x1F0B0) ||
      (ch >= 0x1F0B1 && ch <= 0x1F0BE) ||
      (ch == 0x1F0BF) ||
      (ch == 0x1F0C0) ||
      (ch >= 0x1F0C1 && ch <= 0x1F0CF) ||
      (ch == 0x1F0D0) ||
      (ch >= 0x1F0D1 && ch <= 0x1F0DF) ||
      (ch >= 0x1F0E0 && ch <= 0x1F0F5) ||
      (ch >= 0x1F0F6 && ch <= 0x1F0FF) ||
      (ch >= 0x1F10D && ch <= 0x1F10F) ||
      (ch == 0x1F12F) ||
      (ch == 0x1F16C) ||
      (ch >= 0x1F16D && ch <= 0x1F16F) ||
      (ch >= 0x1F170 && ch <= 0x1F171) ||
      (ch == 0x1F17E) ||
      (ch == 0x1F17F) ||
      (ch == 0x1F18E) ||
      (ch >= 0x1F191 && ch <= 0x1F19A) ||
      (ch >= 0x1F1AD && ch <= 0x1F1E5) ||
      (ch >= 0x1F201 && ch <= 0x1F202) ||
      (ch >= 0x1F203 && ch <= 0x1F20F) ||
      (ch == 0x1F21A) ||
      (ch == 0x1F22F) ||
      (ch >= 0x1F232 && ch <= 0x1F23A) ||
      (ch >= 0x1F23C && ch <= 0x1F23F) ||
      (ch >= 0x1F249 && ch <= 0x1F24F) ||
      (ch >= 0x1F250 && ch <= 0x1F251) ||
      (ch >= 0x1F252 && ch <= 0x1F25F) ||
      (ch >= 0x1F260 && ch <= 0x1F265) ||
      (ch >= 0x1F266 && ch <= 0x1F2FF) ||
      (ch >= 0x1F300 && ch <= 0x1F320) ||
      (ch >= 0x1F321 && ch <= 0x1F32C) ||
      (ch >= 0x1F32D && ch <= 0x1F32F) ||
      (ch >= 0x1F330 && ch <= 0x1F335) ||
      (ch == 0x1F336) ||
      (ch >= 0x1F337 && ch <= 0x1F37C) ||
      (ch == 0x1F37D) ||
      (ch >= 0x1F37E && ch <= 0x1F37F) ||
      (ch >= 0x1F380 && ch <= 0x1F393) ||
      (ch >= 0x1F394 && ch <= 0x1F39F) ||
      (ch >= 0x1F3A0 && ch <= 0x1F3C4) ||
      (ch == 0x1F3C5) ||
      (ch >= 0x1F3C6 && ch <= 0x1F3CA) ||
      (ch >= 0x1F3CB && ch <= 0x1F3CE) ||
      (ch >= 0x1F3CF && ch <= 0x1F3D3) ||
      (ch >= 0x1F3D4 && ch <= 0x1F3DF) ||
      (ch >= 0x1F3E0 && ch <= 0x1F3F0) ||
      (ch >= 0x1F3F1 && ch <= 0x1F3F7) ||
      (ch >= 0x1F3F8 && ch <= 0x1F3FA) ||
      (ch >= 0x1F400 && ch <= 0x1F43E) ||
      (ch == 0x1F43F) ||
      (ch == 0x1F440) ||
      (ch == 0x1F441) ||
      (ch >= 0x1F442 && ch <= 0x1F4F7) ||
      (ch == 0x1F4F8) ||
      (ch >= 0x1F4F9 && ch <= 0x1F4FC) ||
      (ch >= 0x1F4FD && ch <= 0x1F4FE) ||
      (ch == 0x1F4FF) ||
      (ch >= 0x1F500 && ch <= 0x1F53D) ||
      (ch >= 0x1F546 && ch <= 0x1F54A) ||
      (ch >= 0x1F54B && ch <= 0x1F54F) ||
      (ch >= 0x1F550 && ch <= 0x1F567) ||
      (ch >= 0x1F568 && ch <= 0x1F579) ||
      (ch == 0x1F57A) ||
      (ch >= 0x1F57B && ch <= 0x1F5A3) ||
      (ch == 0x1F5A4) ||
      (ch >= 0x1F5A5 && ch <= 0x1F5FA) ||
      (ch >= 0x1F5FB && ch <= 0x1F5FF) ||
      (ch == 0x1F600) ||
      (ch >= 0x1F601 && ch <= 0x1F610) ||
      (ch == 0x1F611) ||
      (ch >= 0x1F612 && ch <= 0x1F614) ||
      (ch == 0x1F615) ||
      (ch == 0x1F616) ||
      (ch == 0x1F617) ||
      (ch == 0x1F618) ||
      (ch == 0x1F619) ||
      (ch == 0x1F61A) ||
      (ch == 0x1F61B) ||
      (ch >= 0x1F61C && ch <= 0x1F61E) ||
      (ch == 0x1F61F) ||
      (ch >= 0x1F620 && ch <= 0x1F625) ||
      (ch >= 0x1F626 && ch <= 0x1F627) ||
      (ch >= 0x1F628 && ch <= 0x1F62B) ||
      (ch == 0x1F62C) ||
      (ch == 0x1F62D) ||
      (ch >= 0x1F62E && ch <= 0x1F62F) ||
      (ch >= 0x1F630 && ch <= 0x1F633) ||
      (ch == 0x1F634) ||
      (ch >= 0x1F635 && ch <= 0x1F640) ||
      (ch >= 0x1F641 && ch <= 0x1F642) ||
      (ch >= 0x1F643 && ch <= 0x1F644) ||
      (ch >= 0x1F645 && ch <= 0x1F64F) ||
      (ch >= 0x1F680 && ch <= 0x1F6C5) ||
      (ch >= 0x1F6C6 && ch <= 0x1F6CF) ||
      (ch == 0x1F6D0) ||
      (ch >= 0x1F6D1 && ch <= 0x1F6D2) ||
      (ch >= 0x1F6D3 && ch <= 0x1F6D4) ||
      (ch == 0x1F6D5) ||
      (ch >= 0x1F6D6 && ch <= 0x1F6DF) ||
      (ch >= 0x1F6E0 && ch <= 0x1F6EC) ||
      (ch >= 0x1F6ED && ch <= 0x1F6EF) ||
      (ch >= 0x1F6F0 && ch <= 0x1F6F3) ||
      (ch >= 0x1F6F4 && ch <= 0x1F6F6) ||
      (ch >= 0x1F6F7 && ch <= 0x1F6F8) ||
      (ch == 0x1F6F9) ||
      (ch == 0x1F6FA) ||
      (ch >= 0x1F6FB && ch <= 0x1F6FF) ||
      (ch >= 0x1F774 && ch <= 0x1F77F) ||
      (ch >= 0x1F7D5 && ch <= 0x1F7D8) ||
      (ch >= 0x1F7D9 && ch <= 0x1F7DF) ||
      (ch >= 0x1F7E0 && ch <= 0x1F7EB) ||
      (ch >= 0x1F7EC && ch <= 0x1F7FF) ||
      (ch >= 0x1F80C && ch <= 0x1F80F) ||
      (ch >= 0x1F848 && ch <= 0x1F84F) ||
      (ch >= 0x1F85A && ch <= 0x1F85F) ||
      (ch >= 0x1F888 && ch <= 0x1F88F) ||
      (ch >= 0x1F8AE && ch <= 0x1F8FF) ||
      (ch == 0x1F90C) ||
      (ch >= 0x1F90D && ch <= 0x1F90F) ||
      (ch >= 0x1F910 && ch <= 0x1F918) ||
      (ch >= 0x1F919 && ch <= 0x1F91E) ||
      (ch == 0x1F91F) ||
      (ch >= 0x1F920 && ch <= 0x1F927) ||
      (ch >= 0x1F928 && ch <= 0x1F92F) ||
      (ch == 0x1F930) ||
      (ch >= 0x1F931 && ch <= 0x1F932) ||
      (ch >= 0x1F933 && ch <= 0x1F93A) ||
      (ch >= 0x1F93C && ch <= 0x1F93E) ||
      (ch == 0x1F93F) ||
      (ch >= 0x1F940 && ch <= 0x1F945) ||
      (ch >= 0x1F947 && ch <= 0x1F94B) ||
      (ch == 0x1F94C) ||
      (ch >= 0x1F94D && ch <= 0x1F94F) ||
      (ch >= 0x1F950 && ch <= 0x1F95E) ||
      (ch >= 0x1F95F && ch <= 0x1F96B) ||
      (ch >= 0x1F96C && ch <= 0x1F970) ||
      (ch == 0x1F971) ||
      (ch == 0x1F972) ||
      (ch >= 0x1F973 && ch <= 0x1F976) ||
      (ch >= 0x1F977 && ch <= 0x1F979) ||
      (ch == 0x1F97A) ||
      (ch == 0x1F97B) ||
      (ch >= 0x1F97C && ch <= 0x1F97F) ||
      (ch >= 0x1F980 && ch <= 0x1F984) ||
      (ch >= 0x1F985 && ch <= 0x1F991) ||
      (ch >= 0x1F992 && ch <= 0x1F997) ||
      (ch >= 0x1F998 && ch <= 0x1F9A2) ||
      (ch >= 0x1F9A3 && ch <= 0x1F9A4) ||
      (ch >= 0x1F9A5 && ch <= 0x1F9AA) ||
      (ch >= 0x1F9AB && ch <= 0x1F9AD) ||
      (ch >= 0x1F9AE && ch <= 0x1F9AF) ||
      (ch >= 0x1F9B0 && ch <= 0x1F9B9) ||
      (ch >= 0x1F9BA && ch <= 0x1F9BF) ||
      (ch == 0x1F9C0) ||
      (ch >= 0x1F9C1 && ch <= 0x1F9C2) ||
      (ch >= 0x1F9C3 && ch <= 0x1F9CA) ||
      (ch >= 0x1F9CB && ch <= 0x1F9CC) ||
      (ch >= 0x1F9CD && ch <= 0x1F9CF) ||
      (ch >= 0x1F9D0 && ch <= 0x1F9E6) ||
      (ch >= 0x1F9E7 && ch <= 0x1F9FF) ||
      (ch >= 0x1FA00 && ch <= 0x1FA53) ||
      (ch >= 0x1FA54 && ch <= 0x1FA5F) ||
      (ch >= 0x1FA60 && ch <= 0x1FA6D) ||
      (ch >= 0x1FA6E && ch <= 0x1FA6F) ||
      (ch >= 0x1FA70 && ch <= 0x1FA73) ||
      (ch >= 0x1FA74 && ch <= 0x1FA77) ||
      (ch >= 0x1FA78 && ch <= 0x1FA7A) ||
      (ch >= 0x1FA7B && ch <= 0x1FA7F) ||
      (ch >= 0x1FA80 && ch <= 0x1FA82) ||
      (ch >= 0x1FA83 && ch <= 0x1FA8F) ||
      (ch >= 0x1FA90 && ch <= 0x1FA95) ||
      (ch >= 0x1FA96 && ch <= 0x1FFFD))
      return true;

  return false;
}

static bool is_all_emoji(const string& text)
{
  const char *p = text.c_str();
  uint32_t ch;

  while ((ch = utf8::unchecked::next(p)) != 0) {
    if (!is_emoji(ch)) {
      return false;
    }
  }

  return true;
}


// OftenCharsetFilterTranslation

class OftenCharsetFilterTranslation : public Translation {
 public:
  OftenCharsetFilterTranslation(an<Translation> translation,
                                   const string& charset,
                                   bool is_often,
                                   bool is_advanced);

  bool Next();
  an<Candidate> Peek();

 protected:
  an<Translation> translation_;
  CandidateQueue cache_;

  an<Candidate> PeekCache();

  bool NextCache();

  void InitAdvanced();

  bool LocateNextCandidate();

  bool FilterText(const string& text);

  string charset_;
  bool is_often_;
  bool is_filter_;
  bool is_advanced_;
};

OftenCharsetFilterTranslation::OftenCharsetFilterTranslation(
    an<Translation> translation, const string& charset,
    bool is_often, bool is_advanced)
    : translation_(translation) {
  set_exhausted(!translation_ || translation_->exhausted());

      charset_ = charset;
      is_often_ = is_often;
      is_filter_ = true;
      is_advanced_ = is_advanced;

      InitAdvanced();
      LocateNextCandidate();
}

bool OftenCharsetFilterTranslation::Next() {
  if (exhausted()) {
    return false;
  }
  if (!cache_.empty()) {
    cache_.pop_front();
  }
  else {
    translation_->Next();
  }
  if (cache_.empty() && translation_->exhausted()) {
    set_exhausted(true);
  }
  return LocateNextCandidate();
}

an<Candidate> OftenCharsetFilterTranslation::Peek() {
  if (exhausted()) {
    return nullptr;
  }
  // an<Candidate> cand;
  if (!cache_.empty()) {
    return cache_.front();
    // cand = cache_.front();
  }
  else {
    return translation_->Peek();
    // cand = translation_->Peek();
  }
  // return New<ShadowCandidate>(cand,cand->type(),cand->text(),cand->type());
}

bool OftenCharsetFilterTranslation::LocateNextCandidate() {
  an<Candidate> cand;
  while (!cache_.empty()) {
    cand = PeekCache();
    if ( FilterText(cand->text()) )
      return true;
    NextCache();
  }

  while (!translation_->exhausted()) {
    cand = translation_->Peek();
    if (cand && FilterText(cand->text()))
      return true;
    translation_->Next();
  }

  set_exhausted(true);
  return false;
}

an<Candidate> OftenCharsetFilterTranslation::PeekCache() {
  if (cache_.empty())
  {
    return nullptr;
  }
  return cache_.front();
}

bool OftenCharsetFilterTranslation::NextCache() {
  if (cache_.empty()) {
    return false;
  }
  cache_.pop_front();
  return true;
}

void OftenCharsetFilterTranslation::InitAdvanced() {
  if (is_advanced_ && is_often_) {
    // 一个简体都没有就不开启过滤。
    is_filter_ = false;
    size_t i = 0;
    while (!translation_->exhausted()) {
      auto cand = translation_->Peek();
      if (cand && cand->type() != "completion" &&
          OftenCharsetFilter::FilterText(cand->text(),
                                        charset_)) {
        // 只要有一个简体，就只显示简体。
        // 有两个简体就关闭过滤。
        // 简单来说就是过滤了能上屏才过滤。
        // 过滤了还不能上屏，就失去了过滤的意义。
        // type: table 词库里的；completion 逐码提示的。
        is_filter_ = true;
        ++i;
        if (i >= 2)
        {
          is_filter_ = false;
          break;
        }
      }
      cache_.push_back(cand);
      translation_->Next();
    }
  } else {
    LocateNextCandidate();
  }
}

bool OftenCharsetFilterTranslation::FilterText(const string& text) {
    if (charset_ == "emoji") {
      return OftenCharsetFilter::FilterText(text, charset_);
    }
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

  // 全都搞定了，完全自己写还快些。

  // 这个功能历时三天到现在结束。
  // 第三天开始自己写。
  return !is_filter_ || 
  (is_often_ == OftenCharsetFilter::FilterText(text, charset_));
}

// OftenCharsetFilter

OftenCharsetFilter::OftenCharsetFilter(const Ticket& ticket)
      : Filter(ticket), TagMatching(ticket)  {
  if (name_space_ == "filter") {
    name_space_ = "often_charset_filter";
  }
  if (Config* config = engine_->schema()->config()) {
    config->GetString(name_space_ + "/charset", &charset_);
    config->GetString(name_space_ + "/often", &often_);
    config->GetString(name_space_ + "/rare", &rare_);
    config->GetBool(name_space_ + "/is_advanced", &is_advanced_);
    config->GetBool(name_space_ + "/difficult", &is_difficult_);
    config->GetBool(name_space_ + "/default_enable", &is_default_enable_);
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
  if (engine_->context()->get_option(often_) ||
      engine_->context()->get_option(rare_) ||
      charset_ == "emoji" ||
      is_default_enable_) {
    return New<OftenCharsetFilterTranslation>(translation,
                        charset_,
                        (!engine_->context()->get_option(rare_) &&
                        !is_difficult_),
                        is_advanced_);
  }
  return translation;
}

bool OftenCharsetFilter::FilterText(const string& text,
                                       const string& charset) {
  if (charset.empty()) {
    return true;
  }
  if (charset == "emoji") {
    return is_all_emoji(text);
  }
  try {
    boost::locale::conv::from_utf(
        text, charset, boost::locale::conv::method_type::stop);
  }
  catch(boost::locale::conv::conversion_error const& ex) {
    DLOG(INFO) << ex.what();
    return false;
  }
  catch(boost::locale::conv::invalid_charset_error const& ex) {
    DLOG(ERROR) << ex.what();
    return true;
  }
  return true;
}


}  // namespace rime
