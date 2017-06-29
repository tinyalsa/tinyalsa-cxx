#include <tinyalsa/config.hpp>

namespace {

class Config final : public tinyalsa::Config {
	unsigned int channels;
	unsigned int rate;
public:
	Config(void);
	~Config(void);
	unsigned int GetChannels(void) const;
	unsigned int GetRate(void) const;
	void SetChannels(unsigned int channels);
	void SetRate(unsigned int rate);
};

} /* namespace */

namespace tinyalsa {

std::shared_ptr<Config> Config::Create(void) {
	std::shared_ptr<Config> config(new ::Config);
	return config;
}

Config::~Config(void) {

}

} /* namespace tinyalsa */

namespace {

Config::Config(void) : channels(2), rate(48000) {

}

Config::~Config(void) {

}

unsigned int Config::GetChannels(void) const {
	return channels;
}

unsigned int Config::GetRate(void) const {
	return rate;
}

void Config::SetChannels(unsigned int channels_) {
	channels = channels_;
}

void Config::SetRate(unsigned int rate_) {
	rate = rate_;
}

} /* namespace */

