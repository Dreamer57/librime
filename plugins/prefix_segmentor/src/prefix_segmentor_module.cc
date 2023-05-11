#include <rime/component.h>
#include <rime/registry.h>
#include <rime_api.h>

#include "prefix_segmentor.h"

using namespace rime;

static void rime_prefix_segmentor_initialize() {
  Registry &r = Registry::instance();
  r.Register("prefix_segmentor", new Component<PrefixSegmentor>);
}

static void rime_prefix_segmentor_finalize() {
}

RIME_REGISTER_MODULE(prefix_segmentor)
