/*****************************************************
 * A Class which cna be used for Sample Rate converting 
 * with Speex Src - ffkaka
 * **************************************************/
#ifndef __SPEEX_SRC_FFKAKA__
#define __SPEEX_SRC_FFKAKA__
#include <stdint.h>
#include <vector>
#include <speex/speex_resampler.h>

class SpeexSrc {
	public:
		SpeexSrc();
		~SpeexSrc();

		int initSrc(uint32_t inRate, uint32_t outRate, uint16_t chs);
		void removeSrc();
		int processSrc(int16_t* in = nullptr, uint32_t inSamples = 0);

		std::vector<int16_t> out;

	private:
		int init(uint32_t inRate, uint32_t outRate, uint16_t chs);
		void remove();
		int process(int16_t* in, uint32_t inSamples);

		uint32_t inRate = 0;
		uint32_t outRate = 0;
		uint16_t format = 8;
		uint16_t channels = 0;

		SpeexResamplerState *pSrc = nullptr;

};

#endif //__SPEEX_SRC_FFKAKA__
