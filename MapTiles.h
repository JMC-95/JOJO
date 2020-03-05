#pragma once

#define TILESIZE 48					//Ÿ�� ������

#define TILEX 20					//���� Ÿ�� ���� X
#define TILEY 20					//���� Ÿ�� ���� Y

#define TILESIZEX TILEX * TILESIZE	//Width
#define TILESIZEY TILEY * TILESIZE	//Height

#define SAMPLETILEX 3				//������ ȭ���� Ÿ�� ���� X
#define SAMPLETILEY 10				//������ ȭ���� Ÿ�� ���� Y

//��Ʈ ������ �ϱ� ���� ��ũ��
#define ATTR_UNMOVABLE	0x00000001
#define ATTR_POSITION	0x00000002

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
	OBJ_CHARACTER,		//ĳ����
	OBJ_NONE
};

//Ÿ�� ����ü
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

//�̹��� Ÿ�� ����ü
struct tagSampleTile
{
	RECT rcTile;

	int terrainFrameX;
	int terrainFrameY;
};

//���� Ÿ�� ����ü
struct tagCurrentTile
{
	int x;
	int y;
};
