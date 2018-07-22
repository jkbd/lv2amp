#include "Amp.hpp"

namespace jkbd {

  static LV2_Handle
  instantiate(const LV2_Descriptor*     descriptor,
	      double                    rate,
	      const char*               bundle_path,
	      const LV2_Feature* const* features)
  {
    AmpPlugin* amp = new AmpPlugin();
    return static_cast<LV2_Handle>(amp);
  }

  static void
  connect_port(LV2_Handle instance,
	       uint32_t   port,
	       void*      data)
  {
    AmpPlugin* amp = static_cast<AmpPlugin*>(instance);
    switch (static_cast<AmpPlugin::Port>(port)) {
    case AmpPlugin::Port::GAIN:
      amp->gain = static_cast<const float*>(data);
      break;
    case AmpPlugin::Port::INPUT:
      amp->input = static_cast<const float*>(data);
      break;
    case AmpPlugin::Port::OUTPUT:
      amp->output = static_cast<float*>(data);
      break;
    }    
  }
  
  static void
  activate(LV2_Handle instance) {
  }

  inline float decibel_to_coef(float gain) {
    return ((gain) > -90.0f ? powf(10.0f, (gain) * 0.05f) : 0.0f);
  }
  
  static void
  run(LV2_Handle instance, uint32_t n_samples) {
    const AmpPlugin* amp = static_cast<const AmpPlugin*>(instance);

    const float        gain   = *(amp->gain);
    const float* const input  = amp->input;
    float* const       output = amp->output;
    
    const float coef = decibel_to_coef(gain);
    
    for (uint32_t pos = 0; pos < n_samples; ++pos) {
      output[pos] = input[pos] * coef;
    }
  }
  
  static void
  deactivate(LV2_Handle instance) {
  }
  
  static void
  cleanup(LV2_Handle instance) {
    delete static_cast<AmpPlugin*>(instance);
  }
  
  static const void*
  extension_data(const char* uri) {
    return nullptr;
  }  
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
