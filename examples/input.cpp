#include <tinyalsa/input-pcm.hpp>

#include <cstdlib>

int main(void) {

	auto input_pcm = tinyalsa::InputPcm::Create();

	input_pcm->Open(0, 0);

	input_pcm->Close();

	return EXIT_SUCCESS;
}
