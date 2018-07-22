// MIT License
//
// Copyright (c) 2018 Jakob Dübel <jkbd@posteo.de>
// Copyright (c) 2006-2016 David Robillard <d@drobilla.net>
// Copyright (c) 2006 Steve Harris <steve@plugin.org.uk>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#ifndef AMP_H
#define AMP_H

#include <cmath>
#include "lv2.h"

// A namespace to force these symbols being not exported in the shared
// library.
namespace jkbd {
  class AmpPlugin {
  public:
    enum Port {
	       GAIN = 0,
	       INPUT = 1,
	       OUTPUT = 2
    };

    // Port buffers
    const float* gain;
    const float* input;
    float*       output;
  };

  static LV2_Handle
  instantiate(const LV2_Descriptor*     descriptor,
	      double                    rate,
	      const char*               bundle_path,
	      const LV2_Feature* const* features);
  
  static void
  connect_port(LV2_Handle instance,
	       uint32_t   port,
	       void*      data);
  
  static void
  activate(LV2_Handle instance);
  
  static void
  run(LV2_Handle instance, uint32_t n_samples);
  
  static void
  deactivate(LV2_Handle instance);
  
  static void
  cleanup(LV2_Handle instance);
  
  static const void*
  extension_data(const char* uri);

  static constexpr char uri[] = "https://github.com/jkbd/lv2amp";
  
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
  const LV2_Descriptor* lv2_descriptor(uint32_t index);
}

#endif // AMP_H
