#include <tinyalsa.hpp>

#include <iostream>

#include <cstdlib>

int main()
{
  tinyalsa::pcm pcm;

  auto open_result = pcm.open_capture_device();
  if (open_result.failed()) {
    std::cerr << "Failed to open PCM: " << open_result << std::endl;
    return EXIT_FAILURE;
  }

  auto info_result = pcm.get_info();
  if (info_result.failed()) {
    std::cerr << "Failed to get PCM info: " << info_result << std::endl;
    return EXIT_FAILURE;
  }

  std::cout << info_result.unwrap();

  return EXIT_SUCCESS;
}
