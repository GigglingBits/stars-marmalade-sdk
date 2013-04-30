#include "JpegFile.h"

//#include "MemBuckets.h"
#include "IwJPEG.h"
#include "IwDebug.h"
#include "s3eFile.h"
#include "IwTexture.h"

#define TEXTURE_FORMAT CIwImage::RGB_565

JpegFile::JpegFile() {
	m_pData = NULL;
	m_iSize = 0;
}

JpegFile::~JpegFile() {
	Reset();
}

std::string JpegFile::GetFileName() {
	return m_sFileName;
}

bool JpegFile::LoadFile(std::string filename) {
	bool success = false;
	Reset();

	//IwMemBucketPush(MEM_BUCKET_IMAGECACHE);
	if (!filename.empty()) {
		s3eFile* pFile;
		if ((pFile = s3eFileOpen(filename.c_str(), "rb"))) {
			int32 size = s3eFileGetSize(pFile);
			char* pBuf = (char*)s3eMalloc(size);
			if (1 >= s3eFileRead(pBuf, size, 1, pFile)) {
				if (IsJPEG(pBuf, size)) {
					m_pData = pBuf;
					m_iSize = size;
					pBuf = NULL;
					size = 0;

					m_sFileName = filename;
					success = true;
				} else {
					IwAssertMsg(MYAPP, false, ("%s is not a JPEG!", filename.c_str()));
				}
			} else {
				IwAssertMsg(MYAPP, false, ("%s could not be read: %s", filename.c_str(), s3eFileGetErrorString()));
			}
			s3eFileClose(pFile);
			if (pBuf) {
				s3eFree(pBuf);
			}
		} else {
			IwAssertMsg(MYAPP, false, ("%s could not be opened: %s", filename.c_str(), s3eFileGetErrorString()));
		}
	}
	//IwMemBucketPop();

	return success;
}

bool JpegFile::IsLoaded() {
	return m_pData != NULL;
}

void JpegFile::Reset() {
	if (m_pData) {
		delete[] m_pData;
		m_pData = NULL;
	}
	m_iSize = 0;
}

JpegDecompressor* JpegFile::CreateDecompressor() {
	if (!IsLoaded()) return NULL;
	return new JpegDecompressor(m_pData, m_iSize);
}
