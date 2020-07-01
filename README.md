# tinyalsa-cxx

A paper-thin C++ library for interfacing with ALSA.

### Examples

Reading information on a PCM.

```cxx
tinyalsa::pcm pcm;

auto open_result = pcm.open_capture_device();
if (open_result.failed()) {
  std::cerr << open_result << std::endl;
  return;
}

std::cout << pcm.get_info();
```
