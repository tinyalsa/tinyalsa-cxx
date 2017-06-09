#include <tinyalsa/input-pcm.hpp>

#include <tinyalsa/config.hpp>

#include <unistd.h>

namespace {

class InputPcm final : public tinyalsa::InputPcm {
	int fd;
public:
	InputPcm(void);
	~InputPcm(void);
	void Close(void);
	tinyalsa::Config* GetConfig(void) const;
	void Open(unsigned int card, unsigned int device);
	size_t Read(void* frame_array, size_t frame_count);
	void SetConfig(const tinyalsa::Config* config);
protected:
};

} /* namespace */

namespace tinyalsa {

InputPcm* InputPcm::Create(void) {
	return new ::InputPcm();
}

InputPcm::~InputPcm(void) {

}

} /* namespace tinyalsa */

namespace {

InputPcm::InputPcm(void) : fd(-1) {

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

tinyalsa::Config* InputPcm::GetConfig(void) const {
	return tinyalsa::Config::Create();
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

void InputPcm::SetConfig(const tinyalsa::Config* config) {
	(void) config;
}

} /* namespace */

