#ifndef _DIRECTIONS_H_ 
#define _DIRECTIONS_H_ 

#define DIR_CENTER		0							// 0000 0000

#define DIR_UP			1							// 0000 0001
#define DIR_DOWN		2							// 0000 0010
#define DIR_LEFT		4							// 0000 0100
#define DIR_RIGHT		8							// 0000 1000

#define DIR_UPLEFT		(DIR_UP | DIR_LEFT)			// 0000 0101
#define DIR_DOWNLEFT	(DIR_DOWN | DIR_LEFT)		// 0000 0110
#define DIR_UPRIGHT		(DIR_UP | DIR_RIGHT)		// 0000 1001
#define DIR_DOWNRIGHT	(DIR_DOWN | DIR_RIGHT)		// 0000 1010 

#define DIR_TYPE		unsigned char 
#define DIR_TYPE_MAXVAL DIR_DOWNRIGHT

#define DIR_IS_UP(x)	(((DIR_TYPE)x) & DIR_UP) 
#define DIR_IS_DOWN(x)	(((DIR_TYPE)x) & DIR_DOWN) 
#define DIR_IS_LEFT(x)	(((DIR_TYPE)x) & DIR_LEFT) 
#define DIR_IS_RIGHT(x)	(((DIR_TYPE)x) & DIR_RIGHT) 

enum Direction {
	eCenter = DIR_CENTER, 
	eUp = DIR_UP,
	eUpRight = DIR_UPRIGHT,
	eRight = DIR_RIGHT,
	eDownRight = DIR_DOWNRIGHT,
	eDown = DIR_DOWN,
	eDownLeft = DIR_DOWNLEFT,
	eLeft = DIR_LEFT, 
	eUpLeft = DIR_UPLEFT
};

#endif
