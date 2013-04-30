#include "JpegDecompressor.h"
#include "IwJPEG.h"

#define TEXTURE_FORMAT CIwImage::RGB_565

JpegDecompressor::JpegDecompressor(const char* data, int length) : ThreadPoolTask() {
	// caller owned input data
	m_pxInputData = data;
	m_iInputSize = length;

	// private, locally owned output data
	m_pxOutputData = NULL;
	m_iOutputSize = 0;
	m_pxTexture = NULL;

	// internal state
	m_eNextStep = ePrepare;

	// set up decompression state
	jpeg_create_decompress(&m_xCompressionInfo);
}

JpegDecompressor::~JpegDecompressor() {
	jpeg_destroy_decompress(&m_xCompressionInfo);

	if (m_pxOutputData) {
		s3eFree(m_pxOutputData);
	}
	if (m_pxTexture) {
		delete m_pxTexture;
	}
}

void JpegDecompressor::OnExecute() {
	while (Step());
}

bool JpegDecompressor::Step() {
	if (m_eNextStep == eDone) {
		return false;
	}

	// make sure that we can capture errors
	m_xCompressionInfo.err = jpeg_std_error(&m_xError);

	switch (m_eNextStep) {
		case ePrepare:
			StepDecompressHeader();
			StepAllocateOutputData();
			m_eNextStep = eDecompress;
			break;
		case eDecompress:
			if (!StepDecompressBody()) {
				m_eNextStep = eMakeTexture;
			}
			break;
		case eMakeTexture:
			StepReduceColours();
			StepCreateTexture();
			m_eNextStep = eDone;
			break;
	}

	return true;
}

CIwTexture* JpegDecompressor::GetTexture() {
	CIwTexture* tex = m_pxTexture;
	m_pxTexture = NULL;
	return tex;
}

void JpegDecompressor::StepDecompressHeader() {
	jpeg_buf_src(&m_xCompressionInfo,(char*)m_pxInputData, m_iInputSize);
	//Realloc crash?
	jpeg_read_header(&m_xCompressionInfo, TRUE);
	m_xCompressionInfo.out_color_space = JCS_RGB;
	jpeg_start_decompress(&m_xCompressionInfo);
}

void JpegDecompressor::StepAllocateOutputData() {
	m_iOutputSize = m_xCompressionInfo.image_width * m_xCompressionInfo.image_height * 3 /* 24bit */;
	m_pxOutputData = (char*)s3eMalloc(m_iOutputSize);
}

bool JpegDecompressor::StepDecompressBody() {
	const int numlines = 150;

	// are we done?
	if (m_xCompressionInfo.output_scanline >= m_xCompressionInfo.output_height) {
		return false;
	}

	// decompress some lines
	for (int i = 0; i < numlines; i++) {
		if (m_xCompressionInfo.output_scanline < m_xCompressionInfo.output_height) {
			char* linha = m_pxOutputData + 3 /* 24bit*/ * m_xCompressionInfo.image_width * m_xCompressionInfo.output_scanline;
			jpeg_read_scanlines(&m_xCompressionInfo, (unsigned char**)&linha, 1);
		} else {
			jpeg_finish_decompress(&m_xCompressionInfo);
			break;
		}
	}
	return true;
}

#define RGB16(red, green, blue) ( ((red >> 3) << 11) | ((green >> 2) << 5) | (blue >> 3))

void JpegDecompressor::StepReduceColours() {
	// initial colour splace is 24 bit; reduce to 16bit
	int len24bit = m_iOutputSize;
	char* data24bit = m_pxOutputData;
	
	int len16bit = m_xCompressionInfo.image_width * m_xCompressionInfo.image_height * sizeof(uint16);
	uint16* data16bit = (uint16*)s3eMalloc(len16bit);

	int pixelcount = m_xCompressionInfo.image_width * m_xCompressionInfo.image_height;
	for (int i = 0; i < pixelcount; i++) {
		data16bit[i] = RGB16(
			(uint16)data24bit[i * 3], 
			(uint16)data24bit[i * 3 + 1], 
			(uint16)data24bit[i * 3 + 2]);
	}

	// replace the 24bit version with 16 bit version
	m_pxOutputData = (char *)data16bit;
	m_iOutputSize = len16bit;
	s3eFree(data24bit);
}

void JpegDecompressor::StepCreateTexture() {
	m_pxTexture = new CIwTexture();
	m_pxTexture->CopyFromBuffer(
		m_xCompressionInfo.output_width, m_xCompressionInfo.output_height, CIwImage::RGB_565, 
		sizeof(uint16)*m_xCompressionInfo.output_width, (unsigned char*)m_pxOutputData, 0);
	
	s3eFree(m_pxOutputData);
	m_pxOutputData = NULL;
	m_iOutputSize = 0;
}

