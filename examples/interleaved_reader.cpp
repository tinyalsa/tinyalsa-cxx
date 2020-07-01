#include <tinyalsa.hpp>

#include <cstdio>
#include <cstdlib>

int main()
{
  unsigned short int frames[1024];

  constexpr const unsigned int frame_count = sizeof(frames) / (2 * sizeof(frames[0]));

  tinyalsa::interleaved_pcm_reader pcm_reader;

  auto open_result = pcm_reader.open();
  if (open_result.failed()) {
    std::printf("Failed to open PCM: %s\n", open_result.error_description());
    return EXIT_FAILURE;
  }

  auto setup_result = pcm_reader.setup();
  if (setup_result.failed()) {
    std::printf("Failed to setup PCM: %s\n", setup_result.error_description());
    return EXIT_FAILURE;
  }

  auto prepare_result = pcm_reader.prepare();
  if (prepare_result.failed()) {
    std::printf("Failed to prepare PCM: %s\n", prepare_result.error_description());
    return EXIT_FAILURE;
  }

  auto start_result = pcm_reader.start();
  if (start_result.failed()) {
    std::printf("Failed to start PCM: %s\n", start_result.error_description());
    return EXIT_FAILURE;
  }

  auto read_result = pcm_reader.read_unformatted(frames, frame_count);
  if (read_result.failed()) {
    std::printf("Failed to read PCM: %s\n", read_result.error_description());
    return EXIT_FAILURE;
  }

  std::printf("Read %lu frames from PCM.\n", read_result.unwrap());

  return EXIT_SUCCESS;
}
