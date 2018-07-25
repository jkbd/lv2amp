#include "Osc.hpp"

namespace jkbd {

  inline float decibel_to_coef(float gain) {
    return ((gain) > -90.0f ? powf(10.0f, (gain) * 0.05f) : 0.0f);
  }
  
  void Osc::run(uint32_t n_samples) {

    // Smooth the control parameter
    float s = 0.00100000005f * freq[0];
    
    // See Smith & Cook "The Second-Order Digital Waveguide Oscillator" 1992,
    // https://ccrma.stanford.edu/~jos/wgo/wgo.pdf, p. 2    

    const float PIT = M_PI / 48000.0f;
    for (uint32_t pos = 0; pos < n_samples; ++pos) {
      f[static_cast<int>(index)] = s + (0.999000013f * f[static_cast<int>(!index)]);

      // "Magic Circle" algorithm
      const float e = 2*sin(PIT * f[static_cast<int>(index)]);
      x[static_cast<int>(index)] = x[static_cast<int>(!index)] + e*y[static_cast<int>(!index)];
      y[static_cast<int>(index)] = -e*x[static_cast<int>(index)] + y[static_cast<int>(!index)];
      
      sine[pos] = x[static_cast<int>(index)];
      cosine[pos] = y[static_cast<int>(index)];

      index = !index;
    }
  }
  
  static LV2_Handle instantiate(const LV2_Descriptor*     descriptor,
				double                    rate,
				const char*               bundle_path,
				const LV2_Feature* const* features)
  {
    Osc* osc = new Osc();
    return static_cast<LV2_Handle>(osc);
  }

  static void connect_port(LV2_Handle instance, uint32_t port, void* data)
  {
    Osc* osc = static_cast<Osc*>(instance);
    switch (static_cast<Osc::Port>(port)) {
    case Osc::Port::SINE:
      osc->sine = static_cast<float*>(data);
      break;
    case Osc::Port::COSINE:
      osc->cosine = static_cast<float*>(data);
      break;
    case Osc::Port::FREQ:
      osc->freq = static_cast<const float*>(data);
      break;     
    }
  }
  
  static void activate(LV2_Handle instance) {
  }
  
  static void run(LV2_Handle instance, uint32_t n_samples) {
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
