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
#include "RoundRobin.hpp"

// A namespace to force these symbols being not exported in the shared
// library.
namespace jkbd {
  
  class Osc {
  public:
    enum Port {
	       Out_0, // = 0, use automatic enumeration
	       Out_1,
	       Count
    };

    Osc(double sample_rate);
    
    // Port buffers
    float* out[Port::Count]; // Works iff all ports are AudioPorts with out-flow
    const float* freq;

    void sample_rate(double sr);
    void run(std::uint32_t n_samples);
    
  private:
    //constexpr int frequency = 440;

    // Look-up table
// 44100 / 441 = 100.0 in frames
    const uint32_t table_length = 100;
    float lut[100]{
	   0.0, 0.06279051952931337, 0.12533323356430426, 0.1873813145857246, 0.2486898871648548, 0.3090169943749474, 0.3681245526846779, 0.42577929156507266, 0.4817536741017153, 0.5358267949789967, 0.5877852522924731, 0.6374239897486896, 0.6845471059286886, 0.7289686274214116, 0.7705132427757893, 0.8090169943749473, 0.8443279255020151, 0.8763066800438637, 0.9048270524660196, 0.9297764858882513, 0.9510565162951535, 0.9685831611286311, 0.9822872507286886, 0.9921147013144778, 0.9980267284282716, 1.0, 0.9980267284282716, 0.9921147013144779, 0.9822872507286887, 0.9685831611286311, 0.9510565162951536, 0.9297764858882515, 0.9048270524660195, 0.8763066800438635, 0.844327925502015, 0.8090169943749475, 0.7705132427757893, 0.7289686274214114, 0.6845471059286888, 0.6374239897486899, 0.5877852522924732, 0.535826794978997, 0.4817536741017152, 0.4257792915650729, 0.36812455268467814, 0.3090169943749475, 0.24868988716485524, 0.18738131458572457, 0.12533323356430454, 0.06279051952931358, 1.2246467991473532e-16, -0.06279051952931335, -0.12533323356430429, -0.18738131458572477, -0.24868988716485457, -0.3090169943749473, -0.3681245526846779, -0.42577929156507227, -0.4817536741017154, -0.5358267949789964, -0.5877852522924727, -0.6374239897486896, -0.6845471059286884, -0.7289686274214116, -0.7705132427757894, -0.8090169943749473, -0.8443279255020153, -0.8763066800438636, -0.9048270524660198, -0.9297764858882515, -0.9510565162951535, -0.9685831611286312, -0.9822872507286887, -0.9921147013144778, -0.9980267284282716, -1.0, -0.9980267284282716, -0.9921147013144779, -0.9822872507286887, -0.9685831611286311, -0.9510565162951536, -0.9297764858882516, -0.9048270524660199, -0.8763066800438634, -0.844327925502015, -0.8090169943749476, -0.7705132427757896, -0.7289686274214121, -0.684547105928689, -0.6374239897486896, -0.5877852522924734, -0.5358267949789971, -0.4817536741017161, -0.425779291565073, -0.36812455268467786, -0.3090169943749477, -0.24868988716485535, -0.18738131458572468, -0.12533323356430465, -0.06279051952931326
	     };
 
    double sr{ 8000.0 };
    std::uint32_t now{ 0 };
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
