#include <speex/speex_resampler.h>
#include <vector>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include "SpeexSrc.h"

#define CH (4)
#define BUFSIZE (1024)
SpeexResamplerState *pSrc = nullptr;
int fd_in = -1;
int fd_out = -1;

int main()
{
	fd_in = open("./rec.pcm", O_RDONLY);
	if (fd_in < 0) {
		printf("Fail to open in files\n");
		return -1;
	}

	fd_out = open("./16k.pcm", O_CREAT|O_TRUNC|O_WRONLY, S_IRWXU|S_IRWXG|S_IROTH);
	if (fd_out < 0) {
		printf("Fail to open out files\n");
		return -1;
	}

	int err;
	pSrc = speex_resampler_init(4, 48000, 16000, SPEEX_RESAMPLER_QUALITY_DEFAULT, &err);
	if (nullptr == pSrc) {
		printf("Fail to create speex\n");
		close(fd_in); return -1;
	}

	int nread = 0;
	int nwrite = 0;
	std::vector<int8_t> in(BUFSIZE, 0);
	auto buf = in.data();
	uint32_t inSamples = BUFSIZE / sizeof(int16_t) / CH;
	uint32_t outSamples = inSamples / 3;
	std::vector<int16_t> out;
	uint32_t total = 0;
	do {
		out.resize(outSamples * CH, 0);
		auto outb = out.data();
		nread = read(fd_in, buf, BUFSIZE);
		speex_resampler_process_interleaved_int(pSrc,
				(int16_t*)buf,
				&inSamples,
				outb,
				&outSamples);
		//printf("in=(%d), out=(%d)\n", inSamples, outSamples);
		if (fd_out > 0) {
			nwrite = write(fd_out, outb, outSamples * sizeof(int16_t) * CH);
		}
		total += nread;
	} while(nread > 0);
	printf("total read(%u)\n", total);

	speex_resampler_destroy(pSrc);
	pSrc = nullptr;
	close(fd_out);
	close(fd_in);

}
