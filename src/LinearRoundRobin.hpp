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

#ifndef LINEAR_ROUND_ROBIN_H
#define LINEAR_ROUND_ROBIN_H

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

} // namespace jkbd

// Class factories
extern "C" {  
  jkbd::LinearRoundRobin<4>* lrr_create() {
    return new jkbd::LinearRoundRobin<4>(48000.0f);
  }

  void lrr_destroy(jkbd::LinearRoundRobin<4>* llr) {
    delete llr;
  }
}


#endif // LINEAR_ROUND_ROBIN_H
