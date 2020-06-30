#include <tinyalsa/interleaved_reader.hpp>

namespace tinyalsa {

int interleaved_pcm_reader::open(size_type card, size_type device, bool non_blocking) noexcept
{
  return pcm::open_capture(card, device, non_blocking);
}

} // namespace tinyalsa
