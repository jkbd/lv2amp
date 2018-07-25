#include "Amp.hpp"

namespace jkbd {

  inline float decibel_to_coef(float gain) {
    return ((gain) > -90.0f ? powf(10.0f, (gain) * 0.05f) : 0.0f);
  }
  
  void AmpPlugin::run(uint32_t n_samples) {

    const float f = freq[0];
    
    // See Smith & Cook "The Second-Order Digital Waveguide Oscillator" 1992,
    // https://ccrma.stanford.edu/~jos/wgo/wgo.pdf, p. 2    

    // FAUST generated code... how does it fake cosine?
    const float PI2T = 2*M_PI / 48000.0f;
    for (uint32_t pos = 0; pos < n_samples; ++pos) {
      const float v = PI2T * f;

      i[0] = 1;

      z[0] =  z[1] + (v * -x[1]);
      y[0] = (y[1] + (v *  z[0])) + float(1 - i[1]); // Einmal i[1]==0. Nachfolgend immer 1?
      x[0] = y[0];
      
      output[pos] = x[0];
      
      i[1] = i[0];

      z[1] = z[0];
      y[1] = y[0];
      x[1] = x[0];      
    }
  }
  
  static LV2_Handle instantiate(const LV2_Descriptor*     descriptor,
				double                    rate,
				const char*               bundle_path,
				const LV2_Feature* const* features)
  {
    AmpPlugin* amp = new AmpPlugin();
    return static_cast<LV2_Handle>(amp);
  }

  static void connect_port(LV2_Handle instance, uint32_t port, void* data)
  {
    AmpPlugin* amp = static_cast<AmpPlugin*>(instance);
    switch (static_cast<AmpPlugin::Port>(port)) {
    case AmpPlugin::Port::OUTPUT:
      amp->output = static_cast<float*>(data);
      break;
    case AmpPlugin::Port::FREQ:
      amp->freq = static_cast<const float*>(data);
      break;     
    }    
  }
  
  static void activate(LV2_Handle instance) {
  }
  
  static void run(LV2_Handle instance, uint32_t n_samples) {
    AmpPlugin* amp = static_cast<AmpPlugin*>(instance);
    amp->run(n_samples);
  }
  
  static void deactivate(LV2_Handle instance) {
  }
  
  static void cleanup(LV2_Handle instance) {
    delete static_cast<AmpPlugin*>(instance);
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
