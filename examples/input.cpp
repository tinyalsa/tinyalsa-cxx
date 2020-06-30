#include <tinyalsa/interleaved_reader.hpp>

#include <cstdlib>

int main()
{
  namespace snd = tinyalsa;

  snd::interleaved_pcm_reader pcm_reader;

  pcm_reader.open();

  return EXIT_SUCCESS;
}
