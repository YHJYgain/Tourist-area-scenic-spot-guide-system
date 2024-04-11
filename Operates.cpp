/* 全局常量和变量，重复利用内存空间 */
int flag;                                                      // 用于规范输入和输出
int z;                                                         // 功能选择的编号
int id,id1,id2;                                                // 读取的景点编号
int index,index1,index2,finalindex;                            // 景点下标
int pre,vex[MAX_VERTEX_NUM],P[MAX_VERTEX_NUM];                 // 前驱景点下标
int path[MAX_VERTEX_NUM];                                      // 路径上的景点下标
int shortestpaths[MAX_VERTEX_NUM][MAX_VERTEX_NUM];             // 多条路程相等的最短简单路径的各景点下标
int pathnum,numinpath[MAX_VERTEX_NUM];                         // 最短简单路径个数及各路径上景点个数
double D[MAX_VERTEX_NUM],totaldistance;                        // 最短简单路径路程
double adj;                                                    // 读取的路程
char vfilename[]="Vertexs.txt",efilename[]="Edges.txt";        // 分别存放景点、路的相关信息的 txt 文件名
char warnz[]="无效选择！";                                     // 功能选择编号输入出错时的警告信息
char input[5],case1[]="id",case2[]="name";                     // 区分用户根据 id 或 name 使用相关功能
char find[]="查询",edit[]="编辑",add[]="添加",erase[]="删除";  // 用户选择 id 或 name 使用相关功能时的提示信息
char name[MAX_NAME];                                           // 读取的景点名称
InfoType vinfo[MAX_INFO],einfo[MAX_INFO];                      // 读取的景点信息、路信息
bool visited[MAX_VERTEX_NUM],visited1[MAX_VERTEX_NUM],visited2[MAX_VERTEX_NUM]; // 判断景点是否被访问过

void MadeMenuCenter() { // 使整个主菜单大概居于窗口顶部中间
	int i;
	printf("\n");
	for(i=1; i<=9; ++i) printf("\t");
} // MadeMenuCenter

void DisplayStarLine(int len) { // 打印一整行星号
	int i;
	MadeMenuCenter();
	for(i=1; i<=len; ++i) printf("*");
} // DisplayStarLine

void DisplayHollowLine(int len) { // 打印两边带星号的空行
	int i;
	MadeMenuCenter();
	printf("*");
	for(i=1; i<=len-2; ++i) printf(" ");
	printf("*");
} // DisplayHollowLine

void RenderOneWords(int len,int len1,char *content,int len2) { // 输出带文字的一行
	int i;
	DisplayHollowLine(len);
	MadeMenuCenter();
	printf("*");
	for(i=1; i<=len1; ++i) printf(" ");
	printf("%s",content);
	for(i=1; i<=len2; ++i) printf(" ");
	printf("*");
} // RenderOneWords

void RenderMenu() { // 渲染页面主菜单
	int i,spacelen=22,starlen=18+2*spacelen;
	char systemname[]="旅游区景点导游系统";
	char content[10][33]= {"<1> 查看旅游区地图",
	                       "<2> 查询景点到其它景点的最短路径",
	                       "<3> 查询两景点间的简单路径",
	                       "<4> 查询两景点间的最短简单路径",
	                       "<5> 最佳游览路线推荐",
	                       "<6> 查询景点、路相关信息",
	                       "<7> 编辑景点、路相关信息",
	                       "<8> 添加景点、路相关信息",
	                       "<9> 删除景点、路相关信息",
	                       "<0> 退出旅游区景点导游系统"
	                      };
	DisplayStarLine(starlen);
	RenderOneWords(starlen,spacelen-1,systemname,spacelen-1);
	DisplayHollowLine(starlen);
	DisplayStarLine(starlen);
	for(i=0; i<10; ++i) RenderOneWords(starlen,14,content[i],starlen-strlen(content[i])-16);
	DisplayHollowLine(starlen);
	DisplayStarLine(starlen);
	printf("\n\n");
} // RenderMenu

void RenderFunctionTop(char *title) { // 渲染每个功能区的顶部
	int i,j=0,len=13*14,clen=strlen(title),llen=(len-clen)/2;
	for(i=1; i<=len; ++i) {
		if(i<=llen) printf("*");
		else if(i<=llen+clen) printf("%c",title[j++]);
		else printf("*");
	}
	printf("\n---");
} // RenderFunctionTop

void RenderFunctionBottom() { // 渲染每个功能区的底部
	int i;
	printf("---\n");
	for(i=1; i<=13*14; ++i) printf("*");
	printf("\n\n\n");
} // RenderFunctionBottom

bool isFullVisited(Map map,bool *visitedx) { // 判断景点是否全部访问到
	/*
	 * visitedx[i]：下标为 i 的景点是否访问过
	 */
	int i;
	for(i=1; i<=map.vexnum; ++i) {
		if(!visitedx[i]) return false;
	}
	return true;
} // isFullVisited

int GetIndexById(Map map,int number) { // 根据景点编号获得下标，若不存在则返回 -1
	/*
	 * number：景点编号
	 */
	int i;
	for(i=1; i<=map.vexnum; ++i) {
		if(map.vexs[i].id==number) return i;
	}
	return -1;
} // GetIndexById

int GetIndexByName(Map map,char *designation) { // 根据景点名称获得下标，若不存在则返回 -1
	/*
	 * designation[]：景点名称
	 */
	int i;
	for(i=1; i<=map.vexnum; ++i) {
		if(strcmp(map.vexs[i].name,designation)==0) return i;
	}
	return -1;
} // GetIndexByName

void CreatMap(Map &map) { // 创建一个旅游区地图
	int i,j;
	FILE *vertexsfile=fopen(vfilename,"r"),*edgesfile=fopen(efilename,"r"); // 以只读方式打开文件
	fscanf(vertexsfile,"%d",&map.vexnum);
	fscanf(edgesfile,"%d",&map.arcnum);
	for(i=1; i<=map.vexnum; ++i) fscanf(vertexsfile,"%s %d %s",map.vexs[i].name,&map.vexs[i].id,map.vexs[i].vinfo); // 读入景点相关信息
	/* 初始化邻接矩阵 */
	for(i=0; i<=map.vexnum; ++i) {
		for(j=0; j<=map.vexnum; ++j) {
			/* 将第 1 行、第 1 列初始化为景点编号 */
			if(i==0) map.arcs[i][j+1].distance=map.vexs[j+1].id;
			if(j==0) map.arcs[i+1][j].distance=map.vexs[i+1].id;
			if(i!=0&&j!=0) map.arcs[i][j].distance=INFINITY; // 网
		}
	}
	/* 读入路相关信息 */
	for(i=1; i<=map.arcnum; ++i) {
		fscanf(edgesfile,"%d %d %lf %s",&id1,&id2,&adj,einfo);
		index1=GetIndexById(map,id1);
		index2=GetIndexById(map,id2);
		/* 无向图 */
		map.arcs[index1][index2].distance=map.arcs[index2][index1].distance=adj;
		strcpy(map.arcs[index1][index2].einfo,einfo);
		strcpy(map.arcs[index2][index1].einfo,einfo);
	}
	fclose(vertexsfile);
	fclose(edgesfile);
} // CreatMap

void DisplayEditedAttraction(Map map,int subscript) { // 显示下标为 subscript 的景点相关信息
	/*
	 * subscript：景点下标
	 */
	printf("---\n");
	printf("该景点的相关信息变更为：\n");
	printf("·景点编号：%d\n·景点名称：%s\n·景点信息：%s\n",map.vexs[subscript].id,map.vexs[subscript].name,map.vexs[subscript].vinfo);
} // DisplayEditedAttraction

void DisplayAttraction(Map map,int subscript) { // 显示查找景点、其相邻景点和它们之间的路的相关信息
	/*
	 * subscript：景点下标
	 */
	int i;
	printf("---\n");
	printf("查找的景点的相关信息如下：\n");
	printf("·景点编号：%d\n·景点名称：%s\n·景点信息：%s\n",map.vexs[subscript].id,map.vexs[subscript].name,map.vexs[subscript].vinfo);
	printf("---\n");
	printf("与景点“%s”相邻的景点有 <格式： ·景点名称(id)，景点信息，交通工具(路程)>：\n",map.vexs[subscript].name);
	for(i=1; i<=map.vexnum; ++i) {
		if(map.arcs[subscript][i].distance!=INFINITY) {
			printf("·%s(%d)，%s，%s(%.1fm)\n",map.vexs[i].name,map.vexs[i].id,map.vexs[i].vinfo,map.arcs[subscript][i].einfo,map.arcs[subscript][i].distance);
		}
	}
} // DisplayAttraction

void DisplayAdjMatrix(Map map) { // 显示邻接矩阵
	int i,j;
	for(i=0; i<=map.vexnum; ++i) {
		for(j=0; j<=map.vexnum; ++j) {
			if(i==0||j==0) printf("%10d",(int)map.arcs[i][j].distance);    // 第 1 行、第 1 列显示景点编号
			if(i!=0&&j!=0) {
				if(map.arcs[i][j].distance==INFINITY) printf("%10.1f",0);  // 无路则显示 0
				else printf("%10.1f",map.arcs[i][j].distance);
			}
		}
		printf("\n");
	}
} // DisplayAdjMatrix

void DisplayMap(Map map) { // 功能 1：查看旅游区地图
	int i;
	printf("\n该旅游区总共有%d个景点，%d条路。",map.vexnum,map.arcnum);
	printf("\n%d个景点信息 <格式：·景点名称 景点编号> 分别为：\n",map.vexnum);
	for(i=1; i<=map.vexnum; ++i) printf("·%s  %d\n",map.vexs[i].name,map.vexs[i].id);
	printf("各景点之间的路程（m）矩阵（第1行和第1列表示景点编号，0表示无路）：\n");
	DisplayAdjMatrix(map);
} // DisplayMap

int StandardizedT(Map map,int t,int max,char *warning) { // 保证 t 输入正确，并返回 t
	/*
	 * t：用户输入的整数
	 * warning[]：警告信息
	 * max：k 的上限
	 */
	do {
		scanf("%d",&t);
		if(t<1||t>max) printf("%s请重新输入：",warning);
	} while(t<1||t>max);
	return t;
} // StandardizedT

int StandardizedId(Map map) { // 规范景点编号输入，并返回对应下标
	/*
	 * index：全局变量，景点下标
	 * id：全局变量，景点编号
	 */
	do {
		scanf("%d",&id);
		index=GetIndexById(map,id);
		if(index==-1) printf("景点编号%d不存在！请重新输入：",id);
	} while(index==-1);
	return index;
} // StandardizedId

int StandardizedName(Map map) { // 规范景点名称输入，并返回对应下标
	/*
	 * index：全局变量，景点下标
	 * name[]：全局变量，景点名称
	 */
	do {
		scanf("%s",name);
		index=GetIndexByName(map,name);
		if(index==-1) printf("景点“%s”不存在！请重新输入：",name);
	} while(index==-1);
	return index;
} // StandardizedName

double StandardizedAdj(Map map) { // 规范路程输入，并返回路程
	/*
	 * adj：全局变量，路程
	 */
	do {
		scanf("%lf",&adj);
		if(adj<=0) printf("路程必须为正数！请重新输入：");
	} while(adj<=0);
	return adj;
} // StandardizedAdj

int FindOneIndexByIdOrName(Map map,char *choose) { // 让用户自行选择跟据单个景点的 id 还是 name 查询景点下标
	/*
	 * case1[]：全局常量，用于判断用户输入是否为“id”
	 * case2[]：全局常量，用于判断用户输入是否为“name”
	 * flag：全局变量，在此函数中用于规范用户输入
	 * input[]：全局变量，接收用户输入的字符串
	 * index：全局变量，景点下标
	 *
	 * choose[]：使用功能的名称
	 */
	printf("请选择通过景点名称(id)或景点编号(name)进行%s（输入“id”或“name”）：",choose);
	do {
		flag=0;
		scanf("%s",input);
		if(strcmp(input,case1)==0) {
			printf("请输入景点编号：");
			index=StandardizedId(map);
		} else if(strcmp(input,case2)==0) {
			printf("请输入景点名称：");
			index=StandardizedName(map);
		} else {
			flag=1;
			printf("输入不满足要求！请重新输入：");
		}
	} while(flag==1);
	return index;
} // FindOneIndexByIdOrName

void FindTwoIndexByIdOrName(Map map,char *choose) { // 让用户自行选择跟据两个景点的 id 还是 name 查询景点下标
	/*
	 * case1[]：全局常量，用于判断用户输入是否为“id”
	 * case2[]：全局常量，用于判断用户输入是否为“name”
	 * flag：全局变量，在此函数中用于规范用户输入
	 * input[]：全局变量，接收用户输入的字符串
	 * index1、index2：全局变量，景点下标
	 *
	 * choose[]：使用功能的名称
	 */
	printf("请选择通过景点名称(id)或景点编号(name)进行%s（输入“id”或“name”）：",choose);
	do {
		flag=0;
		scanf("%s",input);
		if(strcmp(input,case1)==0) {
			printf("请单个输入需要查询的两景点编号：\n");
			printf("景点1：");
			index1=StandardizedId(map);
			printf("景点2：");
			index2=StandardizedId(map);
		} else if(strcmp(input,case2)==0) {
			printf("请单个输入需要查询的两景点名称：\n");
			printf("景点1：");
			index1=StandardizedName(map);
			printf("景点2：");
			index2=StandardizedName(map);
		} else {
			flag=1;
			printf("输入不满足要求！请重新输入：");
		}
	} while(flag==1);
} // FindTwoByIdOrName

void PrintfPath(Map map,int subscript) { // 规范路径输出
	/*
	 * subscript：景点下标
	 */
	if(flag==0) {
		printf("%s",map.vexs[subscript].name);
		flag=1;
	} else printf("-->%s",map.vexs[subscript].name);
} // PrintfPath

void ShortestPath_DIJ(Map map,int v0,int *P,double *D) { // Dijkstra 算法求最短路径
	/*
	 * v0：起点景点的下标
	 * visited[i]：全局变量，用于判断下标为 i 的景点是否被访问过
	 * P[i]：全局变量，在最短路径中下标为 i 的景点的前驱景点下标
	 * D[i]：全局变量，起点景点到下标为 i 的景点之间最短路径的路程
	 */
	int i,v,w;
	double min;
	for(v=1; v<=map.vexnum; ++v) {
		visited[v]=false;
		D[v]=map.arcs[v0][v].distance;                             // 初始化 D[i] 为各景点 i 到 v0 之间的路程
		if(D[v]!=INFINITY) P[v]=v0;                                // 若 v0 与 v 之间有路，则 v 的前驱为 v0
		else P[v]=0;                                               // 否则无前驱（0）
	}
	D[v0]=0;                                                       // v0 到自己的最短路程为 0
	visited[v0]=true;                                              // 最短路径从 v0 开始
	/* 开始主循环，每次求得 v0 到某个景点 v 的最短路程，将 v 设为访问过即加入最短路径中 */
	for(i=2; i<=map.vexnum; ++i) {                                 // 其余 map.vexnum-1 个景点
		min=INFINITY;                                              // 当前所知离 v0 最近的距离
		/* 求离 v0 最近的景点 v */
		for(w=1; w<=map.vexnum; ++w) {
			if(!visited[w]&&D[w]<min) {
				v=w;
				min=D[w];
			}
		}
		visited[v]=true;                                           // 离 v0 最近的景点 v 加入最短路径中
		/* 更新当前最短路径及路程 */
		for(w=1; w<=map.vexnum; ++w) {
			if(!visited[w]&&(min+map.arcs[v][w].distance<D[w])) {  // 如果 v0-->v-->w 的路程小于 v0-->w， 则更新 D 和 P
				D[w]=min+map.arcs[v][w].distance;
				P[w]=v;
			}
		}
	} // end for i
} // ShortestPath_DIJ

void FindAllShortestPath(Map map,int vi,int vj,int *path,int d) { // 利用 Dijkstra 算法求 vi 到 vj 之间的所有最短路径及路程
	/*
	 * visited[i]：全局变量，用于判断下标为 i 的景点是否被访问过
	 * path[i]：全局变量，顺序存放一整条简单路径的所有景点下标
	 * pathnum：全局变量，最短路径的条数
	 * numinpath[i]：全局变量，第 i 条最短路径中景点的个数
	 * shortestpaths[i][j]：全局变量，顺序存放第 i 条最短路径的所有景点下标
	 * totaldistance：全局变量，vi 到 vj 之间简单路径的路程

	 * d：简单路径中的景点个数
	 */
	int i,len;
	totaldistance=0;
	++d;
	path[d]=vi; // 从 vi 开始
	visited[vi]=true;
	/* 找到一条最短路径就存一条 */
	if(vi==vj) {
		len=d;
		for(i=1; i<len; ++i) totaldistance+=map.arcs[path[i]][path[i+1]].distance;
		for(i=1; i<=len; ++i) {
			if(totaldistance==D[vj]) shortestpaths[pathnum][i]=path[i]; // 把最短路径存入 shortestpaths 中
		}
		if(totaldistance==D[vj]) {
			numinpath[pathnum]=len;
			pathnum++;
		}
		visited[vi]=false; // 处理一条最短路径后将 vi 设置为未访问，并回退找另一条最短路径
		return;
	} // end if(vi==vj)
	for(i=1; i<=map.vexnum; ++i) {
		if(!visited[i]&&map.arcs[vi][i].distance!=INFINITY) FindAllShortestPath(map,i,vj,path,d); // 递归
	}
	visited[vi]=false; // for 循环结束意味着，vi 的邻景点要么被访问过，要么与 vi 无路，则同样置未访问后回退
} // FindAllShortestPath

void AllShortestPathByOne(Map map) { // 功能 2：查询景点到其它景点的最短路径
	/*
	 * find[]：全局常量，值为"查询"
	 * flag：全局变量，此函数中用于规范路径输出
	 */
	int v,i,j,d;
	printf("\n");
	index=FindOneIndexByIdOrName(map,find);                 // 获得用户输入景点的下标
	for(i=1; i<=map.vexnum; ++i) visited[i]=false;          // 初始化访问数组
	ShortestPath_DIJ(map,index,P,D);                        // 获得用户输入景点到其它任一景点之间的最短路程
	printf("---\n");
	printf("景点“%s”到其它任一景点的最短简单路径及路程：\n",map.vexs[index].name);
	printf("---\n");
	for(v=1; v<=map.vexnum; ++v) {
		if(D[v]!=0) {
			d=0;                                            // 重置 d
			pathnum=1;                                      // 每输出完 index 到 v 的所有最短路径后，重置 pathnum
			printf("“%s”：%5.1f(m)\n",map.vexs[v].name,D[v]);
			for(i=1; i<=map.vexnum; ++i) visited[i]=false;  // 调用 FindAllShortestPath 之前也要初始化访问数组
			FindAllShortestPath(map,index,v,path,d);        // 求下标为 index 和 v 景点之间的所有最短路径存入 shortestpaths 中
			for(i=1; i<pathnum; ++i) {
				flag=0;                                     // 每输出完一条最短路径后，重置 flag
				printf("·");
				for(j=1; j<=numinpath[i]; ++j) PrintfPath(map,shortestpaths[i][j]);
				printf("\n");
			}
			if(v<map.vexnum) printf("---\n");
		}
	}
} // AllShortestPathByOne

void FindAllSimplePath(Map map,int vi,int vj,int *path,int d) { // 求并输出 vi 到 vj 之间的所有简单路径及路程
	/*
	 * （修改自函数 FindAllShortestPath）
	 */
	int i,len;
	totaldistance=0;
	++d;
	path[d]=vi;
	visited[vi]=true;
	if(vi==vj) {
		len=d;
		flag=0;
		for(i=1; i<len; ++i) totaldistance+=map.arcs[path[i]][path[i+1]].distance;
		printf("%5.1f(m)，",totaldistance);
		for(i=1; i<=len; ++i) PrintfPath(map,path[i]);
		printf("\n");
		visited[vi]=false;
		return;
	} // end if(vi==vj)
	for(i=1; i<=map.vexnum; ++i) {
		if(!visited[i]&&map.arcs[vi][i].distance!=INFINITY) FindAllSimplePath(map,i,vj,path,d);
	}
	visited[vi]=false;
} // FindAllSimplePath

void AllSimplePathByTwo(Map map) { // 功能 3：查询两景点间的简单路径
	/*
	 * find[]：全局常量，值为"查询"
	 */
	int i,d=0;
	printf("\n");
	for(i=1; i<=map.vexnum; ++i) visited[i]=false;
	FindTwoIndexByIdOrName(map,find); // 获得用户输入两景点的下标
	printf("---\n");
	printf("景点“%s”到景点“%s”间所有简单路径及路程：\n",map.vexs[index1].name,map.vexs[index2].name);
	FindAllSimplePath(map,index1,index2,path,d);
} // AllSimplePathByTwo

void AllShortestPathByTwo(Map map) { // 功能 4：查询两景点间的最短简单路径
	/*
	 * find[]：全局常量，值为"查询"
	 */
	int i,j,d=0;
	pathnum=1;
	printf("\n");
	FindTwoIndexByIdOrName(map,find);
	for(i=1; i<=map.vexnum; ++i) visited[i]=false;
	ShortestPath_DIJ(map,index1,P,D);
	for(i=1; i<=map.vexnum; ++i) visited[i]=false;
	FindAllShortestPath(map,index1,index2,path,d);
	printf("---\n");
	printf("景点“%s”到景点“%s”间所有最短简单路径及路程：\n",map.vexs[index1].name,map.vexs[index2].name);
	for(i=1; i<pathnum; ++i) {
		flag=0;
		printf("·%5.1f(m)，",D[index2]);
		for(j=1; j<=numinpath[i]; ++j) PrintfPath(map,shortestpaths[i][j]);
		printf("\n");
	}
} // AllShortestPathByTwo

bool BestVisitPath_DIJ(Map map,int subscript) { // 利用 Dijsktra 算法的思路求最佳旅游路线及路程
	/*
	 * visited1[]：全局变量，每个景点作为终点的访问情况
	 * visited2[]：全局变量，路径中走过的每个景点的访问情况
	 * finalindex：全局变量，最终的终点下标
	 * pre：全局变量，前驱景点下标
	 * totaldistance：全局变量，路径路程
	 *
	 * subscript：景点下标
	 * minindex：在 visited1 未被访问过的所有景点中选取到 subscript 中路程最短的景点下标
	 * min：subscript 到 minindex 的路程
	 * vex[]：顺序存放最佳旅游路线上的所有景点下标
	 */
	int i,j=1,k,m=1,n,minindex;
	double min;
	visited1[subscript]=true;            // 每个起点设置为访问过
	if(isFullVisited(map,visited1)&&!isFullVisited(map,visited2)) return false; // 如果每个景点都作为终点过且在整个过程中没有经过所有景点
	ShortestPath_DIJ(map,subscript,P,D); // 获得下标为 subscript 的景点到其它景点的最短路径及路程
	min=INFINITY;
	/* 每次在 visited1 未被访问过的所有景点中选取到 subscript 中路程最短的景点 */
	for(i=1; i<=map.vexnum; ++i) {
		if(D[i]!=0&&D[i]<min&&!visited1[i]) {
			min=D[i];
			minindex=i;
		}
	}
	/* 将路程最短的这条路径上的所有景点下标存入 vex 中 */
	pre=P[minindex];
	while(pre!=0) {
		vex[m++]=pre;
		pre=P[pre];
	}
	/* 输出这条路径上除 mindex 的所有景点，并在 visited2 中置经过的景点为访问过 */
	for(n=m-1; n>=1; --n) {
		PrintfPath(map,vex[n]);
		visited2[vex[n]]=true;
	}
	visited2[minindex]=true;    // 将这条路径的终点也置为访问过
	totaldistance+=D[minindex]; // 统计总路程，直到 visited2 全为 true
	/* 如果 visited2 全为 true，则说明已经找到一整条最佳旅游路径，获得最终的终点下标后，返回 true */
	if(isFullVisited(map,visited2)) {
		finalindex=minindex;
		return true;
	}
	if(BestVisitPath_DIJ(map,minindex)) return true; // 将这次选出的路径终点作为起点继续递归直到 visited2 全为 true
	else return false;
} // BestVisitPath_DIJ

void BestVisitPath(Map map) { // 功能 5：最佳游览路线推荐
	int i;
	flag=0;
	for(i=1; i<=map.vexnum; ++i) {      // 初始化访问数组
		visited1[i]=false;
		visited2[i]=false;
	}
	printf("\n");
	index=FindOneIndexByIdOrName(map,find);
	printf("---\n");
	printf("从景点“%s”出发的最佳游览路线及路程：\n",map.vexs[index].name);
	if(BestVisitPath_DIJ(map,index)) {  // 如果该旅游区有最佳旅游路线，则输出路径和路程
		PrintfPath(map,finalindex);     // 输出最终的终点
		printf("，%5.1f(m)\n",totaldistance);
	} else printf("该旅游区无最佳旅游路线\n");
} // BestVisitPath

void AttractionInformation(Map map) { // 功能 6：查询景点、路相关信息
	/*
	 * find[]：全局常量，值为"查询"
	 */
	printf("n");
	index=FindOneIndexByIdOrName(map,find);
	DisplayAttraction(map,index);
} // AttractionInformation

void RewriteVFile(Map map) { // 重写景点文件
	/*
	 * vfilename：全局常量，景点文件名
	 */
	FILE *vertexsfile=fopen(vfilename,"w"); // 以只写的方式打开文件（若文件有内容，会自动清空）
	int i;
	fprintf(vertexsfile,"%d",map.vexnum);
	for(i=1; i<=map.vexnum; ++i) {
		fprintf(vertexsfile,"\n%s %d %s",map.vexs[i].name,map.vexs[i].id,map.vexs[i].vinfo);
	}
	fclose(vertexsfile);
} // RewriteVFile

void RewriteEFile(Map map) { // 重写路文件
	/*
	 * efilename；全局常量，路文件名
	 */
	FILE *edgesfile=fopen(efilename,"w"); // 以只写的方式打开文件（若文件有内容，会自动清空）
	int i,j;
	fprintf(edgesfile,"%d",map.arcnum);
	for(i=1; i<=map.vexnum; ++i) {
		for(j=i+1; j<=map.vexnum; ++j) {
			if(map.arcs[i][j].distance==INFINITY) continue;
			else fprintf(edgesfile,"\n%d %d %.1f %s",map.vexs[i].id,map.vexs[j].id,map.arcs[i][j].distance,map.arcs[i][j].einfo);
		}
	}
	fclose(edgesfile);
} // RewriteEFile

void EditId(Map &map) { // 编辑景点编号
	printf("---\n");
	printf("请输入需要编辑的景点编号：");
	index1=StandardizedId(map);
	printf("将其修改为：");
	do {
		scanf("%d",&id);
		index2=GetIndexById(map,id);
		if(index2!=-1) printf("景点编号%d已存在！请重新输入：",id);
	} while(index2!=-1);
	/* 在内存上修改 */
	map.vexs[index1].id=id;
	map.arcs[0][index1].distance=map.arcs[index1][0].distance=id;
	/* 同步到 Vertexs.txt 文件中 */
	RewriteVFile(map);
	/* 同步到 Edges.txt 文件中 */
	RewriteEFile(map);
	DisplayEditedAttraction(map,index1);
} // EditId

void EditName(Map &map) { // 编辑景点名称
	printf("---\n");
	printf("请输入需要编辑的景点名称：");
	index1=StandardizedName(map);
	printf("将其修改为：");
	do {
		scanf("%s",name);
		index2=GetIndexByName(map,name);
		if(index2!=-1) printf("景点“%s”已存在！请重新输入：",name);
	} while(index2!=-1);
	/* 在内存上修改 */
	strcpy(map.vexs[index1].name,name);
	/* 同步到 Vertexs.txt 文件中 */
	RewriteVFile(map);
	DisplayEditedAttraction(map,index1);
} // EditName

void EditVinfo(Map &map) { // 编辑景点信息
	/*
	 * edit[]：全局常量，值为"编辑"
	 * vinfo[]：全局变量，景点信息
	 */
	printf("---\n");
	index=FindOneIndexByIdOrName(map,edit);
	printf("将该景点的景点信息修改为：");
	scanf("%s",vinfo);
	/* 在内存上修改 */
	strcpy(map.vexs[index].vinfo,vinfo);
	/* 同步到 Vertexs.txt 文件中 */
	RewriteVFile(map);
	DisplayEditedAttraction(map,index);
} // EditVinfo

void EditAttraction(Map &map) { // 编辑景点的相关信息
	printf("---\n");
	printf("·(1)编辑景点编号\n·(2)编辑景点名称\n·(3)编辑景点信息\n·(4)退出编辑景点功能\n");
	printf("请选择：");
	z=StandardizedT(map,z,4,warnz);
	switch(z) {
		case 1:
			EditId(map);
			break;
		case 2:
			EditName(map);
			break;
		case 3:
			EditVinfo(map);
			break;
		case 4:
			printf("---\n");
			printf("成功退出编辑景点功能！！！\n");
			break;
	}
} // EditAttraction

void EditDistance(Map &map) { // 编辑路程
	/*
	 * edit[]：全局常量，值为"编辑"
	 */
	printf("---\n");
	printf("请输入与需要编辑的路相关联的两个景点：\n");
	FindTwoIndexByIdOrName(map,edit);
	printf("将路程修改为：");
	adj=StandardizedAdj(map);
	/* 在内存上修改 */
	map.arcs[index1][index2].distance=map.arcs[index2][index1].distance=adj;
	/* 同步到 Edges.txt 文件中 */
	RewriteEFile(map);
	printf("---\n");
	printf("新路程（m）矩阵（第1行和第1列表示景点编号，0表示无路）：\n");
	DisplayAdjMatrix(map);
} // EditDistance

void EditEinfo(Map &map) { // 编辑路信息（交通工具）
	/*
	 * edit[]：全局常量，值为"编辑"
	 * einfo[]：全局变量，路信息
	 */
	printf("---\n");
	printf("请输入与需要编辑的路相关联的两个景点：\n");
	FindTwoIndexByIdOrName(map,edit);
	printf("将路信息修改为：");
	scanf("%s",einfo);
	/* 在内存上修改 */
	strcpy(map.arcs[index1][index2].einfo,einfo);
	strcpy(map.arcs[index2][index1].einfo,einfo);
	/* 同步到 Edges.txt 文件中 */
	RewriteEFile(map);
	printf("---\n");
	printf("该路段的路信息更改为：%s\n",map.arcs[index1][index2].einfo);
} // EditEinfo

void EditWay(Map &map) { // 编辑路的相关信息
	printf("---\n");
	printf("·(1)编辑路程\n·(2)编辑路信息（交通工具）\n·(3)退出编辑路功能\n");
	printf("请选择：");
	z=StandardizedT(map,z,3,warnz);
	switch(z) {
		case 1:
			EditDistance(map);
			break;
		case 2:
			EditEinfo(map);
			break;
		case 3:
			printf("---\n");
			printf("成功退出编辑路功能！！！\n");
			break;
	}
} // EditWay

void Edit(Map &map) { // 功能 7：编辑景点、路相关信息
	/*
	 * warnz[]：全局常量，值为"无效选择！"
	 * z：全局变量，用户输入的功能编号
	 */
	printf("---\n·(1)编辑景点\n·(2)编辑路\n·(3)退出编辑功能\n");
	printf("请选择：");
	z=StandardizedT(map,z,3,warnz);
	switch(z) {
		case 1:
			EditAttraction(map);
			break;
		case 2:
			EditWay(map);
			break;
		case 3:
			printf("---\n");
			printf("成功退出编辑功能！！！\n");
			break;
	}
} // Edit

void AddAttraction(Map &map) { // 添加景点
	/*
	 * vfilename：全局常量，景点文件名
	 * efilename；全局常量，路文件名
	 * add[]：全局常量，值为"添加"
	 * index1、index2：全局变量，景点下标
	 * id1：全局变量，景点编号
	 * name[]：全局变量，景点名称
	 * vinfo[]：全局变量，景点信息
	 * einfo[]：全局变量，路信息
	 *
	 * n：需要添加的景点个数
	 * addmaxn：能添加的最大景点个数
	 * m：需要添加的与已添加景点相关的路个数
	 * addmaxm：能添加的最大路个数
	 * warnn[]：输入 n 错误时的提示信息
	 * warnm[]：输入 m 错误时的提示信息
	 */
	printf("---\n");
	printf("·(1)确认使用添加景点功能\n·(2)退出添加景点功能\n");
	printf("请选择：");
	z=StandardizedT(map,z,2,warnz);
	if(z==1) {
		int i,j,k,n,m,addmaxn=MAX_VERTEX_NUM-1-map.vexnum,addmaxm;
		char warnn[]="需添加景点个数超出规定范围！",warnm[]="需添加路的个数超出规定范围！";
		printf("---\n");
		printf("首先添加景点相关信息，请输入需要添加几个(1<=n<=%d)景点：",addmaxn);
		n=StandardizedT(map,n,addmaxn,warnn);
		for(i=1; i<=n; ++i) {
			addmaxm=((n+map.vexnum)*(n+map.vexnum+1))/2-map.arcnum;
			printf("---\n");
			printf("请输入第%d个新景点的景点名称：",i);
			do {
				scanf("%s",name);
				index1=GetIndexByName(map,name);
				if(index1!=-1) printf("景点“%s”已存在！请重新输入：",name);
			} while(index1!=-1);
			printf("请继续输入景点编号：");
			do {
				scanf("%d",&id1);
				index1=GetIndexById(map,id1);
				if(index1!=-1) printf("景点编号 %d 已存在！请重新输入：",id1);
			} while(index1!=-1);
			printf("请继续输入景点信息（可填“无”）：");
			scanf("%s",vinfo);
			/* 在内存上添加景点 */
			map.vexnum++;
			strcpy(map.vexs[map.vexnum].name,name);
			map.vexs[map.vexnum].id=id1;
			strcpy(map.vexs[map.vexnum].vinfo,vinfo);
			/* 同步到 Vertexs.txt 文件中 */
			RewriteVFile(map);
			printf("第%d个新景点的相关信息已添加成功！\n",i);
			printf("\n");
			index1=GetIndexById(map,id1);
			for(k=1; k<=map.vexnum; ++k) { // 将新景点与其它景点之间的路程初始化
				map.arcs[index1][k].distance=map.arcs[k][index1].distance=INFINITY;
			}
			printf("现在添加路相关信息，请输入需要添加几条(1<=m<=%d)与之相关的路：",addmaxm);
			m=StandardizedT(map,m,addmaxn,warnm);
			for(j=1; j<=m; ++j) {
				printf("请输入与第%d条路相关的景点：\n",j);
				index2=FindOneIndexByIdOrName(map,add);
				printf("请继续输入路程(>0)：");
				adj=StandardizedAdj(map);
				printf("请继续输入该路段的交通工具（无交通工具填“步行”）：");
				scanf("%s",einfo);
				/* 在内存上添加路 */
				map.arcnum++;
				map.arcs[index1][index2].distance=map.arcs[index2][index1].distance=adj;
				strcpy(map.arcs[index1][index2].einfo,einfo);
				strcpy(map.arcs[index2][index1].einfo,einfo);
				map.arcs[0][index1].distance=map.arcs[index1][0].distance=id1;
				/* 同步到 Edges.txt 文件中 */
				RewriteEFile(map);
				printf("第%d个新景点的第%d条路已添加成功！\n",i,j);
				printf("\n");
			} // end for j
			printf("第%d个新景点及其相关的路已添加成功！\n",i);
		} // end for i
		printf("---\n");
		printf("%d个新景点及其相关的路均已添加成功！！！\n",n);
		printf("---\n");
		printf("新地图：");
		DisplayMap(map);
	} // end if z
	else {
		printf("---\n");
		printf("成功退出添加景点功能！！！\n");
	}
} // AddAttraction

void AddWay(Map &map) { // 添加路
	printf("---\n");
	printf("·(1)确认使用添加路功能\n·(2)退出添加路功能\n");
	printf("请选择：");
	z=StandardizedT(map,z,2,warnz);
	if(z==1) {
		int i,m,addmaxm=(map.vexnum*(map.vexnum+1))/2-map.arcnum;
		char warnm[]="需添加路的个数超出规定范围！";
		printf("---\n");
		printf("请输入需要添加几条(1<=m<=%d)路：",addmaxm);
		m=StandardizedT(map,m,addmaxm,warnm);
		for(i=1; i<=m; ++i) {
			printf("---\n");
			printf("请输入与需要添加的第%d条路相关联的两个景点：\n",i);
			do {
				FindTwoIndexByIdOrName(map,add);
				if(map.arcs[index1][index2].distance!=INFINITY) printf("这两个景点之间有路！请重新输入：\n");
			} while(map.arcs[index1][index2].distance!=INFINITY);
			printf("请继续输入路程(>0)：");
			adj=StandardizedAdj(map);
			printf("请继续输入该路段的交通工具（无交通工具填“步行”）：");
			scanf("%s",einfo);
			/* 在内存上添加路 */
			map.arcnum++;
			map.arcs[index1][index2].distance=map.arcs[index2][index1].distance=adj;
			strcpy(map.arcs[index1][index2].einfo,einfo);
			strcpy(map.arcs[index2][index1].einfo,einfo);
			/* 同步到 Edges.txt 文件中 */
			RewriteEFile(map);
			printf("第%d条路已添加成功！\n",i);
		} // end for i
		printf("---\n");
		printf("%d条路均已添加成功！！！\n",m);
		printf("---\n");
		printf("新路程（m）矩阵（第1行和第1列表示景点编号，0表示无路）：\n");
		DisplayAdjMatrix(map);
	} // end if z
	else {
		printf("---\n");
		printf("成功退出添加路功能！！！\n");
	}
} // AddWay

void Add(Map &map) { // 功能 8：添加景点、路相关信息
	/*
	 * warnz[]：全局常量，值为"无效选择！"
	 * z：全局变量，用户输入的功能编号
	 *
	 * arcmax：旅游区能修的路个数的最大值
	 */
	int arcmax=(map.vexnum*(map.vexnum+1))/2;
	printf("\n·(1)添加景点\n·(2)添加路\n·(3)退出添加功能\n");
	printf("请选择：");
	z=StandardizedT(map,z,3,warnz);
	switch(z) {
		case 1:
			if(map.vexnum>=MAX_VERTEX_NUM) printf("旅游区景点已满，无法再添加景点！\n");
			else AddAttraction(map);
			break;
		case 2:
			if(map.arcnum>=arcmax) printf("旅游区各景点之间已互相直接可达，无需再添加路！\n");
			else AddWay(map);
			break;
		case 3:
			printf("---\n");
			printf("成功退出添加功能！！！\n");
			break;
	}
} // Add

void DeleteAttraction(Map &map) { // 删除景点
	/*
	 * vfilename：全局常量，景点文件名
	 * efilename；全局常量，路文件名
	 * erase[]：全局常量，值为"删除"
	 * index：全局变量，景点下标
	 *
	 * n：需要删除的景点个数
	 * deletemaxn：能删除的最大景点个数
	 * m：整个旅游区的路个数
	 * deletem：与需要删除的景点相关的路个数
	 * warnn[]：输入 n 错误时的提示信息
	 */
	printf("---\n");
	printf("·(1)确认使用删除景点功能\n·(2)退出删除景点功能\n");
	printf("请选择：");
	z=StandardizedT(map,z,2,warnz);
	if(z==1) {
		int i,j,k,n,m,deletemaxn=map.vexnum-1,deletem;
		char warnn[]="需删除的景点个数超出规定范围！";
		printf("---\n");
		printf("请输入需要删除几个(1<=n<=%d)景点：",deletemaxn);
		n=StandardizedT(map,n,deletemaxn,warnn);
		for(i=1; i<=n; ++i) {
			deletem=0;
			printf("---\n");
			printf("请输入需要删除的第%d个景点：\n",i);
			index=FindOneIndexByIdOrName(map,erase);
			m=map.arcnum;
			for(j=1; j<=map.vexnum; ++j) {
				if(map.arcs[index][j].distance!=INFINITY) {
					m--;
					deletem++;
				}
			}
			if(m>=map.vexnum-1) {
				/* 在内存上删除景点及其相关的路 */
				map.arcnum-=deletem; // 更新路数
				for(j=0; j<=map.vexnum; ++j) {
					for(k=index; k<map.vexnum; ++k) map.arcs[j][k]=map.arcs[j][k+1]; // 移动待删除顶点之后的矩阵元素
				}
				for(j=0; j<=map.vexnum; ++j) {
					for(k=index; k<map.vexnum; ++k) map.arcs[k][j]=map.arcs[k+1][j]; // 移动待删除顶点之下的矩阵元素
				}
				printf("第%d个景点“%s”已成功删除！\n",i,map.vexs[index].name);
				for(j=index; j<map.vexnum; ++j) map.vexs[j]=map.vexs[j+1];
				map.vexnum--; // 更新顶点数
				/* 同步到 Vertexs.txt 文件中 */
				RewriteVFile(map);
				/* 同步到 Edges.txt 文件中 */
				RewriteEFile(map);
			} // end if m
			else printf("删除景点“%s”会导致旅游区各景点不互通，禁止删除该景点\n",map.vexs[index].name);
		} // end for i
		printf("---\n");
		printf("%d个景点均已成功删除！！！\n",n);
		printf("---\n");
		printf("新地图：");
		DisplayMap(map);
	} // end if z
	else {
		printf("---\n");
		printf("成功退出删除景点功能！！！\n");
	}
} // DeleteAttraction

void DeleteWay(Map &map) { // 删除路
	/*
	 * index1、index2：全局变量，景点下标
	 *
	 * warnm[]：输入 m 错误时的提示信息
	 */
	printf("---\n");
	printf("·(1)确认使用删除路功能\n·(2)退出删除路功能\n");
	printf("请选择：");
	z=StandardizedT(map,z,2,warnz);
	if(z==1) {
		int i,j,k,m,deletemaxm=map.arcnum-map.vexnum+1;
		char warnm[]="需删除路的个数超出规定范围！";
		printf("---\n");
		printf("请输入需要删除几条(1<=m<=%d)路：",deletemaxm);
		m=StandardizedT(map,m,deletemaxm,warnm);
		for(i=1; i<=m; ++i) {
			printf("---\n");
			printf("请输入与需要删除的第%d条路相关联的两个景点：\n",i);
			FindTwoIndexByIdOrName(map,erase);
			/* 在内存上删除路 */
			map.arcs[index1][index2].distance=map.arcs[index2][index1].distance=INFINITY;
			strcpy(map.arcs[index1][index2].einfo,"");
			strcpy(map.arcs[index2][index1].einfo,"");
			map.arcnum--;
			/* 同步到 Edges.txt 文件中 */
			RewriteEFile(map);
			printf("第%d条路已成功删除！\n",i);
		}// end for i
		printf("---\n");
		printf("%d条路均已成功删除！！！\n",m);
		printf("---\n");
		printf("新路程（m）矩阵（第1行和第1列表示景点编号，0表示无路）：\n");
		DisplayAdjMatrix(map);
	} // end if z
	else {
		printf("---\n");
		printf("成功退出删除路功能！！！\n");
	}
} // DeleteWay

void Delete(Map &map) { // 功能 9：删除景点、路相关信息
	/*
	 * warnz[]：全局常量，值为"无效选择！"
	 * z：全局变量，用户输入的功能编号
	 */
	printf("\n·(1)删除景点\n·(2)删除路\n·(3)退出删除功能\n");
	printf("请选择：");
	z=StandardizedT(map,z,3,warnz);
	switch(z) {
		case 1:
			if(map.vexnum==1) printf("旅游区仅有1个景点，禁止删除最后一个景点！\n");
			else DeleteAttraction(map);
			break;
		case 2:
			if(map.arcnum==map.vexnum-1) printf("继续删除路会导致旅游区不互通，禁止继续删除路！\n");
			else DeleteWay(map);
			break;
		case 3:
			printf("---\n");
			printf("成功退出删除功能！\n");
			break;
	}
} // Delete
