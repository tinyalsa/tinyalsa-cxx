#ifndef TINYALSA_SAMPLE_TYPE_HPP
#define TINYALSA_SAMPLE_TYPE_HPP

namespace tinyalsa {

enum class SampleType {
	S32_LE,
	S32_BE,
	S24_LE,
	S24_BE,
	S24_3LE,
	S24_3BE,
	S16_LE,
	S16_BE,
	S8,
	U32_LE,
	U32_BE,
	U24_LE,
	U24_BE,
	U24_3LE,
	U24_3BE,
	U16_LE,
	U16_BE,
	U8
};

} /* namespace tinyalsa */

#endif /* TINYALSA_SAMPLE_TYPE_HPP */

