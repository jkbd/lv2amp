#include "Plugin.hpp"

namespace jkbd {


  
  inline float decibel_to_coef(float gain) {
    return ((gain) > -90.0f ? powf(10.0f, (gain) * 0.05f) : 0.0f);
  }

  Plugin::Plugin(double sample_rate) : sr(sample_rate), lrr(sample_rate) {
  }

  
  void Plugin::sample_rate(double sr) {
    // TODO: assert 0 < sr <= 192000.0 ?
    Plugin::sr = sr;
  }

  void Plugin::run(std::uint32_t n_samples) {
    // Smooth frequency parameter
    const float alpha = 0.001f;
    float s = alpha * freq[0];
    
    for (std::uint32_t pos = 0; pos < n_samples; ++pos) {
      f[0] = s + ((1-alpha) * f[1]);

      lrr.frequency(f[0]);
      lrr.render(out, pos);
      
      f[1] = f[0];
    }
  }
  
  static LV2_Handle instantiate(const LV2_Descriptor*     descriptor,
				double                    rate,
				const char*               bundle_path,
				const LV2_Feature* const* features)
  {
    Plugin* osc = new Plugin(rate);
    return static_cast<LV2_Handle>(osc);
  }

  static void connect_port(LV2_Handle instance, std::uint32_t port, void* data)
  {
    Plugin* osc = static_cast<Plugin*>(instance);
    switch (static_cast<Plugin::PortID>(port)) {
    case Plugin::PortID::Out_0:
      osc->out[Plugin::PortID::Out_0] = static_cast<float*>(data);
      break;
    case Plugin::PortID::Out_1:
      osc->out[Plugin::PortID::Out_1] = static_cast<float*>(data);
      break;
    case Plugin::PortID::Out_2:
      osc->out[Plugin::PortID::Out_2] = static_cast<float*>(data);
      break;
    case Plugin::PortID::Out_3:
      osc->out[Plugin::PortID::Out_3] = static_cast<float*>(data);
      break;
    case Plugin::PortID::Freq:
      osc->freq = static_cast<const float*>(data);
      break;     
    }
  }
  
  static void activate(LV2_Handle instance) {
  }
  
  static void run(LV2_Handle instance, std::uint32_t n_samples) {
    Plugin* osc = static_cast<Plugin*>(instance);
    osc->run(n_samples);
  }
  
  static void deactivate(LV2_Handle instance) {
  }
  
  static void cleanup(LV2_Handle instance) {
    delete static_cast<Plugin*>(instance);
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
