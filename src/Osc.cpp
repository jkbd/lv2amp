#include "Osc.hpp"

namespace jkbd {


  
  inline float decibel_to_coef(float gain) {
    return ((gain) > -90.0f ? powf(10.0f, (gain) * 0.05f) : 0.0f);
  }

  Osc::Osc(double sample_rate) : sr(sample_rate) {
  }

  
  void Osc::sample_rate(double sr) {
    // TODO: assert 0 < sr <= 192000.0 ?
    Osc::sr = sr;
  }

  void Osc::run(std::uint32_t n_samples) {    
    for (std::uint32_t pos = 0; pos < n_samples; ++pos) {
      std::uint32_t base = (now + pos) % table_length;
      std::uint32_t harmonic = 2*(now + pos) % table_length;
      
      out[Osc::Port::Out_0][pos] = lut[base];
      out[Osc::Port::Out_1][pos] = lut[harmonic];
    }
    now = (now + n_samples) % table_length;
  }
  
  static LV2_Handle instantiate(const LV2_Descriptor*     descriptor,
				double                    rate,
				const char*               bundle_path,
				const LV2_Feature* const* features)
  {
    Osc* osc = new Osc(rate);
    return static_cast<LV2_Handle>(osc);
  }

  static void connect_port(LV2_Handle instance, std::uint32_t port, void* data)
  {
    Osc* osc = static_cast<Osc*>(instance);
    switch (static_cast<Osc::Port>(port)) {
    case Osc::Port::Out_0:
      osc->out[Osc::Port::Out_0] = static_cast<float*>(data);
      break;
    case Osc::Port::Out_1:
      osc->out[Osc::Port::Out_1] = static_cast<float*>(data);
      break;     
    }
  }
  
  static void activate(LV2_Handle instance) {
  }
  
  static void run(LV2_Handle instance, std::uint32_t n_samples) {
    Osc* osc = static_cast<Osc*>(instance);
    osc->run(n_samples);
  }
  
  static void deactivate(LV2_Handle instance) {
  }
  
  static void cleanup(LV2_Handle instance) {
    delete static_cast<Osc*>(instance);
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
