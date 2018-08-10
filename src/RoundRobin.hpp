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

  /**
   * This oscilator generates a traveling triangle signal so than the
   * sum of all outputs is `1.0` for each frame. The number of
   * channels is given as a template parameter, which enables the
   * compiler to check if you are dealing with the asserted number of
   * channels.
   */
  template<std::uint32_t channels>
  class LinearRoundRobin {
  public:
    LinearRoundRobin(double sample_rate);

    // Set or get the frequency
    void frequency(double f);
    double frequency() const;

    // Note: This does not reset the frequency!
    void reset_phase();

    // Render the frame at position. `Out` must be in the form of
    // out[channel][position].
    void render(float* out[channels], const std::uint32_t position);
    
  private:
    double sr{ 8000.0 };
    
    // The current state of the oscillator
    float y;
    enum class SlopeState { rise, fall, silent };
    SlopeState channel_slope_state[channels];
    std::uint32_t phase;

    // Amplitude
    const float a = 1.0;    
    float freq;
  };
  
  template<std::uint32_t channels>
  LinearRoundRobin<channels>::LinearRoundRobin(double sample_rate) :
    freq(440.0), sr(sample_rate) {
    reset_phase();

    // Initialize the state per channel
    channel_slope_state[0] = SlopeState::rise;
    for (int i = 1; i < channels-1; ++i) {
      channel_slope_state[i] = SlopeState::silent;
    }
    channel_slope_state[channels-1] = SlopeState::fall;
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
    phase = 0U;
  }

  template<std::uint32_t channels>
  void LinearRoundRobin<channels>::render(float* out[channels], const std::uint32_t position) {

    // Slope of the rising edge
    float m = (freq*2*a)/sr;
      
    if (y < a-m) {
      y += m;
    } else {
      y = 0.0f;
      phase = (phase+1) % channels;
    }

    // Distribute and copy to output
    for(std::uint32_t i = 0; i<channels; ++i) {
      const unsigned int ch = (channels-phase+i)%channels;
      switch(channel_slope_state[ch]) {
      case SlopeState::rise:
	out[i][position] = y;
	break;
      case SlopeState::fall:
	out[i][position] = a-y;
	break;
      default: // SlopeState::silent:
	out[i][position] = 0;
	break;
      }
    }
  }
  
} // namespace jkbd

#endif // ROUND_ROBIN_H
