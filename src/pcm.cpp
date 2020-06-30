#include <tinyalsa/pcm.hpp>

#include <new>

#include <errno.h>
#include <fcntl.h>
#include <sound/asound.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>

namespace tinyalsa {

/// Contains all the implementation data for a PCM.
class pcm_impl final
{
  friend pcm;
  /// The file descriptor for the opened PCM.
  int fd = pcm::invalid_fd();
  /// Opens a PCM by a specified path.
  ///
  /// @param path The path of the PCM to open.
  ///
  /// @param non_blocking Whether or not the call to ::open
  /// should block if the device is not available.
  ///
  /// @return On success, zero is returned.
  /// On failure, a copy of errno is returned.
  int open_by_path(const char* path, bool non_blocking) noexcept;
};

namespace {

/// This function allocates an instance
/// of the implementation data, if it is not null.
///
/// @param impl The current pointer to the implementation data.
/// If this is null then a new one is allocated.
///
/// @return A pointer to either an existing implementation
/// data instance, a new implementation data instance, or
/// in the case of a memory allocation failure a null pointer.
pcm_impl* lazy_init(pcm_impl* impl) noexcept
{
  if (impl) {
    return impl;
  }

  return new (std::nothrow) pcm_impl();
}

} // namespace

pcm::pcm() noexcept : self(nullptr) { }

pcm::pcm(pcm&& other) noexcept : self(other.self)
{
  other.self = nullptr;
}

pcm::~pcm()
{
  close();
  delete self;
}

int pcm::close() noexcept
{
  if (!self) {
    return 0;
  }

  if (self->fd != invalid_fd()) {

    auto result = ::close(self->fd);

    self->fd = invalid_fd();

    if (result == -1) {
      return errno;
    }
  }

  return 0;
}

bool pcm::is_open() const noexcept
{
  if (!self) {
    return false;
  }

  return self->fd != pcm::invalid_fd();
}

int pcm::prepare() noexcept
{
  if (!self) {
    return ENOENT;
  }

  auto result = ::ioctl(self->fd, SNDRV_PCM_IOCTL_PREPARE);
  if (result < 0) {
    return errno;
  }

  return 0;
}

int pcm::start() noexcept
{
  if (!self) {
    return ENOENT;
  }

  auto result = ::ioctl(self->fd, SNDRV_PCM_IOCTL_START);
  if (result < 0) {
    return errno;
  }

  return 0;
}

int pcm::drop() noexcept
{
  if (!self) {
    return ENOENT;
  }

  auto result = ::ioctl(self->fd, SNDRV_PCM_IOCTL_DROP);
  if (result < 0) {
    return errno;
  }

  return 0;
}

int pcm::open_capture(size_type card, size_type device, bool non_blocking) noexcept
{
  self = lazy_init(self);
  if (!self) {
    return ENOMEM;
  }

  char path[256];

  snprintf(path, sizeof(path), "/dev/snd/pcmC%luD%luc",
           (unsigned long) card,
           (unsigned long) device);

  return self->open_by_path(path, non_blocking);
}

int pcm::open_playback(size_type card, size_type device, bool non_blocking) noexcept
{
  self = lazy_init(self);
  if (!self) {
    return ENOMEM;
  }

  char path[256];

  snprintf(path, sizeof(path), "/dev/snd/pcmC%luD%lup",
           (unsigned long) card,
           (unsigned long) device);

  return self->open_by_path(path, non_blocking);
}

int pcm_impl::open_by_path(const char* path, bool non_blocking) noexcept
{
  if (fd != pcm::invalid_fd()) {
    ::close(fd);
  }

  fd = ::open(path, non_blocking ? (O_RDWR | O_NONBLOCK) : O_RDWR);
  if (fd < 0) {
    fd = pcm::invalid_fd();
    return errno;
  } else {
    return 0;
  }
}

} // namespace tinyalsa
