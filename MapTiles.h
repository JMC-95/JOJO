#pragma once

#define TILESIZE 48					//타일 사이즈

#define TILEX 20					//맵의 타일 갯수 X
#define TILEY 20					//맵의 타일 갯수 Y

#define TILESIZEX TILEX * TILESIZE	//Width
#define TILESIZEY TILEY * TILESIZE	//Height

#define SAMPLETILEX 3				//오른쪽 화면의 타일 갯수 X
#define SAMPLETILEY 10				//오른쪽 화면의 타일 갯수 Y

//비트 연산을 하기 위한 매크로
#define ATTR_UNMOVABLE	0x00000001
#define ATTR_POSITION	0x00000002

//지형 (EX. 밟을 때 사운드를 나게 하거나 이동할 수 없는 지역 등)
enum TERRAIN
{
	TR_BARRACKS,		//병영
	TR_PLAIN,			//평지
	TR_BADLANDS,		//황무지
	TR_CASTLERAND,		//성내
	TR_CASTLEGATE,		//성문
	TR_END
};

//오브젝트 (EX. 지형 위쪽에 배치. 움직이거나 부서지거나 하는 녀석 등 변화를 줄수 있는 물체)
enum OBJECT
{
	OBJ_CASTLEWALLS,	//성벽
	OBJ_ROCKMOUNTAIN,	//바위산
	OBJ_MOUNTAIN,		//산
	OBJ_CHARACTER,		//캐릭터
	OBJ_NONE
};

//타일 구조체
struct tagTile
{
	TERRAIN terrain;
	OBJECT obj;
	RECT rc;

	int terrainFrameX;
	int terrainFrameY;
	int objFrameX;
	int objFrameY;
};

//이미지 타일 구조체
struct tagSampleTile
{
	RECT rcTile;

	int terrainFrameX;
	int terrainFrameY;
};

//현재 타일 구조체
struct tagCurrentTile
{
	int x;
	int y;
};
