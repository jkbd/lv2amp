#include "Amp.h"

// anonymous namespace to force these symbols not being exported in
// the shared library.
namespace {
  static int unvisible_variable;
  
  static void unvisible_function() {
    unvisible_variable = 42;
  };

  class Testy {
  public:
    int i;
  private:
    double o;
  };
}

namespace jkbd {
  class AmpPlugin {
  public:
    static int n() { return 42; };

    static void cleanup(LV2_Handle instance) {
      //free(instance);      
    }
  };

  static const void* extension_data(const char* uri) {
    return nullptr;
  }
  
  static const LV2_Descriptor descriptor = {
					    "Hello",
					    nullptr,
					    nullptr,
					    nullptr,
					    nullptr,
					    nullptr,
					    AmpPlugin::cleanup,
					    extension_data
  };
}

// Force no C++ name mangeling
extern "C" {  
  const LV2_Descriptor* lv2_descriptor(uint32_t index) {
    switch (index) {
    case 0:  return &jkbd::descriptor;
    default: return nullptr;
    }
  }
}
