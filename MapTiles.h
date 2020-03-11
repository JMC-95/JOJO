#pragma once

#define TILE_WIDTH 48					//Ÿ�� ���� ������
#define TILE_HEIGHT 48					//Ÿ�� ���� ������

#define TILE_X 20						//���� Ÿ�� ���� X
#define TILE_Y 20						//���� Ÿ�� ���� Y

#define TILESIZEX TILE_X * TILE_WIDTH	//Width
#define TILESIZEY TILE_Y * TILE_HEIGHT	//Height

#define SAMPLE_TILE_X 3					//������ ȭ���� Ÿ�� ���� X
#define SAMPLE_TILE_Y 10				//������ ȭ���� Ÿ�� ���� Y

//��Ʈ ������ �ϱ� ���� ��ũ��
#define ATTR_UNMOVABLE	0x00000001

//���� (EX. ���� �� ���带 ���� �ϰų� �̵��� �� ���� ���� ��)
enum TERRAIN
{
	TR_BARRACKS,		//����
	TR_PLAIN,			//����
	TR_BADLANDS,		//Ȳ����
	TR_CASTLERAND,		//����
	TR_CASTLEGATE,		//����
	TR_END
};

//������Ʈ (EX. ���� ���ʿ� ��ġ. �����̰ų� �μ����ų� �ϴ� �༮ �� ��ȭ�� �ټ� �ִ� ��ü)
enum OBJECT
{
	OBJ_CASTLEWALLS,	//����
	OBJ_ROCKMOUNTAIN,	//������
	OBJ_MOUNTAIN,		//��
	OBJ_NONE
};

//Ÿ�� ����ü
struct tagTile
{
	TERRAIN terrain;	//���� ����ü
	OBJECT obj;			//������Ʈ ����ü
	RECT rc;			//Ÿ�� ��Ʈ

	int terrainFrameX;	//���� ������ X
	int terrainFrameY;	//���� ������ Y
	int objFrameX;		//������Ʈ ������ X
	int objFrameY;		//������Ʈ ������ Y
	int F, G, H;		//Astar���� ���Ǵ� ����
	int node;			//Astar���� ���Ǵ� ���

};

//�̹��� Ÿ�� ����ü
struct tagSampleTile
{
	RECT rcTile;

	int sTileFrameX;	//���� Ÿ�� ������ X
	int sTileFrameY;	//���� Ÿ�� ������ Y
};

//���� Ÿ�� ����ü
struct tagCurrentTile
{
	int x;				//���� Ÿ���� x��
	int y;				//���� Ÿ���� y��
};
