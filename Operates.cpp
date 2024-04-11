/* ȫ�ֳ����ͱ������ظ������ڴ�ռ� */
int flag;                                                      // ���ڹ淶��������
int z;                                                         // ����ѡ��ı��
int id,id1,id2;                                                // ��ȡ�ľ�����
int index,index1,index2,finalindex;                            // �����±�
int pre,vex[MAX_VERTEX_NUM],P[MAX_VERTEX_NUM];                 // ǰ�������±�
int path[MAX_VERTEX_NUM];                                      // ·���ϵľ����±�
int shortestpaths[MAX_VERTEX_NUM][MAX_VERTEX_NUM];             // ����·����ȵ���̼�·���ĸ������±�
int pathnum,numinpath[MAX_VERTEX_NUM];                         // ��̼�·����������·���Ͼ������
double D[MAX_VERTEX_NUM],totaldistance;                        // ��̼�·��·��
double adj;                                                    // ��ȡ��·��
char vfilename[]="Vertexs.txt",efilename[]="Edges.txt";        // �ֱ��ž��㡢·�������Ϣ�� txt �ļ���
char warnz[]="��Чѡ��";                                     // ����ѡ�����������ʱ�ľ�����Ϣ
char input[5],case1[]="id",case2[]="name";                     // �����û����� id �� name ʹ����ع���
char find[]="��ѯ",edit[]="�༭",add[]="���",erase[]="ɾ��";  // �û�ѡ�� id �� name ʹ����ع���ʱ����ʾ��Ϣ
char name[MAX_NAME];                                           // ��ȡ�ľ�������
InfoType vinfo[MAX_INFO],einfo[MAX_INFO];                      // ��ȡ�ľ�����Ϣ��·��Ϣ
bool visited[MAX_VERTEX_NUM],visited1[MAX_VERTEX_NUM],visited2[MAX_VERTEX_NUM]; // �жϾ����Ƿ񱻷��ʹ�

void MadeMenuCenter() { // ʹ�������˵���ž��ڴ��ڶ����м�
	int i;
	printf("\n");
	for(i=1; i<=9; ++i) printf("\t");
} // MadeMenuCenter

void DisplayStarLine(int len) { // ��ӡһ�����Ǻ�
	int i;
	MadeMenuCenter();
	for(i=1; i<=len; ++i) printf("*");
} // DisplayStarLine

void DisplayHollowLine(int len) { // ��ӡ���ߴ��ǺŵĿ���
	int i;
	MadeMenuCenter();
	printf("*");
	for(i=1; i<=len-2; ++i) printf(" ");
	printf("*");
} // DisplayHollowLine

void RenderOneWords(int len,int len1,char *content,int len2) { // ��������ֵ�һ��
	int i;
	DisplayHollowLine(len);
	MadeMenuCenter();
	printf("*");
	for(i=1; i<=len1; ++i) printf(" ");
	printf("%s",content);
	for(i=1; i<=len2; ++i) printf(" ");
	printf("*");
} // RenderOneWords

void RenderMenu() { // ��Ⱦҳ�����˵�
	int i,spacelen=22,starlen=18+2*spacelen;
	char systemname[]="���������㵼��ϵͳ";
	char content[10][33]= {"<1> �鿴��������ͼ",
	                       "<2> ��ѯ���㵽������������·��",
	                       "<3> ��ѯ�������ļ�·��",
	                       "<4> ��ѯ����������̼�·��",
	                       "<5> �������·���Ƽ�",
	                       "<6> ��ѯ���㡢·�����Ϣ",
	                       "<7> �༭���㡢·�����Ϣ",
	                       "<8> ��Ӿ��㡢·�����Ϣ",
	                       "<9> ɾ�����㡢·�����Ϣ",
	                       "<0> �˳����������㵼��ϵͳ"
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

void RenderFunctionTop(char *title) { // ��Ⱦÿ���������Ķ���
	int i,j=0,len=13*14,clen=strlen(title),llen=(len-clen)/2;
	for(i=1; i<=len; ++i) {
		if(i<=llen) printf("*");
		else if(i<=llen+clen) printf("%c",title[j++]);
		else printf("*");
	}
	printf("\n---");
} // RenderFunctionTop

void RenderFunctionBottom() { // ��Ⱦÿ���������ĵײ�
	int i;
	printf("---\n");
	for(i=1; i<=13*14; ++i) printf("*");
	printf("\n\n\n");
} // RenderFunctionBottom

bool isFullVisited(Map map,bool *visitedx) { // �жϾ����Ƿ�ȫ�����ʵ�
	/*
	 * visitedx[i]���±�Ϊ i �ľ����Ƿ���ʹ�
	 */
	int i;
	for(i=1; i<=map.vexnum; ++i) {
		if(!visitedx[i]) return false;
	}
	return true;
} // isFullVisited

int GetIndexById(Map map,int number) { // ���ݾ����Ż���±꣬���������򷵻� -1
	/*
	 * number��������
	 */
	int i;
	for(i=1; i<=map.vexnum; ++i) {
		if(map.vexs[i].id==number) return i;
	}
	return -1;
} // GetIndexById

int GetIndexByName(Map map,char *designation) { // ���ݾ������ƻ���±꣬���������򷵻� -1
	/*
	 * designation[]����������
	 */
	int i;
	for(i=1; i<=map.vexnum; ++i) {
		if(strcmp(map.vexs[i].name,designation)==0) return i;
	}
	return -1;
} // GetIndexByName

void CreatMap(Map &map) { // ����һ����������ͼ
	int i,j;
	FILE *vertexsfile=fopen(vfilename,"r"),*edgesfile=fopen(efilename,"r"); // ��ֻ����ʽ���ļ�
	fscanf(vertexsfile,"%d",&map.vexnum);
	fscanf(edgesfile,"%d",&map.arcnum);
	for(i=1; i<=map.vexnum; ++i) fscanf(vertexsfile,"%s %d %s",map.vexs[i].name,&map.vexs[i].id,map.vexs[i].vinfo); // ���뾰�������Ϣ
	/* ��ʼ���ڽӾ��� */
	for(i=0; i<=map.vexnum; ++i) {
		for(j=0; j<=map.vexnum; ++j) {
			/* ���� 1 �С��� 1 �г�ʼ��Ϊ������ */
			if(i==0) map.arcs[i][j+1].distance=map.vexs[j+1].id;
			if(j==0) map.arcs[i+1][j].distance=map.vexs[i+1].id;
			if(i!=0&&j!=0) map.arcs[i][j].distance=INFINITY; // ��
		}
	}
	/* ����·�����Ϣ */
	for(i=1; i<=map.arcnum; ++i) {
		fscanf(edgesfile,"%d %d %lf %s",&id1,&id2,&adj,einfo);
		index1=GetIndexById(map,id1);
		index2=GetIndexById(map,id2);
		/* ����ͼ */
		map.arcs[index1][index2].distance=map.arcs[index2][index1].distance=adj;
		strcpy(map.arcs[index1][index2].einfo,einfo);
		strcpy(map.arcs[index2][index1].einfo,einfo);
	}
	fclose(vertexsfile);
	fclose(edgesfile);
} // CreatMap

void DisplayEditedAttraction(Map map,int subscript) { // ��ʾ�±�Ϊ subscript �ľ��������Ϣ
	/*
	 * subscript�������±�
	 */
	printf("---\n");
	printf("�þ���������Ϣ���Ϊ��\n");
	printf("�������ţ�%d\n���������ƣ�%s\n��������Ϣ��%s\n",map.vexs[subscript].id,map.vexs[subscript].name,map.vexs[subscript].vinfo);
} // DisplayEditedAttraction

void DisplayAttraction(Map map,int subscript) { // ��ʾ���Ҿ��㡢�����ھ��������֮���·�������Ϣ
	/*
	 * subscript�������±�
	 */
	int i;
	printf("---\n");
	printf("���ҵľ���������Ϣ���£�\n");
	printf("�������ţ�%d\n���������ƣ�%s\n��������Ϣ��%s\n",map.vexs[subscript].id,map.vexs[subscript].name,map.vexs[subscript].vinfo);
	printf("---\n");
	printf("�뾰�㡰%s�����ڵľ����� <��ʽ�� ����������(id)��������Ϣ����ͨ����(·��)>��\n",map.vexs[subscript].name);
	for(i=1; i<=map.vexnum; ++i) {
		if(map.arcs[subscript][i].distance!=INFINITY) {
			printf("��%s(%d)��%s��%s(%.1fm)\n",map.vexs[i].name,map.vexs[i].id,map.vexs[i].vinfo,map.arcs[subscript][i].einfo,map.arcs[subscript][i].distance);
		}
	}
} // DisplayAttraction

void DisplayAdjMatrix(Map map) { // ��ʾ�ڽӾ���
	int i,j;
	for(i=0; i<=map.vexnum; ++i) {
		for(j=0; j<=map.vexnum; ++j) {
			if(i==0||j==0) printf("%10d",(int)map.arcs[i][j].distance);    // �� 1 �С��� 1 ����ʾ������
			if(i!=0&&j!=0) {
				if(map.arcs[i][j].distance==INFINITY) printf("%10.1f",0);  // ��·����ʾ 0
				else printf("%10.1f",map.arcs[i][j].distance);
			}
		}
		printf("\n");
	}
} // DisplayAdjMatrix

void DisplayMap(Map map) { // ���� 1���鿴��������ͼ
	int i;
	printf("\n���������ܹ���%d�����㣬%d��·��",map.vexnum,map.arcnum);
	printf("\n%d��������Ϣ <��ʽ������������ ������> �ֱ�Ϊ��\n",map.vexnum);
	for(i=1; i<=map.vexnum; ++i) printf("��%s  %d\n",map.vexs[i].name,map.vexs[i].id);
	printf("������֮���·�̣�m�����󣨵�1�к͵�1�б�ʾ�����ţ�0��ʾ��·����\n");
	DisplayAdjMatrix(map);
} // DisplayMap

int StandardizedT(Map map,int t,int max,char *warning) { // ��֤ t ������ȷ�������� t
	/*
	 * t���û����������
	 * warning[]��������Ϣ
	 * max��k ������
	 */
	do {
		scanf("%d",&t);
		if(t<1||t>max) printf("%s���������룺",warning);
	} while(t<1||t>max);
	return t;
} // StandardizedT

int StandardizedId(Map map) { // �淶���������룬�����ض�Ӧ�±�
	/*
	 * index��ȫ�ֱ����������±�
	 * id��ȫ�ֱ�����������
	 */
	do {
		scanf("%d",&id);
		index=GetIndexById(map,id);
		if(index==-1) printf("������%d�����ڣ����������룺",id);
	} while(index==-1);
	return index;
} // StandardizedId

int StandardizedName(Map map) { // �淶�����������룬�����ض�Ӧ�±�
	/*
	 * index��ȫ�ֱ����������±�
	 * name[]��ȫ�ֱ�������������
	 */
	do {
		scanf("%s",name);
		index=GetIndexByName(map,name);
		if(index==-1) printf("���㡰%s�������ڣ����������룺",name);
	} while(index==-1);
	return index;
} // StandardizedName

double StandardizedAdj(Map map) { // �淶·�����룬������·��
	/*
	 * adj��ȫ�ֱ�����·��
	 */
	do {
		scanf("%lf",&adj);
		if(adj<=0) printf("·�̱���Ϊ���������������룺");
	} while(adj<=0);
	return adj;
} // StandardizedAdj

int FindOneIndexByIdOrName(Map map,char *choose) { // ���û�����ѡ����ݵ�������� id ���� name ��ѯ�����±�
	/*
	 * case1[]��ȫ�ֳ����������ж��û������Ƿ�Ϊ��id��
	 * case2[]��ȫ�ֳ����������ж��û������Ƿ�Ϊ��name��
	 * flag��ȫ�ֱ������ڴ˺��������ڹ淶�û�����
	 * input[]��ȫ�ֱ����������û�������ַ���
	 * index��ȫ�ֱ����������±�
	 *
	 * choose[]��ʹ�ù��ܵ�����
	 */
	printf("��ѡ��ͨ����������(id)�򾰵���(name)����%s�����롰id����name������",choose);
	do {
		flag=0;
		scanf("%s",input);
		if(strcmp(input,case1)==0) {
			printf("�����뾰���ţ�");
			index=StandardizedId(map);
		} else if(strcmp(input,case2)==0) {
			printf("�����뾰�����ƣ�");
			index=StandardizedName(map);
		} else {
			flag=1;
			printf("���벻����Ҫ�����������룺");
		}
	} while(flag==1);
	return index;
} // FindOneIndexByIdOrName

void FindTwoIndexByIdOrName(Map map,char *choose) { // ���û�����ѡ�������������� id ���� name ��ѯ�����±�
	/*
	 * case1[]��ȫ�ֳ����������ж��û������Ƿ�Ϊ��id��
	 * case2[]��ȫ�ֳ����������ж��û������Ƿ�Ϊ��name��
	 * flag��ȫ�ֱ������ڴ˺��������ڹ淶�û�����
	 * input[]��ȫ�ֱ����������û�������ַ���
	 * index1��index2��ȫ�ֱ����������±�
	 *
	 * choose[]��ʹ�ù��ܵ�����
	 */
	printf("��ѡ��ͨ����������(id)�򾰵���(name)����%s�����롰id����name������",choose);
	do {
		flag=0;
		scanf("%s",input);
		if(strcmp(input,case1)==0) {
			printf("�뵥��������Ҫ��ѯ���������ţ�\n");
			printf("����1��");
			index1=StandardizedId(map);
			printf("����2��");
			index2=StandardizedId(map);
		} else if(strcmp(input,case2)==0) {
			printf("�뵥��������Ҫ��ѯ�����������ƣ�\n");
			printf("����1��");
			index1=StandardizedName(map);
			printf("����2��");
			index2=StandardizedName(map);
		} else {
			flag=1;
			printf("���벻����Ҫ�����������룺");
		}
	} while(flag==1);
} // FindTwoByIdOrName

void PrintfPath(Map map,int subscript) { // �淶·�����
	/*
	 * subscript�������±�
	 */
	if(flag==0) {
		printf("%s",map.vexs[subscript].name);
		flag=1;
	} else printf("-->%s",map.vexs[subscript].name);
} // PrintfPath

void ShortestPath_DIJ(Map map,int v0,int *P,double *D) { // Dijkstra �㷨�����·��
	/*
	 * v0����㾰����±�
	 * visited[i]��ȫ�ֱ����������ж��±�Ϊ i �ľ����Ƿ񱻷��ʹ�
	 * P[i]��ȫ�ֱ����������·�����±�Ϊ i �ľ����ǰ�������±�
	 * D[i]��ȫ�ֱ�������㾰�㵽�±�Ϊ i �ľ���֮�����·����·��
	 */
	int i,v,w;
	double min;
	for(v=1; v<=map.vexnum; ++v) {
		visited[v]=false;
		D[v]=map.arcs[v0][v].distance;                             // ��ʼ�� D[i] Ϊ������ i �� v0 ֮���·��
		if(D[v]!=INFINITY) P[v]=v0;                                // �� v0 �� v ֮����·���� v ��ǰ��Ϊ v0
		else P[v]=0;                                               // ������ǰ����0��
	}
	D[v0]=0;                                                       // v0 ���Լ������·��Ϊ 0
	visited[v0]=true;                                              // ���·���� v0 ��ʼ
	/* ��ʼ��ѭ����ÿ����� v0 ��ĳ������ v �����·�̣��� v ��Ϊ���ʹ����������·���� */
	for(i=2; i<=map.vexnum; ++i) {                                 // ���� map.vexnum-1 ������
		min=INFINITY;                                              // ��ǰ��֪�� v0 ����ľ���
		/* ���� v0 ����ľ��� v */
		for(w=1; w<=map.vexnum; ++w) {
			if(!visited[w]&&D[w]<min) {
				v=w;
				min=D[w];
			}
		}
		visited[v]=true;                                           // �� v0 ����ľ��� v �������·����
		/* ���µ�ǰ���·����·�� */
		for(w=1; w<=map.vexnum; ++w) {
			if(!visited[w]&&(min+map.arcs[v][w].distance<D[w])) {  // ��� v0-->v-->w ��·��С�� v0-->w�� ����� D �� P
				D[w]=min+map.arcs[v][w].distance;
				P[w]=v;
			}
		}
	} // end for i
} // ShortestPath_DIJ

void FindAllShortestPath(Map map,int vi,int vj,int *path,int d) { // ���� Dijkstra �㷨�� vi �� vj ֮����������·����·��
	/*
	 * visited[i]��ȫ�ֱ����������ж��±�Ϊ i �ľ����Ƿ񱻷��ʹ�
	 * path[i]��ȫ�ֱ�����˳����һ������·�������о����±�
	 * pathnum��ȫ�ֱ��������·��������
	 * numinpath[i]��ȫ�ֱ������� i �����·���о���ĸ���
	 * shortestpaths[i][j]��ȫ�ֱ�����˳���ŵ� i �����·�������о����±�
	 * totaldistance��ȫ�ֱ�����vi �� vj ֮���·����·��

	 * d����·���еľ������
	 */
	int i,len;
	totaldistance=0;
	++d;
	path[d]=vi; // �� vi ��ʼ
	visited[vi]=true;
	/* �ҵ�һ�����·���ʹ�һ�� */
	if(vi==vj) {
		len=d;
		for(i=1; i<len; ++i) totaldistance+=map.arcs[path[i]][path[i+1]].distance;
		for(i=1; i<=len; ++i) {
			if(totaldistance==D[vj]) shortestpaths[pathnum][i]=path[i]; // �����·������ shortestpaths ��
		}
		if(totaldistance==D[vj]) {
			numinpath[pathnum]=len;
			pathnum++;
		}
		visited[vi]=false; // ����һ�����·���� vi ����Ϊδ���ʣ�����������һ�����·��
		return;
	} // end if(vi==vj)
	for(i=1; i<=map.vexnum; ++i) {
		if(!visited[i]&&map.arcs[vi][i].distance!=INFINITY) FindAllShortestPath(map,i,vj,path,d); // �ݹ�
	}
	visited[vi]=false; // for ѭ��������ζ�ţ�vi ���ھ���Ҫô�����ʹ���Ҫô�� vi ��·����ͬ����δ���ʺ����
} // FindAllShortestPath

void AllShortestPathByOne(Map map) { // ���� 2����ѯ���㵽������������·��
	/*
	 * find[]��ȫ�ֳ�����ֵΪ"��ѯ"
	 * flag��ȫ�ֱ������˺��������ڹ淶·�����
	 */
	int v,i,j,d;
	printf("\n");
	index=FindOneIndexByIdOrName(map,find);                 // ����û����뾰����±�
	for(i=1; i<=map.vexnum; ++i) visited[i]=false;          // ��ʼ����������
	ShortestPath_DIJ(map,index,P,D);                        // ����û����뾰�㵽������һ����֮������·��
	printf("---\n");
	printf("���㡰%s����������һ�������̼�·����·�̣�\n",map.vexs[index].name);
	printf("---\n");
	for(v=1; v<=map.vexnum; ++v) {
		if(D[v]!=0) {
			d=0;                                            // ���� d
			pathnum=1;                                      // ÿ����� index �� v ���������·�������� pathnum
			printf("��%s����%5.1f(m)\n",map.vexs[v].name,D[v]);
			for(i=1; i<=map.vexnum; ++i) visited[i]=false;  // ���� FindAllShortestPath ֮ǰҲҪ��ʼ����������
			FindAllShortestPath(map,index,v,path,d);        // ���±�Ϊ index �� v ����֮����������·������ shortestpaths ��
			for(i=1; i<pathnum; ++i) {
				flag=0;                                     // ÿ�����һ�����·�������� flag
				printf("��");
				for(j=1; j<=numinpath[i]; ++j) PrintfPath(map,shortestpaths[i][j]);
				printf("\n");
			}
			if(v<map.vexnum) printf("---\n");
		}
	}
} // AllShortestPathByOne

void FindAllSimplePath(Map map,int vi,int vj,int *path,int d) { // ����� vi �� vj ֮������м�·����·��
	/*
	 * ���޸��Ժ��� FindAllShortestPath��
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
		printf("%5.1f(m)��",totaldistance);
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

void AllSimplePathByTwo(Map map) { // ���� 3����ѯ�������ļ�·��
	/*
	 * find[]��ȫ�ֳ�����ֵΪ"��ѯ"
	 */
	int i,d=0;
	printf("\n");
	for(i=1; i<=map.vexnum; ++i) visited[i]=false;
	FindTwoIndexByIdOrName(map,find); // ����û�������������±�
	printf("---\n");
	printf("���㡰%s�������㡰%s�������м�·����·�̣�\n",map.vexs[index1].name,map.vexs[index2].name);
	FindAllSimplePath(map,index1,index2,path,d);
} // AllSimplePathByTwo

void AllShortestPathByTwo(Map map) { // ���� 4����ѯ����������̼�·��
	/*
	 * find[]��ȫ�ֳ�����ֵΪ"��ѯ"
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
	printf("���㡰%s�������㡰%s����������̼�·����·�̣�\n",map.vexs[index1].name,map.vexs[index2].name);
	for(i=1; i<pathnum; ++i) {
		flag=0;
		printf("��%5.1f(m)��",D[index2]);
		for(j=1; j<=numinpath[i]; ++j) PrintfPath(map,shortestpaths[i][j]);
		printf("\n");
	}
} // AllShortestPathByTwo

bool BestVisitPath_DIJ(Map map,int subscript) { // ���� Dijsktra �㷨��˼·���������·�߼�·��
	/*
	 * visited1[]��ȫ�ֱ�����ÿ��������Ϊ�յ�ķ������
	 * visited2[]��ȫ�ֱ�����·�����߹���ÿ������ķ������
	 * finalindex��ȫ�ֱ��������յ��յ��±�
	 * pre��ȫ�ֱ�����ǰ�������±�
	 * totaldistance��ȫ�ֱ�����·��·��
	 *
	 * subscript�������±�
	 * minindex���� visited1 δ�����ʹ������о�����ѡȡ�� subscript ��·����̵ľ����±�
	 * min��subscript �� minindex ��·��
	 * vex[]��˳�����������·���ϵ����о����±�
	 */
	int i,j=1,k,m=1,n,minindex;
	double min;
	visited1[subscript]=true;            // ÿ���������Ϊ���ʹ�
	if(isFullVisited(map,visited1)&&!isFullVisited(map,visited2)) return false; // ���ÿ�����㶼��Ϊ�յ����������������û�о������о���
	ShortestPath_DIJ(map,subscript,P,D); // ����±�Ϊ subscript �ľ��㵽������������·����·��
	min=INFINITY;
	/* ÿ���� visited1 δ�����ʹ������о�����ѡȡ�� subscript ��·����̵ľ��� */
	for(i=1; i<=map.vexnum; ++i) {
		if(D[i]!=0&&D[i]<min&&!visited1[i]) {
			min=D[i];
			minindex=i;
		}
	}
	/* ��·����̵�����·���ϵ����о����±���� vex �� */
	pre=P[minindex];
	while(pre!=0) {
		vex[m++]=pre;
		pre=P[pre];
	}
	/* �������·���ϳ� mindex �����о��㣬���� visited2 ���þ����ľ���Ϊ���ʹ� */
	for(n=m-1; n>=1; --n) {
		PrintfPath(map,vex[n]);
		visited2[vex[n]]=true;
	}
	visited2[minindex]=true;    // ������·�����յ�Ҳ��Ϊ���ʹ�
	totaldistance+=D[minindex]; // ͳ����·�̣�ֱ�� visited2 ȫΪ true
	/* ��� visited2 ȫΪ true����˵���Ѿ��ҵ�һ�����������·����������յ��յ��±�󣬷��� true */
	if(isFullVisited(map,visited2)) {
		finalindex=minindex;
		return true;
	}
	if(BestVisitPath_DIJ(map,minindex)) return true; // �����ѡ����·���յ���Ϊ�������ݹ�ֱ�� visited2 ȫΪ true
	else return false;
} // BestVisitPath_DIJ

void BestVisitPath(Map map) { // ���� 5���������·���Ƽ�
	int i;
	flag=0;
	for(i=1; i<=map.vexnum; ++i) {      // ��ʼ����������
		visited1[i]=false;
		visited2[i]=false;
	}
	printf("\n");
	index=FindOneIndexByIdOrName(map,find);
	printf("---\n");
	printf("�Ӿ��㡰%s���������������·�߼�·�̣�\n",map.vexs[index].name);
	if(BestVisitPath_DIJ(map,index)) {  // ��������������������·�ߣ������·����·��
		PrintfPath(map,finalindex);     // ������յ��յ�
		printf("��%5.1f(m)\n",totaldistance);
	} else printf("�����������������·��\n");
} // BestVisitPath

void AttractionInformation(Map map) { // ���� 6����ѯ���㡢·�����Ϣ
	/*
	 * find[]��ȫ�ֳ�����ֵΪ"��ѯ"
	 */
	printf("n");
	index=FindOneIndexByIdOrName(map,find);
	DisplayAttraction(map,index);
} // AttractionInformation

void RewriteVFile(Map map) { // ��д�����ļ�
	/*
	 * vfilename��ȫ�ֳ����������ļ���
	 */
	FILE *vertexsfile=fopen(vfilename,"w"); // ��ֻд�ķ�ʽ���ļ������ļ������ݣ����Զ���գ�
	int i;
	fprintf(vertexsfile,"%d",map.vexnum);
	for(i=1; i<=map.vexnum; ++i) {
		fprintf(vertexsfile,"\n%s %d %s",map.vexs[i].name,map.vexs[i].id,map.vexs[i].vinfo);
	}
	fclose(vertexsfile);
} // RewriteVFile

void RewriteEFile(Map map) { // ��д·�ļ�
	/*
	 * efilename��ȫ�ֳ�����·�ļ���
	 */
	FILE *edgesfile=fopen(efilename,"w"); // ��ֻд�ķ�ʽ���ļ������ļ������ݣ����Զ���գ�
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

void EditId(Map &map) { // �༭������
	printf("---\n");
	printf("��������Ҫ�༭�ľ����ţ�");
	index1=StandardizedId(map);
	printf("�����޸�Ϊ��");
	do {
		scanf("%d",&id);
		index2=GetIndexById(map,id);
		if(index2!=-1) printf("������%d�Ѵ��ڣ����������룺",id);
	} while(index2!=-1);
	/* ���ڴ����޸� */
	map.vexs[index1].id=id;
	map.arcs[0][index1].distance=map.arcs[index1][0].distance=id;
	/* ͬ���� Vertexs.txt �ļ��� */
	RewriteVFile(map);
	/* ͬ���� Edges.txt �ļ��� */
	RewriteEFile(map);
	DisplayEditedAttraction(map,index1);
} // EditId

void EditName(Map &map) { // �༭��������
	printf("---\n");
	printf("��������Ҫ�༭�ľ������ƣ�");
	index1=StandardizedName(map);
	printf("�����޸�Ϊ��");
	do {
		scanf("%s",name);
		index2=GetIndexByName(map,name);
		if(index2!=-1) printf("���㡰%s���Ѵ��ڣ����������룺",name);
	} while(index2!=-1);
	/* ���ڴ����޸� */
	strcpy(map.vexs[index1].name,name);
	/* ͬ���� Vertexs.txt �ļ��� */
	RewriteVFile(map);
	DisplayEditedAttraction(map,index1);
} // EditName

void EditVinfo(Map &map) { // �༭������Ϣ
	/*
	 * edit[]��ȫ�ֳ�����ֵΪ"�༭"
	 * vinfo[]��ȫ�ֱ�����������Ϣ
	 */
	printf("---\n");
	index=FindOneIndexByIdOrName(map,edit);
	printf("���þ���ľ�����Ϣ�޸�Ϊ��");
	scanf("%s",vinfo);
	/* ���ڴ����޸� */
	strcpy(map.vexs[index].vinfo,vinfo);
	/* ͬ���� Vertexs.txt �ļ��� */
	RewriteVFile(map);
	DisplayEditedAttraction(map,index);
} // EditVinfo

void EditAttraction(Map &map) { // �༭����������Ϣ
	printf("---\n");
	printf("��(1)�༭������\n��(2)�༭��������\n��(3)�༭������Ϣ\n��(4)�˳��༭���㹦��\n");
	printf("��ѡ��");
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
			printf("�ɹ��˳��༭���㹦�ܣ�����\n");
			break;
	}
} // EditAttraction

void EditDistance(Map &map) { // �༭·��
	/*
	 * edit[]��ȫ�ֳ�����ֵΪ"�༭"
	 */
	printf("---\n");
	printf("����������Ҫ�༭��·��������������㣺\n");
	FindTwoIndexByIdOrName(map,edit);
	printf("��·���޸�Ϊ��");
	adj=StandardizedAdj(map);
	/* ���ڴ����޸� */
	map.arcs[index1][index2].distance=map.arcs[index2][index1].distance=adj;
	/* ͬ���� Edges.txt �ļ��� */
	RewriteEFile(map);
	printf("---\n");
	printf("��·�̣�m�����󣨵�1�к͵�1�б�ʾ�����ţ�0��ʾ��·����\n");
	DisplayAdjMatrix(map);
} // EditDistance

void EditEinfo(Map &map) { // �༭·��Ϣ����ͨ���ߣ�
	/*
	 * edit[]��ȫ�ֳ�����ֵΪ"�༭"
	 * einfo[]��ȫ�ֱ�����·��Ϣ
	 */
	printf("---\n");
	printf("����������Ҫ�༭��·��������������㣺\n");
	FindTwoIndexByIdOrName(map,edit);
	printf("��·��Ϣ�޸�Ϊ��");
	scanf("%s",einfo);
	/* ���ڴ����޸� */
	strcpy(map.arcs[index1][index2].einfo,einfo);
	strcpy(map.arcs[index2][index1].einfo,einfo);
	/* ͬ���� Edges.txt �ļ��� */
	RewriteEFile(map);
	printf("---\n");
	printf("��·�ε�·��Ϣ����Ϊ��%s\n",map.arcs[index1][index2].einfo);
} // EditEinfo

void EditWay(Map &map) { // �༭·�������Ϣ
	printf("---\n");
	printf("��(1)�༭·��\n��(2)�༭·��Ϣ����ͨ���ߣ�\n��(3)�˳��༭·����\n");
	printf("��ѡ��");
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
			printf("�ɹ��˳��༭·���ܣ�����\n");
			break;
	}
} // EditWay

void Edit(Map &map) { // ���� 7���༭���㡢·�����Ϣ
	/*
	 * warnz[]��ȫ�ֳ�����ֵΪ"��Чѡ��"
	 * z��ȫ�ֱ������û�����Ĺ��ܱ��
	 */
	printf("---\n��(1)�༭����\n��(2)�༭·\n��(3)�˳��༭����\n");
	printf("��ѡ��");
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
			printf("�ɹ��˳��༭���ܣ�����\n");
			break;
	}
} // Edit

void AddAttraction(Map &map) { // ��Ӿ���
	/*
	 * vfilename��ȫ�ֳ����������ļ���
	 * efilename��ȫ�ֳ�����·�ļ���
	 * add[]��ȫ�ֳ�����ֵΪ"���"
	 * index1��index2��ȫ�ֱ����������±�
	 * id1��ȫ�ֱ�����������
	 * name[]��ȫ�ֱ�������������
	 * vinfo[]��ȫ�ֱ�����������Ϣ
	 * einfo[]��ȫ�ֱ�����·��Ϣ
	 *
	 * n����Ҫ��ӵľ������
	 * addmaxn������ӵ���󾰵����
	 * m����Ҫ��ӵ�������Ӿ�����ص�·����
	 * addmaxm������ӵ����·����
	 * warnn[]������ n ����ʱ����ʾ��Ϣ
	 * warnm[]������ m ����ʱ����ʾ��Ϣ
	 */
	printf("---\n");
	printf("��(1)ȷ��ʹ����Ӿ��㹦��\n��(2)�˳���Ӿ��㹦��\n");
	printf("��ѡ��");
	z=StandardizedT(map,z,2,warnz);
	if(z==1) {
		int i,j,k,n,m,addmaxn=MAX_VERTEX_NUM-1-map.vexnum,addmaxm;
		char warnn[]="����Ӿ�����������涨��Χ��",warnm[]="�����·�ĸ��������涨��Χ��";
		printf("---\n");
		printf("������Ӿ��������Ϣ����������Ҫ��Ӽ���(1<=n<=%d)���㣺",addmaxn);
		n=StandardizedT(map,n,addmaxn,warnn);
		for(i=1; i<=n; ++i) {
			addmaxm=((n+map.vexnum)*(n+map.vexnum+1))/2-map.arcnum;
			printf("---\n");
			printf("�������%d���¾���ľ������ƣ�",i);
			do {
				scanf("%s",name);
				index1=GetIndexByName(map,name);
				if(index1!=-1) printf("���㡰%s���Ѵ��ڣ����������룺",name);
			} while(index1!=-1);
			printf("��������뾰���ţ�");
			do {
				scanf("%d",&id1);
				index1=GetIndexById(map,id1);
				if(index1!=-1) printf("������ %d �Ѵ��ڣ����������룺",id1);
			} while(index1!=-1);
			printf("��������뾰����Ϣ������ޡ�����");
			scanf("%s",vinfo);
			/* ���ڴ�����Ӿ��� */
			map.vexnum++;
			strcpy(map.vexs[map.vexnum].name,name);
			map.vexs[map.vexnum].id=id1;
			strcpy(map.vexs[map.vexnum].vinfo,vinfo);
			/* ͬ���� Vertexs.txt �ļ��� */
			RewriteVFile(map);
			printf("��%d���¾���������Ϣ����ӳɹ���\n",i);
			printf("\n");
			index1=GetIndexById(map,id1);
			for(k=1; k<=map.vexnum; ++k) { // ���¾�������������֮���·�̳�ʼ��
				map.arcs[index1][k].distance=map.arcs[k][index1].distance=INFINITY;
			}
			printf("�������·�����Ϣ����������Ҫ��Ӽ���(1<=m<=%d)��֮��ص�·��",addmaxm);
			m=StandardizedT(map,m,addmaxn,warnm);
			for(j=1; j<=m; ++j) {
				printf("���������%d��·��صľ��㣺\n",j);
				index2=FindOneIndexByIdOrName(map,add);
				printf("���������·��(>0)��");
				adj=StandardizedAdj(map);
				printf("����������·�εĽ�ͨ���ߣ��޽�ͨ��������С�����");
				scanf("%s",einfo);
				/* ���ڴ������· */
				map.arcnum++;
				map.arcs[index1][index2].distance=map.arcs[index2][index1].distance=adj;
				strcpy(map.arcs[index1][index2].einfo,einfo);
				strcpy(map.arcs[index2][index1].einfo,einfo);
				map.arcs[0][index1].distance=map.arcs[index1][0].distance=id1;
				/* ͬ���� Edges.txt �ļ��� */
				RewriteEFile(map);
				printf("��%d���¾���ĵ�%d��·����ӳɹ���\n",i,j);
				printf("\n");
			} // end for j
			printf("��%d���¾��㼰����ص�·����ӳɹ���\n",i);
		} // end for i
		printf("---\n");
		printf("%d���¾��㼰����ص�·������ӳɹ�������\n",n);
		printf("---\n");
		printf("�µ�ͼ��");
		DisplayMap(map);
	} // end if z
	else {
		printf("---\n");
		printf("�ɹ��˳���Ӿ��㹦�ܣ�����\n");
	}
} // AddAttraction

void AddWay(Map &map) { // ���·
	printf("---\n");
	printf("��(1)ȷ��ʹ�����·����\n��(2)�˳����·����\n");
	printf("��ѡ��");
	z=StandardizedT(map,z,2,warnz);
	if(z==1) {
		int i,m,addmaxm=(map.vexnum*(map.vexnum+1))/2-map.arcnum;
		char warnm[]="�����·�ĸ��������涨��Χ��";
		printf("---\n");
		printf("��������Ҫ��Ӽ���(1<=m<=%d)·��",addmaxm);
		m=StandardizedT(map,m,addmaxm,warnm);
		for(i=1; i<=m; ++i) {
			printf("---\n");
			printf("����������Ҫ��ӵĵ�%d��·��������������㣺\n",i);
			do {
				FindTwoIndexByIdOrName(map,add);
				if(map.arcs[index1][index2].distance!=INFINITY) printf("����������֮����·�����������룺\n");
			} while(map.arcs[index1][index2].distance!=INFINITY);
			printf("���������·��(>0)��");
			adj=StandardizedAdj(map);
			printf("����������·�εĽ�ͨ���ߣ��޽�ͨ��������С�����");
			scanf("%s",einfo);
			/* ���ڴ������· */
			map.arcnum++;
			map.arcs[index1][index2].distance=map.arcs[index2][index1].distance=adj;
			strcpy(map.arcs[index1][index2].einfo,einfo);
			strcpy(map.arcs[index2][index1].einfo,einfo);
			/* ͬ���� Edges.txt �ļ��� */
			RewriteEFile(map);
			printf("��%d��·����ӳɹ���\n",i);
		} // end for i
		printf("---\n");
		printf("%d��·������ӳɹ�������\n",m);
		printf("---\n");
		printf("��·�̣�m�����󣨵�1�к͵�1�б�ʾ�����ţ�0��ʾ��·����\n");
		DisplayAdjMatrix(map);
	} // end if z
	else {
		printf("---\n");
		printf("�ɹ��˳����·���ܣ�����\n");
	}
} // AddWay

void Add(Map &map) { // ���� 8����Ӿ��㡢·�����Ϣ
	/*
	 * warnz[]��ȫ�ֳ�����ֵΪ"��Чѡ��"
	 * z��ȫ�ֱ������û�����Ĺ��ܱ��
	 *
	 * arcmax�����������޵�·���������ֵ
	 */
	int arcmax=(map.vexnum*(map.vexnum+1))/2;
	printf("\n��(1)��Ӿ���\n��(2)���·\n��(3)�˳���ӹ���\n");
	printf("��ѡ��");
	z=StandardizedT(map,z,3,warnz);
	switch(z) {
		case 1:
			if(map.vexnum>=MAX_VERTEX_NUM) printf("�����������������޷�����Ӿ��㣡\n");
			else AddAttraction(map);
			break;
		case 2:
			if(map.arcnum>=arcmax) printf("������������֮���ѻ���ֱ�ӿɴ���������·��\n");
			else AddWay(map);
			break;
		case 3:
			printf("---\n");
			printf("�ɹ��˳���ӹ��ܣ�����\n");
			break;
	}
} // Add

void DeleteAttraction(Map &map) { // ɾ������
	/*
	 * vfilename��ȫ�ֳ����������ļ���
	 * efilename��ȫ�ֳ�����·�ļ���
	 * erase[]��ȫ�ֳ�����ֵΪ"ɾ��"
	 * index��ȫ�ֱ����������±�
	 *
	 * n����Ҫɾ���ľ������
	 * deletemaxn����ɾ������󾰵����
	 * m��������������·����
	 * deletem������Ҫɾ���ľ�����ص�·����
	 * warnn[]������ n ����ʱ����ʾ��Ϣ
	 */
	printf("---\n");
	printf("��(1)ȷ��ʹ��ɾ�����㹦��\n��(2)�˳�ɾ�����㹦��\n");
	printf("��ѡ��");
	z=StandardizedT(map,z,2,warnz);
	if(z==1) {
		int i,j,k,n,m,deletemaxn=map.vexnum-1,deletem;
		char warnn[]="��ɾ���ľ�����������涨��Χ��";
		printf("---\n");
		printf("��������Ҫɾ������(1<=n<=%d)���㣺",deletemaxn);
		n=StandardizedT(map,n,deletemaxn,warnn);
		for(i=1; i<=n; ++i) {
			deletem=0;
			printf("---\n");
			printf("��������Ҫɾ���ĵ�%d�����㣺\n",i);
			index=FindOneIndexByIdOrName(map,erase);
			m=map.arcnum;
			for(j=1; j<=map.vexnum; ++j) {
				if(map.arcs[index][j].distance!=INFINITY) {
					m--;
					deletem++;
				}
			}
			if(m>=map.vexnum-1) {
				/* ���ڴ���ɾ�����㼰����ص�· */
				map.arcnum-=deletem; // ����·��
				for(j=0; j<=map.vexnum; ++j) {
					for(k=index; k<map.vexnum; ++k) map.arcs[j][k]=map.arcs[j][k+1]; // �ƶ���ɾ������֮��ľ���Ԫ��
				}
				for(j=0; j<=map.vexnum; ++j) {
					for(k=index; k<map.vexnum; ++k) map.arcs[k][j]=map.arcs[k+1][j]; // �ƶ���ɾ������֮�µľ���Ԫ��
				}
				printf("��%d�����㡰%s���ѳɹ�ɾ����\n",i,map.vexs[index].name);
				for(j=index; j<map.vexnum; ++j) map.vexs[j]=map.vexs[j+1];
				map.vexnum--; // ���¶�����
				/* ͬ���� Vertexs.txt �ļ��� */
				RewriteVFile(map);
				/* ͬ���� Edges.txt �ļ��� */
				RewriteEFile(map);
			} // end if m
			else printf("ɾ�����㡰%s���ᵼ�������������㲻��ͨ����ֹɾ���þ���\n",map.vexs[index].name);
		} // end for i
		printf("---\n");
		printf("%d��������ѳɹ�ɾ��������\n",n);
		printf("---\n");
		printf("�µ�ͼ��");
		DisplayMap(map);
	} // end if z
	else {
		printf("---\n");
		printf("�ɹ��˳�ɾ�����㹦�ܣ�����\n");
	}
} // DeleteAttraction

void DeleteWay(Map &map) { // ɾ��·
	/*
	 * index1��index2��ȫ�ֱ����������±�
	 *
	 * warnm[]������ m ����ʱ����ʾ��Ϣ
	 */
	printf("---\n");
	printf("��(1)ȷ��ʹ��ɾ��·����\n��(2)�˳�ɾ��·����\n");
	printf("��ѡ��");
	z=StandardizedT(map,z,2,warnz);
	if(z==1) {
		int i,j,k,m,deletemaxm=map.arcnum-map.vexnum+1;
		char warnm[]="��ɾ��·�ĸ��������涨��Χ��";
		printf("---\n");
		printf("��������Ҫɾ������(1<=m<=%d)·��",deletemaxm);
		m=StandardizedT(map,m,deletemaxm,warnm);
		for(i=1; i<=m; ++i) {
			printf("---\n");
			printf("����������Ҫɾ���ĵ�%d��·��������������㣺\n",i);
			FindTwoIndexByIdOrName(map,erase);
			/* ���ڴ���ɾ��· */
			map.arcs[index1][index2].distance=map.arcs[index2][index1].distance=INFINITY;
			strcpy(map.arcs[index1][index2].einfo,"");
			strcpy(map.arcs[index2][index1].einfo,"");
			map.arcnum--;
			/* ͬ���� Edges.txt �ļ��� */
			RewriteEFile(map);
			printf("��%d��·�ѳɹ�ɾ����\n",i);
		}// end for i
		printf("---\n");
		printf("%d��·���ѳɹ�ɾ��������\n",m);
		printf("---\n");
		printf("��·�̣�m�����󣨵�1�к͵�1�б�ʾ�����ţ�0��ʾ��·����\n");
		DisplayAdjMatrix(map);
	} // end if z
	else {
		printf("---\n");
		printf("�ɹ��˳�ɾ��·���ܣ�����\n");
	}
} // DeleteWay

void Delete(Map &map) { // ���� 9��ɾ�����㡢·�����Ϣ
	/*
	 * warnz[]��ȫ�ֳ�����ֵΪ"��Чѡ��"
	 * z��ȫ�ֱ������û�����Ĺ��ܱ��
	 */
	printf("\n��(1)ɾ������\n��(2)ɾ��·\n��(3)�˳�ɾ������\n");
	printf("��ѡ��");
	z=StandardizedT(map,z,3,warnz);
	switch(z) {
		case 1:
			if(map.vexnum==1) printf("����������1�����㣬��ֹɾ�����һ�����㣡\n");
			else DeleteAttraction(map);
			break;
		case 2:
			if(map.arcnum==map.vexnum-1) printf("����ɾ��·�ᵼ������������ͨ����ֹ����ɾ��·��\n");
			else DeleteWay(map);
			break;
		case 3:
			printf("---\n");
			printf("�ɹ��˳�ɾ�����ܣ�\n");
			break;
	}
} // Delete
