#include <vector>
#include <string>
#include <speex/speex.h>
#include <speex/speex_resampler.h>
#include "SpeexSrc.h"

SpeexSrc::SpeexSrc()
{
}

SpeexSrc::~SpeexSrc()
{
}

int SpeexSrc::initSrc(uint32_t inRate, uint32_t outRate, uint16_t chs)
{
	return init(inRate, outRate, chs);
}

void SpeexSrc::removeSrc()
{
	remove();
}

int SpeexSrc::processSrc(int16_t* in, uint32_t inSamples) 
{
	return process(in, inSamples);
}


int SpeexSrc::init(uint32_t inRate, uint32_t outRate, uint16_t chs)
{
	int err;
	pSrc = speex_resampler_init(chs,
			inRate,
			outRate,
			SPEEX_RESAMPLER_QUALITY_DEFAULT,
			&err);
	if (nullptr == pSrc) {
		return -1;
		printf("Fail to create speex(%d)\n", err);
	}
	return 0;
}

void SpeexSrc::remove()
{
	if (pSrc) {
		speex_resampler_destroy(pSrc);
		pSrc = nullptr;
	}
}


int SpeexSrc::process(int16_t* in, uint32_t inSamples)
{
	uint32_t outSamples = inSamples * outRate / inRate;
	out.resize(outSamples * channels);
	auto outb = out.data();
	speex_resampler_process_interleaved_int(pSrc,
			(int16_t*)in,
			&inSamples,
			outb,
			&outSamples);
}

