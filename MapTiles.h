#pragma once

#define TILE_WIDTH 48					//타일 가로 사이즈
#define TILE_HEIGHT 48					//타일 세로 사이즈

#define TILE_X 20						//맵의 타일 갯수 X
#define TILE_Y 20						//맵의 타일 갯수 Y

#define TILESIZEX TILE_X * TILE_WIDTH	//Width
#define TILESIZEY TILE_Y * TILE_HEIGHT	//Height

#define SAMPLE_TILE_X 3					//오른쪽 화면의 타일 갯수 X
#define SAMPLE_TILE_Y 10				//오른쪽 화면의 타일 갯수 Y

//비트 연산을 하기 위한 매크로
#define ATTR_UNMOVABLE	0x00000001

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
	OBJ_NONE
};

//타일 구조체
struct tagTile
{
	TERRAIN terrain;	//지형 구조체
	OBJECT obj;			//오브젝트 구조체
	RECT rc;			//타일 렉트

	int terrainFrameX;	//지형 프레임 X
	int terrainFrameY;	//지형 프레임 Y
	int objFrameX;		//오브젝트 프레임 X
	int objFrameY;		//오브젝트 프레임 Y
	int F, G, H;		//Astar에서 사용되는 변수
	int node;			//Astar에서 사용되는 노드

};

//이미지 타일 구조체
struct tagSampleTile
{
	RECT rcTile;

	int sTileFrameX;	//샘플 타일 프레임 X
	int sTileFrameY;	//샘플 타일 프레임 Y
};

//현재 타일 구조체
struct tagCurrentTile
{
	int x;				//현재 타일의 x축
	int y;				//현재 타일의 y축
};
