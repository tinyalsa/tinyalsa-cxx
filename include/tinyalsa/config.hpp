#ifndef TINYALSA_CONFIG_HPP
#define TINYALSA_CONFIG_HPP

namespace tinyalsa {

class Config {
public:
	static Config* Create(void);
	virtual ~Config(void);
	virtual unsigned int GetChannels(void) const = 0;
	virtual unsigned int GetRate(void) const = 0;
	virtual void SetChannels(unsigned int channels) = 0;
	virtual void SetRate(unsigned int rate) = 0;
};

} /* namespace tinyalsa */

#endif /* TINYALSA_CONFIG_HPP */

