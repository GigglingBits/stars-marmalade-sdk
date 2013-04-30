#ifndef _JPEGFILE_H
#define _JPEGFILE_H

#include <string>
#include "JpegDecompressor.h"

class JpegFile {
private:
	char* m_pData;
	int32 m_iSize;

	std::string m_sFileName;

public:
	JpegFile();
	~JpegFile();

	bool LoadFile(std::string filename);
	std::string GetFileName();

	JpegDecompressor* CreateDecompressor();
private:
	bool IsLoaded();
	void Reset();
};

#endif

