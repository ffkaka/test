#include <iostream>
#include <fstream>
#include <omp.h>
#include <stdio.h>
#include <chrono>
#include <vector>

using namespace std;

int main(void) {

	ifstream ifs;
	ifs.open("48k_s32le_6ch.pcm", ifstream::in);
	ofstream ofs;
	ofs.open("48k_s16le_6ch.pcm", ofstream::out);

	ifs.seekg(0, ifs.end);
	auto len = ifs.tellg();
	ifs.seekg(0, ifs.beg);
	printf("FileSize(%ldB)\n", len);

	vector<int32_t> inPcm;
	vector<int16_t> outPcm;
	inPcm.resize(640);
	outPcm.resize(640);

	for(int i= 0; ifs.tellg() < len; ++i) {
		char* in = (char*)inPcm.data();
		ifs.read(in, 640 * sizeof(int32_t));
		for(int j=0;j<640;j++) {
			outPcm[j] = static_cast<int16_t>(inPcm[j]/0x7FFF);
		}
		ofs.write((char*)outPcm.data(), 640*sizeof(int16_t));
	}

	ifs.close();
	ofs.close();
	return 0;
}
