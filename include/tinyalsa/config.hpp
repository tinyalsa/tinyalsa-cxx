#ifndef TINYALSA_CONFIG_HPP
#define TINYALSA_CONFIG_HPP

#include <memory>

namespace tinyalsa {

/// Describes the way a single
/// sample is encoded
enum class SampleType {
	/// Signed, 32-bit, little-endian
	S32_LE,
	/// Signed, 32-bit, big-endian
	S32_BE,
	/// Signed, 24-bit (32-bit, with padding), little-endian
	S24_LE,
	/// Signed, 24-bit (32-bit, with padding), big-endian
	S24_BE,
	/// Signed, 24-bit, little-endian
	S24_3LE,
	/// Signed, 24-bit, big-endian
	S24_3BE,
	/// Signed, 16-bit, little-endian
	S16_LE,
	/// Signed, 16-bit, big-endian
	S16_BE,
	/// Signed, 8-bit
	S8
};

/// Describes the way a PCM is configured.
class Config {
public:
	/// Creates a PCM configuration
	static std::shared_ptr<Config> Create(void);
	virtual ~Config(void);
	/// Returns the number of channels in the configuration
	virtual unsigned int GetChannels(void) const = 0;
	/// Returns the number of frames per second in the configuration
	virtual unsigned int GetRate(void) const = 0;
	/// Sets the number of channels in the configuration
	virtual void SetChannels(unsigned int channels) = 0;
	/// Sets the frames per second in the configuration
	virtual void SetRate(unsigned int rate) = 0;
};

} /* namespace tinyalsa */

#endif /* TINYALSA_CONFIG_HPP */

