#include <stdio.h>
#include <stdlib.h>  // INT_MAX
#include <string.h>  // strcpy()��strlen()��strcmp() 

#define MAX_NAME 36        // �������ַ�������󳤶�
#define MAX_VERTEX_NUM 61  // ��󾰵��� 
#define MAX_INFO 36        // ������Ϣ��·��Ϣ�ַ�������󳤶� 
#define INFINITY INT_MAX   // ���ڴ������������ֵ����������֮����·������ʾΪ 0 

typedef char InfoType;     // ��Ϣ���� 

/* �����ڽӾ�����Ϊ���ݽṹ���洢��Ϣ */
typedef struct Attractions {
	int id;                    // ������
	char name[MAX_NAME];       // ������
	InfoType vinfo[MAX_INFO];  // ������Ϣ
} Attractions,VexArray[MAX_VERTEX_NUM]; // ���㣨���㣩

typedef struct ArcCell {
	double distance;           // ��������·��
	InfoType einfo[MAX_INFO];  // �ߣ�·����Ϣ
} AdjMatrix[MAX_VERTEX_NUM][MAX_VERTEX_NUM]; // �ڽӾ��󣨱ߣ�

typedef struct Map {
	VexArray vexs;      // ���㣨���㣩����
	AdjMatrix arcs;     // �ڽӾ���
	int vexnum,arcnum;  // ͼ�ĵ�ǰ���㣨���㣩���ͱߣ�·����
} Map; // ��������ͼ����������

typedef struct {
	Attractions adjvex;
	double lowcost;
} AuxiliaryArrays;
