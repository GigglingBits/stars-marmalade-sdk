#ifndef __MEMORYBUCKETS_H__
#define __MEMORYBUCKETS_H__

#include "IwMemory.h"

enum MemoryBuckets {
	eMemoryBucketApp = IW_MEM_BUCKET_ID_SYSTEM,
	eMemoryBucketResources = IW_MEM_BUCKET_ID_USER,
	// eMemoryBucketLevels = IW_MEM_BUCKET_ID_USER + 1
};

#endif
