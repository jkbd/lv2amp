#include "Osc.hpp"

namespace jkbd {

  inline float decibel_to_coef(float gain) {
    return ((gain) > -90.0f ? powf(10.0f, (gain) * 0.05f) : 0.0f);
  }

  void Osc::sample_rate(double sr) {
    // TODO: assert 0 < sr <= 192000.0 ?
    Osc::sr = sr;
  }
  
  void Osc::run(uint32_t n_samples) {
    // Smooth frequency parameter
    const float alpha = 0.001f;
    float s = alpha * freq[0];
    
    // Amplitude
    const float a = 1.0;

    for (uint32_t pos = 0; pos < n_samples; ++pos) {
      f[0] = s + ((1-alpha) * f[1]);

      // Slope of the rising edge
      float m = (f[0]*2*a)/sr;
      
      // Ping pong between the bounds
      if (y >= a-m/2) {
	rise = false;
	p = (p + 1)%phases;
      }
      if (y <= 0+m/2) {
	rise = true;
	p = (p + 1)%phases;
      }
      rise ? y += (m) : y -= (m);

      // Copy to output
      switch(p) {
      case 0:
	out1[pos] = a-y;
	out2[pos] = 0;
	out3[pos] = 0;
	out4[pos] = y;
	break;
      case 1:
	out1[pos] = a-y;
	out2[pos] = y;
	out3[pos] = 0;
	out4[pos] = 0;
	break;
      case 2:
	out1[pos] = 0;
	out2[pos] = y;
	out3[pos] = a-y;
	out4[pos] = 0;
	break;
      case 3:
	out1[pos] = 0;
	out2[pos] = 0;
	out3[pos] = a-y;
	out4[pos] = y;
	break;
      }

      f[1] = f[0];
    }
  }
  
  static LV2_Handle instantiate(const LV2_Descriptor*     descriptor,
				double                    rate,
				const char*               bundle_path,
				const LV2_Feature* const* features)
  {
    Osc* osc = new Osc();
    osc->sample_rate(rate);
    return static_cast<LV2_Handle>(osc);
  }

  static void connect_port(LV2_Handle instance, uint32_t port, void* data)
  {
    Osc* osc = static_cast<Osc*>(instance);
    switch (static_cast<Osc::Port>(port)) {
    case Osc::Port::OUTPUT1:
      osc->out1 = static_cast<float*>(data);
      break;
    case Osc::Port::OUTPUT2:
      osc->out2 = static_cast<float*>(data);
      break;
    case Osc::Port::OUTPUT3:
      osc->out3 = static_cast<float*>(data);
      break;
    case Osc::Port::OUTPUT4:
      osc->out4 = static_cast<float*>(data);
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
