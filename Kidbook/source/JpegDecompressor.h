#ifndef _JPEGDECOMPRESSOR_H
#define _JPEGDECOMPRESSOR_H

#include "iw2d.h"
#include "IwTexture.h"
#include "ThreadPoolTask.h"

extern "C" {
	#include "jpeglib.h"
}

void init_source_from_buf(j_decompress_ptr cinfo);
void skip_input_data_from_buf(j_decompress_ptr cinfo, long nbytes);
boolean fill_input_buffer_from_buf(j_decompress_ptr cinfo);
void term_source_from_buf(j_decompress_ptr cinfo);
void jpeg_buf_src (j_decompress_ptr cinfo, char* buf,long size);

class JpegDecompressor : ThreadPoolTask {
private:
	enum DecompressionStep {
		ePrepare,
		eDecompress,
		eMakeTexture,
		eDone
	};

	typedef struct BUF_SOURCE_MGR {
		jpeg_source_mgr	pub;
		char*			buf;
		char			buf_term[2];
		long			buf_size;
		long			pos;
		bool			read_started;
	} _BUF_SOURCE_MGR;

private:
	DecompressionStep m_eNextStep;

	const char* m_pxInputData;
	int m_iInputSize;

	char* m_pxOutputData;
	int m_iOutputSize;

	//BUF_SOURCE_MGR m_xBufSourceMgr;
	jpeg_error_mgr m_xError;
	jpeg_decompress_struct m_xCompressionInfo;

	CIwTexture* m_pxTexture;

public:
	JpegDecompressor(const char* data, int length);
	~JpegDecompressor();

	// does a new compression step; returns true if there is more work
	bool Step();

	// returns thge texture once it is finished;
	// a texture can only be retrieved once, since retrieving it will clear the decompression state
	CIwTexture* GetTexture();

private:
	virtual void OnExecute();

	void StepDecompressHeader();
	void StepAllocateOutputData();
	bool StepDecompressBody();
	void StepReduceColours();
	void StepCreateTexture();
};

#endif

