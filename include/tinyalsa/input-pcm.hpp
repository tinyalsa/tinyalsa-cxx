#ifndef TINYALSA_INPUT_PCM_HPP
#define TINYALSA_INPUT_PCM_HPP

#include <tinyalsa/pcm.hpp>

#include <memory>
#include <cstdlib>

namespace tinyalsa {

class InputPcm : public Pcm {
public:
	static std::shared_ptr<InputPcm> Create(void);
	virtual ~InputPcm(void);
	virtual const Config& GetConfig(void) const = 0;
	virtual size_t Read(void* frame_array, size_t frame_count) = 0;
	virtual void SetConfig(const Config& config) = 0;
};

} /* namespace tinyalsa */

#endif /* TINYALSA_INPUTP_PCM_HPP */

