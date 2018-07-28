#include "Osc.hpp"

namespace jkbd {

  template<std::uint32_t channels>
  LinearRoundRobin<channels>::LinearRoundRobin(double sample_rate) :
    freq(440.0), sr(sample_rate) {
    reset_phase();
  }

  template<std::uint32_t channels>
  void LinearRoundRobin<channels>::frequency(double f) {
    const float minimum = 0.1;
    f >= minimum ? freq = f : freq = minimum;
  }

  template<std::uint32_t channels>
  double LinearRoundRobin<channels>::frequency() const {
    return freq;
  }

  template<std::uint32_t channels>
  void LinearRoundRobin<channels>::reset_phase() {
    y = 0.0f;
    rise = true;
    phase = 0U;
  }

  template<std::uint32_t channels>
  void LinearRoundRobin<channels>::render(float* out[channels], const std::uint32_t position) {

    // Slope of the rising edge
    float m = (freq*2*a)/sr;
      
    // Ping pong between the bounds
    if (y >= a-m/2) {
      rise = false;
      phase = (phase+1) % channels;
    }
    if (y <= 0+m/2) {
      rise = true;
      phase = (phase+1) % channels;
    }
    rise ? y += (m) : y -= (m);

    // Distribute and copy to output
    for(std::uint32_t i = 0; i<channels; ++i) {
      switch((phase-i)%channels) {	  
      case 0:
      case 1:
	if (i%2 == 0) {
	  out[i][position] = a-y;
	} else {
	  out[i][position] = y;
	}
	break;
      default:
	out[i][position] = 0;
	break;
      }
    }
  }

  
  inline float decibel_to_coef(float gain) {
    return ((gain) > -90.0f ? powf(10.0f, (gain) * 0.05f) : 0.0f);
  }

  Osc::Osc(double sample_rate) : sr(sample_rate), lrr(sample_rate) {
  }

  
  void Osc::sample_rate(double sr) {
    // TODO: assert 0 < sr <= 192000.0 ?
    Osc::sr = sr;
  }

  void Osc::run(std::uint32_t n_samples) {
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
    case Osc::Port::Out_2:
      osc->out[Osc::Port::Out_2] = static_cast<float*>(data);
      break;
    case Osc::Port::Out_3:
      osc->out[Osc::Port::Out_3] = static_cast<float*>(data);
      break;
    case Osc::Port::Freq:
      osc->freq = static_cast<const float*>(data);
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
