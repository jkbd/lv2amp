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

#ifndef OSC_H
#define OSC_H

#include <cstdint>
#include <cmath>
#define _USE_MATH_DEFINES

#include "lv2.h"

#include "LinearRoundRobin.hpp"

// A namespace to force these symbols being not exported in the shared
// library.
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

  
  class Osc {
  public:
    enum Port {
	       Out_0, // = 0, use automatic enumeration
	       Out_1,
	       Out_2,
	       Out_3,
	       Freq
    };

    Osc(double sample_rate);
    
    // Port buffers
    float* out[4];
    const float* freq;

    void sample_rate(double sr);
    void run(std::uint32_t n_samples);
    
  private:
    float f[2]{ 0.0f, 0.0f };
    double sr{ 8000.0 };

    LinearRoundRobin<4> lrr;
  };

  static LV2_Handle
  instantiate(const LV2_Descriptor*     descriptor,
	      double                    rate,
	      const char*               bundle_path,
	      const LV2_Feature* const* features);
  
  static void
  connect_port(LV2_Handle instance,
	       std::uint32_t   port,
	       void*      data);
  
  static void
  activate(LV2_Handle instance);
  
  static void
  run(LV2_Handle instance, std::uint32_t n_samples);
  
  static void
  deactivate(LV2_Handle instance);
  
  static void
  cleanup(LV2_Handle instance);
  
  static const void*
  extension_data(const char* uri);

  static constexpr char uri[] = "https://github.com/jkbd/tri";
  
  static const LV2_Descriptor
  descriptor = {
	      jkbd::uri,
	      instantiate, // instantiate
	      connect_port, // connect_port,
	      activate, // activate
	      run, // run
	      deactivate, // deactivate
	      cleanup, // cleanup
	      extension_data // extension_data	      
  };
} // namespace jkbd

// Force no C++ name mangeling
extern "C" {  
  const LV2_Descriptor* lv2_descriptor(std::uint32_t index);
}

#endif // OSC_H
