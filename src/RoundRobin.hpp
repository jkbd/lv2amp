// ISC License
//
// Copyright (c) 2018 Jakob Dübel <jkbd@posteo.de>
//
// Permission to use, copy, modify, and/or distribute this software
// for any purpose with or without fee is hereby granted, provided
// that the above copyright notice and this permission notice appear
// in all copies.
//
// THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL
// WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE
// AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR
// CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS
// OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT,
// NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
// CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.

#ifndef ROUND_ROBIN_H
#define ROUND_ROBIN_H

#include <cstdint>
#include <cmath>
#define _USE_MATH_DEFINES

#include "lv2.h"

namespace jkbd {
  
  template<std::uint32_t channels>
  class LinearRoundRobin {
  public:
    LinearRoundRobin(double sample_rate);

    // Set or get the frequency
    void frequency(double f);
    double frequency() const;

    // Note: This does not reset the frequency!
    void reset_phase();

    // Render the frame at position. Out must be in the form of
    // out[channel][position].
    void render(float* out[channels], const std::uint32_t position);
    
  private:
    double sr{ 8000.0 };
    
    // The current state of the oscillator
    float y;
    bool rise;
    std::uint32_t phase;

    // Amplitude
    const float a = 1.0;
    
    float freq;
  };

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

#endif // ROUND_ROBIN_H
