#include <rime/component.h>
#include <rime/registry.h>
#include <rime_api.h>

#include "speller_r.h"

using namespace rime;

static void rime_speller_r_initialize() {
  Registry &r = Registry::instance();
  r.Register("speller_r", new Component<SpellerR>);
}

static void rime_speller_r_finalize() {
}

RIME_REGISTER_MODULE(speller_r)
