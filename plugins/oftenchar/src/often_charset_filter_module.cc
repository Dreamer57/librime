#include <rime/component.h>
#include <rime/registry.h>
#include <rime_api.h>

#include "often_charset_filter.h"

using namespace rime;

static void rime_often_charset_filter_initialize() {
  Registry &r = Registry::instance();
  r.Register("often_charset_filter", new Component<OftenCharsetFilter>);
}

static void rime_often_charset_filter_finalize() {
}

RIME_REGISTER_MODULE(often_charset_filter)
