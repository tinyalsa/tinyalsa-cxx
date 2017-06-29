#ifndef TINYALSA_PCM_HPP
#define TINYALSA_PCM_HPP

namespace tinyalsa {

class Config;

class Pcm {
public:
	virtual ~Pcm(void);
	virtual void Close(void) = 0;
	virtual const Config& GetConfig(void) const = 0;
	virtual void Open(unsigned int card, unsigned int device) = 0;
	virtual void SetConfig(const Config& config) = 0;
	virtual void Start(void) = 0;
	virtual void Reset(void) = 0;
	virtual void Prepare(void) = 0;
};

} /* namespace tinyalsa */

#endif /* TINYALSA_PCM_HPP */

