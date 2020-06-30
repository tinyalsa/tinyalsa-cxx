#ifndef TINYALSA_INCLUDE_TINYALSA_PCM_HPP
#define TINYALSA_INCLUDE_TINYALSA_PCM_HPP

#include <tinyalsa/types.hpp>

namespace tinyalsa {

class pcm_impl;

/// This is the base of any kind of PCM.
/// The base class is responsible for interfacing
/// with the file descriptor of the PCM device.
class pcm
{
  /// A pointer to the implementation data.
  pcm_impl* self = nullptr;
public:
  /// Indicates the number assigned to
  /// invalid file descriptors. These can
  /// appear when either the PCM hasn't been
  /// opened yet or it failed to open.
  static constexpr int invalid_fd() noexcept
  {
    return -1;
  }
  /// Constructs an unopened PCM device.
  pcm() noexcept;
  /// Moves a PCM from one variable to another.
  ///
  /// @param other The PCM to be moved.
  pcm(pcm&& other) noexcept;
  /// Closes the PCM device.
  virtual ~pcm();
  /// Closes the PCM device.
  /// This function does nothing if
  /// the PCM was already closed or was never opened.
  ///
  /// @return Zero on success. On failure, a copy
  /// of errno is returned.
  int close() noexcept;
  /// Accesses the underlying file descriptor of the PCM.
  /// This is useful when polling the file descriptor.
  ///
  /// @return The file descriptor of the PCM.
  /// If the PCM has not been opened yet,
  /// then @ref pcm::invalid_id is returned instead.
  int get_file_descriptor() const noexcept;
  /// Indicates whether or not the PCM is opened.
  ///
  /// @return True if the PCM is opened,
  /// false if it is not.
  bool is_open() const noexcept;
  /// Prepares the PCM to be started.
  ///
  /// @return On success, zero is returned.
  /// On failure, a copy of errno is returned.
  virtual int prepare() noexcept;
  /// Starts the PCM. This can have different
  /// meanings depending on whether the PCM is
  /// a capture device or a playback device.
  ///
  /// If it is a playback device, this function
  /// tells the audio device to begin playing audio
  /// to the speaker. Ideally, there would be audio
  /// data buffered to the device at this point.
  ///
  /// If it is a capture device, this function
  /// tells the audio device to begin sending audio
  /// data to the host.
  ///
  /// @return On success, zero is returned.
  /// On failure, a copy of errno is returned.
  virtual int start() noexcept;
  /// Stops either the playback or capture loop
  /// of the audio device. Any buffered audio that
  /// exist at the point of calling this function
  /// is lost.
  ///
  /// @return On success, zero is returned.
  /// On failure, a copy of errno is returned instead.
  virtual int drop() noexcept;
protected:
  /// Opens a capture PCM.
  ///
  /// @param card The index of the card to open the PCM from.
  /// @param device The index of the device to open the PCM from.
  /// @param non_blocking Whether or not the PCM should be opened in non-blocking mode.
  int open_capture(size_type card, size_type device, bool non_blocking) noexcept;
  /// Opens a playback PCM.
  ///
  /// @param card The index of the card to open the PCM from.
  /// @param device The index of the device to open the PCM from.
  /// @param non_blocking Whether or not the PCM should be opened in non-blocking mode.
  int open_playback(size_type card, size_type device, bool non_blocking) noexcept;
};

} // namespace tinyalsa

#endif // TINYALSA_INCLUDE_TINYALSA_PCM_HPP
