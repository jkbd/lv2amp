#include "LinearRoundRobin.hpp"

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
  
} // namespace jkbd
