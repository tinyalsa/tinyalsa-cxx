#ifndef TINYALSA_CXX_TINYALSA_HPP
#define TINYALSA_CXX_TINYALSA_HPP

#include <utility>

#include <cstddef>

namespace tinyalsa {

/// A type used to indicate array sizes
/// and indices.
using size_type = std::size_t;

/// Gets the description of an error.
///
/// @param error The error to get the description of.
///
/// @return A description of the error.
/// If there is no error (error == 0), then "Success" is returned.
const char* get_error_description(int error) noexcept;

/// This structure is used to show the
/// result of an operation that may fail.
///
/// @tparam value_type If the operation returns
/// information other than an error code, then
/// it is of this type.
template <typename value_type>
struct generic_result final {
  /// An errno-type value to indicate success or failure.
  int error = 0;
  /// The result of the operation.
  value_type value = value_type();
  /// Indicates whether the result is
  /// a failure or not.
  constexpr bool failed() const noexcept
  {
    return error != 0;
  }
  /// Gets a description of the error.
  ///
  /// @return A description of the error.
  /// If there was no error, then "Success" is returned.
  inline const char* error_description() const noexcept
  {
    return get_error_description(error);
  }
  /// Gets the resultant value of
  /// the operation.
  ///
  /// @return The result of the operation.
  value_type unwrap() noexcept
  {
    return std::move(value);
  }
};

/// This structure is used to show
/// the result of an operation
/// that has no additional information
/// other than the error code.
template<>
struct generic_result<void> final
{
  /// An errno-type value to indicate the result of the operation.
  int error = 0;
  /// Constructs a simple result instance.
  ///
  /// @param errno_copy An errno value to indicate whether
  /// or not the result was successful.
  constexpr generic_result(int errno_copy = 0) noexcept
    : error(errno_copy) { }
  /// Indicates whether the result is
  /// a failure or not.
  constexpr bool failed() const noexcept
  {
    return error != 0;
  }
  /// Gets a description of the error.
  ///
  /// @return A description of the error.
  /// If there was no error, then "Success" is returned.
  inline const char* error_description() const noexcept
  {
    return get_error_description(error);
  }
};

/// A type definition for a result that
/// has nothing except an errno value.
using result = generic_result<void>;

/// Indicates the number assigned to
/// invalid file descriptors. These can
/// appear when either the PCM hasn't been
/// opened yet or it failed to open.
static constexpr int invalid_fd() noexcept
{
  return -1;
}

/// A magic value used to indicate an invalid card number.
static constexpr size_type invalid_card() noexcept
{
  return 0xffff;
}

/// A magic value used to indicate an invalid device number.
static constexpr size_type invalid_device() noexcept
{
  return 0xffff;
}

/// A magic value used to indicate an invalid subdevice number.
static constexpr size_type invalid_subdevice() noexcept
{
  return 0xffff;
}

/// Enumerates the supported sample formats.
enum class sample_format
{
  s8,
  s16_le,
  s16_be,
  s18_3le,
  s18_3be,
  s20_3le,
  s20_3be,
  s24_3le,
  s24_3be,
  s24_le,
  s24_be,
  s32_le,
  s32_be,
  u8,
  u16_le,
  u16_be,
  u18_3le,
  u18_3be,
  u20_3le,
  u20_3be,
  u24_3le,
  u24_3be,
  u24_le,
  u24_be,
  u32_le,
  u32_be
};

/// Used to query parameters about a certain
/// sample format.
///
/// @tparam sf The sample format to get the parameters for.
template <sample_format sf>
struct sample_traits final { };

template <>
struct sample_traits<sample_format::s8> final
{
  bool is_signed() noexcept { return true; }
};

/// Enumerates the several possible
/// modes of accessing sample data from a PCM.
enum class sample_access
{
  /// Interleaved sample buffers.
  /// Samples of a frame appear next
  /// to each other in a single frame.
  /// Only one audio buffer is required.
  interleaved,
  /// Non interleaved sample buffers.
  /// Samples for a channel are separated
  /// into distinct buffers. Usually at least
  /// one or two audio buffers are required.
  non_interleaved,
  /// Memory mapped interleaved buffers.
  mmap_interleaved,
  /// Memory mapped non-interleaved buffers.
  mmap_non_interleaved,
};

/// Enumerates the known PCM classes.
enum class pcm_class
{
  /// A placeholder for uninitialized values or errors.
  unknown,
  /// A generic mono or stereo device.
  generic,
  /// A multi channel device.
  multi_channel,
  /// A software modem calss.
  modem,
  /// A digitizer class.
  digitizer
};

/// Converts a PCM class to a human-readable string.
///
/// @param c The PCM class to convert.
///
/// @return A human-readable string describing the PCM class.
inline constexpr const char* to_string(pcm_class c) noexcept;

/// Enumerates the known PCM sub-classes.
enum class pcm_subclass
{
  /// A placeholder for uninitialized values or errors.
  unknown,
  /// Mono or stereo sub-devices are mixed together.
  generic_mix,
  /// Multi-channel subdevices are mixed together.
  multi_channel_mix
};

/// Converts a PCM subclass to a human-readable string.
///
/// @param subclass The PCM subclass to be converted.
///
/// @return A human-readable form of the subclass.
inline constexpr const char* to_string(pcm_subclass subclass) noexcept;

/// Used to describe the configuration
/// of a PCM device.
struct pcm_config final
{
  /// The number of samples per frame.
  size_type channels = 2;
  /// The number of frames per second.
  size_type rate = 48000;
  /// The number of frames in one period.
  size_type period_size = 1024;
  /// The total number of periods.
  size_type period_count = 2;
  /// The format for one sample.
  sample_format format = sample_format::s16_le;
  /// The number of frames to buffer
  /// before starting playback or capture.
  size_type start_threshold = 0;
  /// The number of frames to buffer
  /// before stopping the playback or capture.
  size_type stop_threshold = 0;
  /// The number of frames to buffer
  /// before silencing the audio.
  size_type silence_threshold = 0;
};

/// Contains information on a PCM device.
struct pcm_info final
{
  /// The card number of the PCM.
  size_type card = invalid_card();
  /// The device number of the PCM.
  size_type device = invalid_device();
  /// The subdevice number of the PCM.
  size_type subdevice = invalid_subdevice();
  /// The PCM class identifier.
  pcm_class class_;
  /// The PCM subclass identifier.
  pcm_subclass subclass;
  /// The human readable device name.
  char id[64];
  /// The name of the device.
  char name[80];
  /// The name of the subdevice.
  char subname[32];
  /// The number of of subdevices.
  size_type subdevices_count = 0;
  /// The number of available subdevices.
  size_type subdevices_available = 0;
};

class pcm_impl;

/// This is the base of any kind of PCM.
/// The base class is responsible for interfacing
/// with the file descriptor of the PCM device.
class pcm
{
  /// A pointer to the implementation data.
  pcm_impl* self = nullptr;
public:
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
  /// Gets information on the PCM.
  ///
  /// @return A structure containing information on the PCM.
  generic_result<pcm_info> get_info() const noexcept;
  /// Indicates whether or not the PCM is opened.
  ///
  /// @return True if the PCM is opened,
  /// false if it is not.
  bool is_open() const noexcept;
  /// Prepares the PCM to be started.
  ///
  /// @return On success, zero is returned.
  /// On failure, a copy of errno is returned.
  result prepare() noexcept;
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
  result start() noexcept;
  /// Stops either the playback or capture loop
  /// of the audio device. Any buffered audio that
  /// exist at the point of calling this function
  /// is lost.
  ///
  /// @return On success, zero is returned.
  /// On failure, a copy of errno is returned instead.
  result drop() noexcept;
  /// Opens a capture PCM.
  ///
  /// @param card The index of the card to open the PCM from.
  /// @param device The index of the device to open the PCM from.
  /// @param non_blocking Whether or not the PCM should be opened in non-blocking mode.
  result open_capture_device(size_type card = 0, size_type device = 0, bool non_blocking = true) noexcept;
  /// Opens a playback PCM.
  ///
  /// @param card The index of the card to open the PCM from.
  /// @param device The index of the device to open the PCM from.
  /// @param non_blocking Whether or not the PCM should be opened in non-blocking mode.
  result open_playback_device(size_type card = 0, size_type device = 0, bool non_blocking = true) noexcept;
protected:
  /// Applys a configuration to a PCM.
  ///
  /// @param access The access mode to assign the PCM.
  /// This affects what read and write operations are available.
  ///
  /// @param is_capture Whether or not the PCM is a capture device.
  result setup(const pcm_config& config, sample_access access, bool is_capture) noexcept;
};

class interleaved_reader
{
public:
  /// Reads unformatted data directly from the device.
  ///
  /// @param frames A pointer to the frame buffer to fill.
  /// @param frame_count The number of audio frames to be read.
  ///
  /// @return Both an error code and the number of read frames are returned.
  /// On success, the error code has a value of zero.
  /// On failure, the error code has an errno value.
  /// On failure, the number of read frames is zero.
  virtual generic_result<size_type> read_unformatted(void* frames, size_type frame_count) noexcept = 0;
};

class interleaved_pcm_reader final : public pcm, public interleaved_reader
{
public:
  /// Opens a new PCM reader.
  ///
  /// @param card The index of the card to open.
  /// @param device The index of the device to open.
  /// @param non_blocking Whether or not the call
  /// should block if the device is not available.
  result open(size_type card = 0, size_type device = 0, bool non_blocking = false) noexcept;
  /// Sets up the PCM with a given config.
  ///
  /// @param config The config to setup the PCM with.
  /// It's perfectly valid to leave out this parameter
  /// and use the default configuration.
  inline result setup(const pcm_config& config = pcm_config()) noexcept
  {
    return pcm::setup(config, sample_access::interleaved, true /* is capture */);
  }
  generic_result<size_type> read_unformatted(void* frames, size_type frame_count) noexcept override;
};

class pcm_list_impl;

/// This class is used for enumerating
/// the PCMs available on the system.
///
/// The best way to use this class is to
/// declare it in a function scope with a
/// short life time. That way, the list is
/// always up to date.
class pcm_list final
{
  /// A pointer to the implementation data.
  pcm_list_impl* self = nullptr;
public:
  /// Constructs the PCM list.
  /// Internally, this constructor will discover the PCMs on the system.
  pcm_list() noexcept;
  /// Moves the PCM list from one variable to another.
  ///
  /// @param other The PCM list to move.
  pcm_list(pcm_list&& other) noexcept;
  /// Releases the memory allocated by the list.
  ~pcm_list();
  /// Indicates the number of PCMs in the list.
  size_type size() const noexcept;
  /// Accesses the array of PCM info instances.
  ///
  /// @return A pointer to the beginning of the PCM info array.
  const pcm_info* data() const noexcept;
  /// Accesses an entry from the list.
  ///
  /// @note This function does not perform boundary checking.
  ///
  /// @param index The index of the entry to access.
  ///
  /// @return A const-reference to the specified entry.
  inline const pcm_info& operator [] (size_type index) const noexcept
  {
    return data()[index];
  }
  /// Accesses the beginning iterator of the list.
  /// Used in range-based for loops.
  ///
  /// @return A pointer to the beginning of the list.
  inline const pcm_info* begin() const noexcept
  {
    return data();
  }
  /// Accesses the ending iterator of the list.
  /// Used in range-based for loops.
  ///
  /// @return A pointer to the end of the list.
  /// This value should not be dereferenced.
  inline const pcm_info* end() const noexcept
  {
    return data() + size();
  }
};

/// Prints the result of an operation.
/// If the result failed, then the error description
/// is printed. If the result did not fail, then the value
/// of the result is printed.
///
/// @tparam stream_type The type of the stream being printed to.
/// @tparam value_type The type of the value contained in the result structure.
///
/// @param output The stream to print the result to.
/// @param result The result to be printed.
///
/// @return A reference to @p output.
template <typename stream_type, typename value_type>
stream_type& operator << (stream_type& output, const generic_result<value_type>& res);

/// Prints the error of a result.
///
/// @param output The stream to print to.
/// @param res The result to print the error of.
///
/// @return A reference to @p output.
template <typename stream_type>
stream_type& operator << (stream_type& output, const result& res);

template <typename stream_type>
stream_type& operator << (stream_type& output, pcm_class class_);

template <typename stream_type>
stream_type& operator << (stream_type& output, pcm_subclass subclass);

/// Prints a PCM information structure to an output stream.
///
/// @param output The stream to print to.
/// @param info The PCM information to be printed.
///
/// @return A reference to @p output.
template <typename stream_type>
stream_type& operator << (stream_type& output, const pcm_info& info);

// Template and inlined implementation below this point.

template <typename stream_type, typename value_type>
stream_type& operator << (stream_type& output, const generic_result<value_type>& res)
{
  if (res.failed()) {
    return output << get_error_description(res.error);
  } else {
    return output << res.value;
  }
}

template <typename stream_type>
stream_type& operator << (stream_type& output, const result& res)
{
  return output << get_error_description(res.error);
}

template <typename stream_type>
stream_type& operator << (stream_type& output, pcm_class class_)
{
  return output << to_string(class_);
}

template <typename stream_type>
stream_type& operator << (stream_type& output, pcm_subclass subclass)
{
  return output << to_string(subclass);
}

template <typename stream_type>
stream_type& operator << (stream_type& output, const pcm_info& info)
{
  output << "card      : " << info.card << '\n';
  output << "device    : " << info.device << '\n';
  output << "subdevice : " << info.subdevice << '\n';
  output << "class     : " << info.class_ << '\n';
  output << "subclass  : " << info.subclass << '\n';
  output << "id        : " << info.id << '\n';
  output << "name:     : " << info.name << '\n';
  output << "subname   : " << info.subname << '\n';
  output << "subdevices count     : " << info.subdevices_count << '\n';
  output << "subdevices available : " << info.subdevices_available << '\n';
  return output;
}

inline constexpr const char* to_string(pcm_class c) noexcept
{
  switch (c) {
    case pcm_class::unknown:
      break;
    case pcm_class::generic:
      return "Generic";
    case pcm_class::multi_channel:
      return "Multi-channel";
    case pcm_class::modem:
      return "Modem";
    case pcm_class::digitizer:
      return "Digitizer";
  }

  return "Unknown";
}

inline constexpr const char* to_string(pcm_subclass subclass) noexcept
{
  switch (subclass) {
    case pcm_subclass::unknown:
      break;
    case pcm_subclass::generic_mix:
      return "Generic Mix";
    case pcm_subclass::multi_channel_mix:
      return "Multi-channel Mix";
  }

  return "Unknown";
}

} // namespace tinyalsa

#endif // TINYALSA_CXX_TINYALSA_HPP
