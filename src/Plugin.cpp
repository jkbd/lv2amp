#include "Plugin.hpp"

namespace jkbd {

  inline float decibel_to_coef(float gain) {
    return ((gain) > -90.0f ? powf(10.0f, (gain) * 0.05f) : 0.0f);
  }
    
  void Plugin::sample_rate(double sr) {
    // TODO: assert 0 < sr <= 192000.0 ?
    Plugin::sr = sr;
  }

  
  Tremolo::Tremolo(double sample_rate) {
    sr = sample_rate;
  }

  void Tremolo::run(std::uint32_t n_samples) {
    for (std::uint32_t pos = 0; pos < n_samples; ++pos) {
      //out = in;
    }
  }
  
  static LV2_Handle instantiate(const LV2_Descriptor*     descriptor,
				double                    rate,
				const char*               bundle_path,
				const LV2_Feature* const* features)
  {
    Plugin* p = new Tremolo(rate);
    return static_cast<LV2_Handle>(p);
  }

  static void connect_port(LV2_Handle instance, std::uint32_t port, void* data)
  {
    Tremolo* p = static_cast<Tremolo*>(instance);
    switch (static_cast<Tremolo::PortID>(port)) {
    case Tremolo::PortID::in_0:
      p->in = static_cast<float*>(data);
      break;
    case Tremolo::PortID::out_0:
      p->out = static_cast<float*>(data);
      break;
    case Tremolo::PortID::freq:
      p->f = static_cast<const float*>(data);
      break;     
    }
  }
  
  static void activate(LV2_Handle instance) {
  }
  
  static void run(LV2_Handle instance, std::uint32_t n_samples) {
    Plugin* p = static_cast<Tremolo*>(instance);
    p->run(n_samples);
  }
  
  static void deactivate(LV2_Handle instance) {
  }
  
  static void cleanup(LV2_Handle instance) {
    delete static_cast<Tremolo*>(instance);
  }
  
  static const void* extension_data(const char* uri) {
    return nullptr;
  }  
} // namespace jkbd

// Force no C++ name mangeling
extern "C" {  
  const LV2_Descriptor* lv2_descriptor(std::uint32_t index) {
    switch (index) {
    case 0:  return &jkbd::descriptor;
    default: return nullptr;
    }
  }
}
