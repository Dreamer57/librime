#include <rime/component.h>
#include <rime/registry.h>
#include <rime_api.h>

#include "date_translator.h"

using namespace rime;

static void rime_date_translator_initialize() {
  Registry &r = Registry::instance();
  r.Register("date_translator", new Component<DateTranslator>);
}

static void rime_date_translator_finalize() {
}

RIME_REGISTER_MODULE(date_translator)
