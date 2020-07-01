#include <tinyalsa.hpp>

#include <cstdio>
#include <cstdlib>

int main()
{
  unsigned short int frames[1024];

  tinyalsa::interleaved_pcm_reader pcm_reader;

  pcm_reader.setup();

  pcm_reader.prepare();

  pcm_reader.start();

  auto open_result = pcm_reader.open();
  if (open_result.failed()) {
    std::printf("Failed to open PCM: %s\n", open_result.error_description());
    return EXIT_FAILURE;
  }

  auto read_result = pcm_reader.read_unformatted(frames, 1024);

  std::printf("result: %s\n", read_result.error_description());

  return EXIT_SUCCESS;
}
