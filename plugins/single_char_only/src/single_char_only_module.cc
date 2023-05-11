#include <rime/component.h>
#include <rime/registry.h>
#include <rime_api.h>

#include "single_char_only.h"

using namespace rime;

static void rime_single_char_only_initialize() {
  Registry &r = Registry::instance();
  r.Register("single_char_only", new Component<SingleCharOnly>);
}

static void rime_single_char_only_finalize() {
}

RIME_REGISTER_MODULE(single_char_only)
