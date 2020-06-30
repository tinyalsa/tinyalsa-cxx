#ifndef TINYALSA_INCLUDE_TINYALSA_INTERLEAVED_READER_HPP
#define TINYALSA_INCLUDE_TINYALSA_INTERLEAVED_READER_HPP

#include <tinyalsa/pcm.hpp>

namespace tinyalsa {

class interleaved_pcm_reader final : public pcm
{
public:
  /// Opens a new PCM reader.
  ///
  /// @param card The index of the card to open.
  /// @param device The index of the device to open.
  /// @param non_blocking Whether or not the call
  /// should block if the device is not available.
  int open(size_type card = 0, size_type device = 0, bool non_blocking = false) noexcept;
};

} // namespace tinyalsa

#endif // TINYALSA_INCLUDE_TINYALSA_INTERLEAVED_READER_HPP
