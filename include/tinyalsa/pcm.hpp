#ifndef TINYALSA_PCM_HPP
#define TINYALSA_PCM_HPP

namespace tinyalsa {

class Config;

class Pcm {
public:
	virtual ~Pcm(void);
	virtual void Close(void) = 0;
	virtual Config* GetConfig(void) const = 0;
	virtual void Open(unsigned int card, unsigned int device) = 0;
	virtual void SetConfig(const Config* config) = 0;
};

} /* namespace tinyalsa */

#endif /* TINYALSA_PCM_HPP */

