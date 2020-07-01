#include <tinyalsa.hpp>

#include <algorithm>
#include <new>
#include <type_traits>

#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <sound/asound.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>

namespace tinyalsa {

//================//
// Section: Masks //
//================//

namespace {

/// The type used to identify hardware parameters.
using parameter_name = int;

/// Represents an indexed mask parameter.
///
/// @tparam param The index of the mask parameter.
template <parameter_name param>
struct mask_ref final
{
  /// A type definition for values assigned to mask parameters.
  using value_type = std::remove_reference<decltype(snd_mask::bits[0])>::type;
  /// Indicates whether or not this is a valid mask parameter.
  ///
  /// @return True if it's a valid hardware parameter, false if it's not.
  static constexpr bool out_of_range() noexcept
  {
    return (param < SNDRV_PCM_HW_PARAM_FIRST_MASK)
        || (param > SNDRV_PCM_HW_PARAM_LAST_MASK);
  }
  /// Initializes the mask.
  ///
  /// @param hw_params The hardware parameters instance to initialize the mask in.
  static constexpr void init(snd_pcm_hw_params& hw_params) noexcept
  {
    static_assert(!out_of_range(), "Not a mask parameter.");

    auto& mask = hw_params.masks[param - SNDRV_PCM_HW_PARAM_FIRST_MASK];
    mask.bits[0] = std::numeric_limits<value_type>::max();
    mask.bits[1] = std::numeric_limits<value_type>::max();
  }
  /// Sets the value of a hardware mask parameter.
  ///
  /// @param hw_params The hardware parameters containing the mask to modify.
  /// @param value The value to assign the mask parameter.
  static constexpr void set(snd_pcm_hw_params& hw_params, value_type value) noexcept
  {
    static_assert(!out_of_range(), "Not a mask parameter.");

    auto& mask = hw_params.masks[param - SNDRV_PCM_HW_PARAM_FIRST_MASK];
    mask.bits[0] = 0;
    mask.bits[1] = 0;
    mask.bits[value >> 5] |= (1 << (value & 31));
  }
};

/// Used for initializing all the masks in a hardware parameter structure.
///
/// @tparam param The caller should leave this to its default
/// value. This parameter is used to control the recursive loop.
template <parameter_name param = SNDRV_PCM_HW_PARAM_FIRST_MASK>
struct masks_initializer final
{
  /// Initializes all the masks in a hardware parameter structure.
  static constexpr void init(snd_pcm_hw_params& hw_params) noexcept
  {
    mask_ref<param>::init(hw_params);

    masks_initializer<param + 1>::init(hw_params);
  }
};

/// Terminates a mask initialization loop.
template <>
struct masks_initializer<SNDRV_PCM_HW_PARAM_LAST_MASK + 1> final
{
  static constexpr void init(snd_pcm_hw_params&) noexcept { }
};

} // namespace

//====================//
// Section: Intervals //
//====================//

namespace {

/// Used for accessing and modifying
/// interval hardware parameters.
///
/// @tparam name The name of the interval being regarded.
template <parameter_name name>
struct interval_ref final
{
  /// A type definition for an interval value.
  using value_type = decltype(snd_interval::min);
  /// Indicates if this is a valid interval parameter.
  ///
  /// @return True if it's a valid interval parameter,
  /// false if it is not.
  static constexpr bool out_of_range() noexcept
  {
    return (name < SNDRV_PCM_HW_PARAM_FIRST_INTERVAL)
        || (name > SNDRV_PCM_HW_PARAM_LAST_INTERVAL);
  }
  /// Sets the value of the interval.
  ///
  /// @param hw_params The hardware parameters that the interval resides in.
  ///
  /// @param value_type The value to assign the interval parameter.
  static constexpr void set(snd_pcm_hw_params& hw_params, value_type value) noexcept
  {
    static_assert(!out_of_range(), "Not an interval parameter.");

    auto& ref = hw_params.intervals[name - SNDRV_PCM_HW_PARAM_FIRST_INTERVAL];
    ref.min = value;
    ref.max = value;
    ref.integer = 1;
  }
  /// Initializes the interval.
  ///
  /// @param hw_params The hardware parameters containing the interval to initialize.
  static constexpr void init(snd_pcm_hw_params& hw_params) noexcept
  {
    static_assert(!out_of_range(), "Not an interval parameter.");

    auto& ref = hw_params.intervals[name - SNDRV_PCM_HW_PARAM_FIRST_INTERVAL];

    ref.max = std::numeric_limits<value_type>::max();
  }
};

/// This is used to initialize all the interval
/// structures in a hardware parameters instance.
///
/// @tparam name Callers should keep this at its
/// default value. This parameter is used to iterate
/// the recusive template loop.
template <parameter_name name = SNDRV_PCM_HW_PARAM_FIRST_INTERVAL>
struct intervals_initializer final
{
  /// Initializes the intervals in a hardware parameters instance.
  ///
  /// @param hw_params The hardware parameters instance containing
  /// the intervals to be initialized.
  inline static constexpr void init(snd_pcm_hw_params& hw_params) noexcept
  {
    interval_ref<name>::init(hw_params);
    intervals_initializer<name + 1>::init(hw_params);
  }
};

/// Terminates an interval initialization loop.
template <>
struct intervals_initializer<SNDRV_PCM_HW_PARAM_LAST_INTERVAL + 1> final
{
  /// Does nothing.
  inline static constexpr void init(snd_pcm_hw_params&) noexcept {}
};

} // namespace

//==============================//
// Section: Hardware Parameters //
//==============================//

/// Initializes a new instance of hardware parameters.
///
/// @return A new instance of hardware parameters.
inline constexpr snd_pcm_hw_params init_hw_parameters() noexcept
{
  snd_pcm_hw_params params {};

  masks_initializer<>::init(params);

  intervals_initializer<>::init(params);

#if 0
  for (int n = SNDRV_PCM_HW_PARAM_FIRST_INTERVAL; n <= SNDRV_PCM_HW_PARAM_LAST_INTERVAL; n++) {
    struct snd_interval *i = param_to_interval(p, n);
    i->max = ~0;
  }
#endif

  params.rmask = ~0U;
  params.info = ~0U;

  return params;
}

//==================================//
// Section: Native ALSA to TinyALSA //
//==================================//

namespace {

auto to_tinyalsa_class(int native_class_) noexcept
{
  switch (native_class_) {
    case SNDRV_PCM_CLASS_GENERIC:
      return pcm_class::generic;
    case SNDRV_PCM_CLASS_MULTI:
      return pcm_class::multi_channel;
    case SNDRV_PCM_CLASS_MODEM:
      return pcm_class::modem;
    case SNDRV_PCM_CLASS_DIGITIZER:
      return pcm_class::digitizer;
    default:
      break;
  }

  return pcm_class::unknown;
}

auto to_tinyalsa_subclass(int native_subclass) noexcept
{
  switch (native_subclass) {
    case SNDRV_PCM_SUBCLASS_GENERIC_MIX:
      return pcm_subclass::generic_mix;
    case SNDRV_PCM_SUBCLASS_MULTI_MIX:
      return pcm_subclass::multi_channel_mix;
    default:
      break;
  }

  return pcm_subclass::unknown;
}

tinyalsa::pcm_info to_tinyalsa_info(const snd_pcm_info& native_info) noexcept
{
  tinyalsa::pcm_info out;

  out.device               = native_info.device;
  out.subdevice            = native_info.subdevice;
  out.card                 = native_info.card;
  out.subdevices_count     = native_info.subdevices_count;
  out.subdevices_available = native_info.subdevices_avail;

  memcpy(out.id,      native_info.id,      std::min(sizeof(out.id),      sizeof(native_info.id)));
  memcpy(out.name,    native_info.name,    std::min(sizeof(out.name),    sizeof(native_info.name)));
  memcpy(out.subname, native_info.subname, std::min(sizeof(out.subname), sizeof(native_info.subname)));

  out.class_   = to_tinyalsa_class(native_info.dev_class);
  out.subclass = to_tinyalsa_subclass(native_info.dev_subclass);

  return out;
}

} // namespace

//==================================//
// Section: TinyALSA to Native ASLA //
//==================================//

namespace {

/// Converts a sample format to
/// one that's recognized by the ALSA drivers.
constexpr int to_alsa_format(sample_format sf) noexcept
{
  switch (sf) {
    case sample_format::u8:
      return SNDRV_PCM_FORMAT_U8;

    case sample_format::u16_le:
      return SNDRV_PCM_FORMAT_U16_LE;

    case sample_format::u16_be:
      return SNDRV_PCM_FORMAT_U16_BE;

    case sample_format::u18_3le:
      return SNDRV_PCM_FORMAT_U18_3LE;

    case sample_format::u18_3be:
      return SNDRV_PCM_FORMAT_U18_3BE;

    case sample_format::u20_3le:
      return SNDRV_PCM_FORMAT_U20_3LE;

    case sample_format::u20_3be:
      return SNDRV_PCM_FORMAT_U20_3BE;

    case sample_format::u24_3le:
      return SNDRV_PCM_FORMAT_U24_3LE;

    case sample_format::u24_3be:
      return SNDRV_PCM_FORMAT_U24_3BE;

    case sample_format::u24_le:
      return SNDRV_PCM_FORMAT_U24_LE;

    case sample_format::u24_be:
      return SNDRV_PCM_FORMAT_U24_BE;

    case sample_format::u32_le:
      return SNDRV_PCM_FORMAT_U32_LE;

    case sample_format::u32_be:
      return SNDRV_PCM_FORMAT_U32_BE;

    case sample_format::s8:
      return SNDRV_PCM_FORMAT_S8;

    case sample_format::s16_le:
      return SNDRV_PCM_FORMAT_S16_LE;

    case sample_format::s16_be:
      return SNDRV_PCM_FORMAT_S16_BE;

    case sample_format::s18_3le:
      return SNDRV_PCM_FORMAT_S18_3LE;

    case sample_format::s18_3be:
      return SNDRV_PCM_FORMAT_S18_3BE;

    case sample_format::s20_3le:
      return SNDRV_PCM_FORMAT_S20_3LE;

    case sample_format::s20_3be:
      return SNDRV_PCM_FORMAT_S20_3BE;

    case sample_format::s24_3le:
      return SNDRV_PCM_FORMAT_S24_3LE;

    case sample_format::s24_3be:
      return SNDRV_PCM_FORMAT_S24_3BE;

    case sample_format::s24_le:
      return SNDRV_PCM_FORMAT_S24_LE;

    case sample_format::s24_be:
      return SNDRV_PCM_FORMAT_S24_BE;

    case sample_format::s32_le:
      return SNDRV_PCM_FORMAT_S32_LE;

    case sample_format::s32_be:
      return SNDRV_PCM_FORMAT_S32_BE;
  }

  /* unreachable */

  return 0;
}

/// Converts a sample access pattern
/// to one that's recognized by the ALSA drivers.
constexpr int to_alsa_access(sample_access access) noexcept
{
  switch (access) {
    case sample_access::interleaved:
      return SNDRV_PCM_ACCESS_RW_INTERLEAVED;
    case sample_access::non_interleaved:
      return SNDRV_PCM_ACCESS_RW_NONINTERLEAVED;
    case sample_access::mmap_interleaved:
      return SNDRV_PCM_ACCESS_MMAP_INTERLEAVED;
    case sample_access::mmap_non_interleaved:
      return SNDRV_PCM_ACCESS_MMAP_NONINTERLEAVED;
  }

  /* unreachable */

  return 0;
}

/// Configures a PCM configuration structure
/// to a hardware parameters instance.
constexpr snd_pcm_hw_params to_alsa_hw_params(const pcm_config& config, sample_access access) noexcept
{
  auto params = init_hw_parameters();

  interval_ref<SNDRV_PCM_HW_PARAM_CHANNELS>::set(params, config.channels);

  interval_ref<SNDRV_PCM_HW_PARAM_PERIOD_SIZE>::set(params, config.period_size);

  interval_ref<SNDRV_PCM_HW_PARAM_PERIODS>::set(params, config.period_count);

  interval_ref<SNDRV_PCM_HW_PARAM_RATE>::set(params, config.rate);

  mask_ref<SNDRV_PCM_HW_PARAM_FORMAT>::set(params, to_alsa_format(config.format));

  mask_ref<SNDRV_PCM_HW_PARAM_ACCESS>::set(params, to_alsa_access(access));

  return params;
}

/// Converts a PCM configuration into the relevant software parameters.
///
/// @param config The PCM configuration to be converted.
/// @param is_capture Whether or not this is a capture device.
constexpr snd_pcm_sw_params to_alsa_sw_params(const pcm_config& config, bool is_capture)
{
  snd_pcm_sw_params params {};

  params.period_step = 1;
  params.avail_min = config.period_size;

  if (config.start_threshold) {
    params.start_threshold = config.start_threshold;
  } else if (is_capture) {
    params.start_threshold = 1;
  } else {
    params.start_threshold = config.period_count * config.period_size / 2;
  }

  if (config.stop_threshold) {
    params.stop_threshold = config.stop_threshold;
  } else if (is_capture) {
    params.stop_threshold = config.period_count * config.period_size * 10;
  } else {
    params.stop_threshold = config.period_count * config.period_size;
  }

  params.boundary = config.period_count * config.period_size;
  params.xfer_align = config.period_size / 2;
  params.silence_size = 0;
  params.silence_threshold = config.silence_threshold;

  return params;
}

} // namespace

const char* get_error_description(int error) noexcept
{
  if (error == 0) {
    return "Success";
  } else {
    return ::strerror(error);
  }
}

//=====================//
// Section: POD Buffer //
//=====================//

/// A simple buffer for POD type data.
///
/// TODO : Check the impact of multiple instantiations
/// on overall binary size.
///
/// @tparam element_type The type of the element
/// stored in the buffer.
template <typename element_type>
struct pod_buffer final
{
  /// The array of elements.
  element_type* data = nullptr;
  /// The number of elements in the buffer.
  size_type size = 0;
  /// Constructs an empty buffer.
  constexpr pod_buffer() noexcept : data(nullptr), size(0) {}
  /// Moves a buffer from one variable to another.
  ///
  /// @param other The variable to be moved.
  inline constexpr pod_buffer(pod_buffer&& other) noexcept
    : data(other.data),
      size(other.size)
  {
    other.data = nullptr;
    other.size = 0;
  }
  /// Releases memory allocated by the buffer.
  ~pod_buffer()
  {
    std::free(data);
    data = nullptr;
    size = 0;
  }
  /// Adds an element to the end of the buffer.
  ///
  /// @param e The element to add to the end of the buffer.
  ///
  /// @return True on success, false on failure.
  bool emplace_back(element_type&& e) noexcept
  {
    auto* tmp = (element_type*) std::realloc(data, (size + 1) * sizeof(element_type));
    if (!tmp) {
      return false;
    }
    data = tmp;
    size++;
    data[size - 1] = std::move(e);
    return true;
  }
};

//=============================//
// Section: Interleaved Reader //
//=============================//

result interleaved_pcm_reader::open(size_type card, size_type device, bool non_blocking) noexcept
{
  return pcm::open_capture_device(card, device, non_blocking);
}

generic_result<size_type> interleaved_pcm_reader::read_unformatted(void* frames, size_type frame_count) noexcept
{
  snd_xferi transfer {
    0 /* result */,
    frames,
    snd_pcm_uframes_t(frame_count),
  };

  auto err = ioctl(get_file_descriptor(), SNDRV_PCM_IOCTL_READI_FRAMES, &transfer);
  if (err < 0) {
    return { errno, 0 };
  }

  return { 0, size_type(transfer.result) };
}

//==============//
// Section: PCM //
//==============//

/// Contains all the implementation data for a PCM.
class pcm_impl final
{
  friend pcm;
  /// The file descriptor for the opened PCM.
  int fd = invalid_fd();
  /// Opens a PCM by a specified path.
  ///
  /// @param path The path of the PCM to open.
  ///
  /// @param non_blocking Whether or not the call to ::open
  /// should block if the device is not available.
  ///
  /// @return On success, zero is returned.
  /// On failure, a copy of errno is returned.
  result open_by_path(const char* path, bool non_blocking) noexcept;
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

int pcm::get_file_descriptor() const noexcept
{
  return self ? self->fd : invalid_fd();
}

bool pcm::is_open() const noexcept
{
  if (!self) {
    return false;
  }

  return self->fd != invalid_fd();
}

result pcm::prepare() noexcept
{
  if (!self) {
    return ENOENT;
  }

  auto err = ::ioctl(self->fd, SNDRV_PCM_IOCTL_PREPARE);
  if (err < 0) {
    return errno;
  }

  return result();
}

result pcm::setup(const pcm_config& config, sample_access access, bool is_capture) noexcept
{
  auto hw_params = to_alsa_hw_params(config, access);

  auto err = ioctl(get_file_descriptor(), SNDRV_PCM_IOCTL_HW_PARAMS, &hw_params);
  if (err < 0) {
    return errno;
  }

  auto sw_params = to_alsa_sw_params(config, is_capture);

  err = ioctl(get_file_descriptor(), SNDRV_PCM_IOCTL_SW_PARAMS, &sw_params);
  if (err < 0) {
    return errno;
  }

  return 0;
}

result pcm::start() noexcept
{
  if (!self) {
    return ENOENT;
  }

  auto err = ::ioctl(self->fd, SNDRV_PCM_IOCTL_START);
  if (err < 0) {
    return errno;
  }

  return result();
}

result pcm::drop() noexcept
{
  if (!self) {
    return ENOENT;
  }

  auto err = ::ioctl(self->fd, SNDRV_PCM_IOCTL_DROP);
  if (err < 0) {
    return errno;
  }

  return result();
}

generic_result<pcm_info> pcm::get_info() const noexcept
{
  using result_type = generic_result<pcm_info>;

  if (!self) {
    return result_type { ENOENT };
  }

  snd_pcm_info native_info;

  int err = ioctl(self->fd, SNDRV_PCM_IOCTL_INFO, &native_info);
  if (err != 0) {
    return result_type { errno };
  }

  return result_type { 0, to_tinyalsa_info(native_info) };
}

result pcm::open_capture_device(size_type card, size_type device, bool non_blocking) noexcept
{
  self = lazy_init(self);
  if (!self) {
    return result { ENOMEM };
  }

  char path[256];

  snprintf(path, sizeof(path), "/dev/snd/pcmC%luD%luc",
           (unsigned long) card,
           (unsigned long) device);

  return self->open_by_path(path, non_blocking);
}

result pcm::open_playback_device(size_type card, size_type device, bool non_blocking) noexcept
{
  self = lazy_init(self);
  if (!self) {
    return result { ENOMEM };
  }

  char path[256];

  snprintf(path, sizeof(path), "/dev/snd/pcmC%luD%lup",
           (unsigned long) card,
           (unsigned long) device);

  return self->open_by_path(path, non_blocking);
}

result pcm_impl::open_by_path(const char* path, bool non_blocking) noexcept
{
  if (fd != invalid_fd()) {
    ::close(fd);
  }

  fd = ::open(path, non_blocking ? (O_RDWR | O_NONBLOCK) : O_RDWR);
  if (fd < 0) {
    fd = invalid_fd();
    return result { errno };
  } else {
    return result { 0 };
  }
}

//===================//
// Section: PCM list //
//===================//

namespace {

/// A wrapper around a directory pointer
/// that closes the directory when it goes out of scope.
struct dir_wrapper final
{
  /// A pointer to the opened directory.
  DIR* ptr = nullptr;
  /// Casts the wrapper to a directory pointer.
  ///
  /// @return The directory pointer.
  inline operator DIR* () noexcept
  {
    return ptr;
  }
  /// Closes the directory if it was opened.
  ~dir_wrapper()
  {
    if (ptr) {
      closedir(ptr);
    }
  }
  /// Opens a directory.
  ///
  /// @param path The path of the directory to open.
  ///
  /// @return True on success, false on failure.
  bool open(const char* path) noexcept
  {
    ptr = opendir(path);
    return !!ptr;
  }
};

/// Represents a PCM name that was parsed
/// from a directory entry.
struct parsed_name final
{
  /// Whether or not the name is valid.
  bool valid = false;
  /// The index of the card.
  size_type card = 0;
  /// The index of the device.
  size_type device = 0;
  /// Whether or not it's a capture PCM.
  bool is_capture = false;
  /// Constructs a new parsed name instance.
  ///
  /// @param name A pointer to the name to parse.
  constexpr parsed_name(const char* name) noexcept
  {
    valid = parse(name);
  }
private:
  /// Parses a name given to the constructor.
  ///
  /// @param name The filename to be parsed.
  ///
  /// @return True on a match, false on failure.
  bool parse(const char* name) noexcept;
  /// Indicates if a character is a decimal number or not.
  ///
  /// @return True if it is a decimal character, false otherwise.
  static constexpr bool is_dec(char c) noexcept
  {
    return (c >= '0') && (c <= '9');
  }
  /// Parses a decimal number.
  ///
  /// @param c The character to convert into a decimal number.
  ///
  /// @return The resultant decimal number.
  constexpr size_type to_dec(char c) noexcept
  {
    return size_type(c - '0');
  }
};

bool parsed_name::parse(const char* name) noexcept
{
  auto name_length = strlen(name);
  if (!name_length) {
    return false;
  }

  if ((name[0] != 'p')
   || (name[1] != 'c')
   || (name[2] != 'm')
   || (name[3] != 'C')) {
    return false;
  }

  size_type d_pos = name_length;

  for (size_type i = 4; i < name_length; i++) {
    if (name[i] == 'D') {
      d_pos = i;
      break;
    }
  }

  if (d_pos >= name_length) {
    return false;
  }

  if (name[name_length - 1] == 'c') {
    is_capture = true;
  } else if (name[name_length - 1] == 'p') {
    is_capture = false;
  } else {
    return false;
  }

  device = 0;
  card = 0;

  for (size_type i = 4; i < d_pos; i++) {
    if (!is_dec(name[i])) {
      return false;
    }
    card *= 10;
    card += to_dec(name[i]);
  }

  for (size_type i = d_pos + 1; i < (name_length - 1); i++) {
    if (!is_dec(name[i])) {
      return false;
    }
    device *= 10;
    device += to_dec(name[i]);
  }

  return true;
}

} // namespace

class pcm_list_impl final
{
  friend pcm_list;
  /// The array of information instances.
  pod_buffer<pcm_info> info_buffer;
};

pcm_list::pcm_list() noexcept : self(nullptr)
{
  self = new (std::nothrow) pcm_list_impl();
  if (!self) {
    return;
  }

  dir_wrapper snd_dir;

  if (!snd_dir.open("/dev/snd")) {
    return;
  }

  dirent* entry = nullptr;

  for (;;) {

    entry = readdir(snd_dir);
    if (!entry) {
      break;
    }

    parsed_name name(entry->d_name);
    if (!name.valid) {
      continue;
    }

    result open_result;

    pcm p;

    if (name.is_capture) {
      open_result = p.open_capture_device(name.card, name.device);
    } else {
      open_result = p.open_playback_device(name.card, name.device);
    }

    if (open_result.failed()) {
      continue;
    }

    auto info_result = p.get_info();
    if (info_result.failed()) {
      continue;
    }

    if (!self->info_buffer.emplace_back(info_result.unwrap())) {
      break;
    }
  }
}

pcm_list::pcm_list(pcm_list&& other) noexcept : self(other.self)
{
  other.self = nullptr;
}

pcm_list::~pcm_list()
{
  delete self;
}

const pcm_info* pcm_list::data() const noexcept
{
  return self ? self->info_buffer.data : nullptr;
}

size_type pcm_list::size() const noexcept
{
  return self ? self->info_buffer.size : 0;
}

} // namespace tinyalsa
