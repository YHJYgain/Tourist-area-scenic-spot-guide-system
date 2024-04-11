#include <stdio.h>
#include <stdlib.h>  // INT_MAX
#include <string.h>  // strcpy()、strlen()、strcmp() 

#define MAX_NAME 36        // 景点名字符串的最大长度
#define MAX_VERTEX_NUM 61  // 最大景点数 
#define MAX_INFO 36        // 景点信息、路信息字符串的最大长度 
#define INFINITY INT_MAX   // 在内存上以整型最大值代表两景点之间无路，但显示为 0 

typedef char InfoType;     // 信息类型 

/* 采用邻接矩阵作为数据结构，存储信息 */
typedef struct Attractions {
	int id;                    // 景点编号
	char name[MAX_NAME];       // 景点名
	InfoType vinfo[MAX_INFO];  // 景点信息
} Attractions,VexArray[MAX_VERTEX_NUM]; // 顶点（景点）

typedef struct ArcCell {
	double distance;           // 两景点间的路程
	InfoType einfo[MAX_INFO];  // 边（路）信息
} AdjMatrix[MAX_VERTEX_NUM][MAX_VERTEX_NUM]; // 邻接矩阵（边）

typedef struct Map {
	VexArray vexs;      // 顶点（景点）向量
	AdjMatrix arcs;     // 邻接矩阵
	int vexnum,arcnum;  // 图的当前顶点（景点）数和边（路）数
} Map; // 旅游区地图（无向网）

typedef struct {
	Attractions adjvex;
	double lowcost;
} AuxiliaryArrays;
