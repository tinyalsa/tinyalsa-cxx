# tinyalsa-cxx

A paper-thin C++ library for interfacing with ALSA.

### Examples

Reading information on a PCM:

```cxx
tinyalsa::pcm pcm;

auto open_result = pcm.open_capture_device();
if (open_result.failed()) {
  std::cerr << open_result << std::endl;
  return;
}

std::cout << pcm.get_info();
```

Reading audio data from a PCM:
```cxx
// This is a 16-bit integer stereo buffer.
unsigned short int frames[1024];
unsigned int frame_count = 2;

// Error checking omitted for brevity.
tinyalsa::interleaved_pcm_reader pcm_reader;
pcm_reader.open(); // Choose the device to open.
pcm_reader.setup(); // Apply hardware parameters
pcm_reader.prepare(); // Prepare the PCM to be started
pcm_reader.start(); // Begin recording
pcm_reader.read_unformatted(frames, frame_count); // Read recorded data
```
