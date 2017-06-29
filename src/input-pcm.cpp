#include <tinyalsa/input-pcm.hpp>

#include <tinyalsa/config.hpp>

#include <unistd.h>

namespace {

class InputPcm final : public tinyalsa::InputPcm {
	int fd;
	std::shared_ptr<tinyalsa::Config> config;
public:
	InputPcm(void);
	~InputPcm(void);
	void Close(void);
	const tinyalsa::Config& GetConfig(void) const;
	void Open(unsigned int card, unsigned int device);
	size_t Read(void* frame_array, size_t frame_count);
	void SetConfig(const tinyalsa::Config& config);
	void Start(void);
	void Reset(void);
	void Prepare(void);
protected:
};

} /* namespace */

namespace tinyalsa {

std::shared_ptr<InputPcm> InputPcm::Create(void) {
	std::shared_ptr<InputPcm> input_pcm(new ::InputPcm);
	return input_pcm;
}

InputPcm::~InputPcm(void) {

}

} /* namespace tinyalsa */

namespace {

InputPcm::InputPcm(void) : fd(-1) {
	config = tinyalsa::Config::Create();
}

InputPcm::~InputPcm(void) {
	Close();
}

void InputPcm::Close(void) {
	if (fd >= 0) {
		close(fd);
		fd = -1;
	}
}

const tinyalsa::Config& InputPcm::GetConfig(void) const {
	return *config;
}

void InputPcm::Open(unsigned int card, unsigned int device) {
	(void) card;
	(void) device;
}

size_t InputPcm::Read(void* frame_array, size_t frame_count) {
	(void) frame_array;
	(void) frame_count;
	return 0;
}

void InputPcm::SetConfig(const tinyalsa::Config& config) {
	(void) config;
}

void InputPcm::Start(void) {

}

void InputPcm::Reset(void) {

}

void InputPcm::Prepare(void) {

}

} /* namespace */

