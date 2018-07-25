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

    // FAUST generated code... how does it fake cosine?
    const float PI2T = 2*M_PI / 48000.0f;
    for (uint32_t pos = 0; pos < n_samples; ++pos) {

      f[0] = s + (0.999000013f * f[1]);
      //const float v = PI2T * f[0];

      const float k = cos(PI2T * f[0]);
      const float a = sin(PI2T * f[0])/sin(PI2T * f[1]);

      x[0] = k*x[1] + y[1];
      y[0] = k*x[0] - x[1];
      
      sine[pos] = x[0];
      cosine[pos] = y[0];

      f[1] = f[0];
      x[1] = x[0];
      y[1] = y[0];
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
