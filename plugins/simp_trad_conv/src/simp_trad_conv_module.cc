#include <rime/component.h>
#include <rime/registry.h>
#include <rime_api.h>

#include "simp_trad_conv.h"

using namespace rime;

static void rime_simp_trad_conv_initialize() {
  Registry &r = Registry::instance();
  r.Register("simp_trad_conv", new Component<SimpTradConv>);
}

static void rime_simp_trad_conv_finalize() {
}

RIME_REGISTER_MODULE(simp_trad_conv)
