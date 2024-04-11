#include"Head.h"
#include"Operates.cpp"

int main() {
	int x;
	char title1[]="【旅游区地图】",title2[]="【任一景点到其它所有景点的所有最短简单路径及路程】",
		 title3[]="【任意两景点间的所有简单路径及路程】",title4[]="【任意两景点间的所有最短简单路径及路程】",
		 title5[]="【最佳游览路线及路程】",title6[]="【查询景点及相邻景点相关信息】",title7[]="【编辑】",title8[]="【添加】",
		 title9[]="【删除】";
	Map map;
	CreatMap(map);
	while(1) {
		RenderMenu();
		MadeMenuCenter();
		printf("请选择功能编号：");
		do {
			scanf("%d",&x);
			if(x<0||x>9) {
				MadeMenuCenter();
				printf("超出功能编号范围，请重新输入：");
			}
		} while(x<0||x>9);
		switch(x) {
			case 1:
				system("cls"); 
				RenderFunctionTop(title1);
				DisplayMap(map);
				RenderFunctionBottom();
				system("pause");
				system("cls");
				break;
			case 2:
				system("cls");
				RenderFunctionTop(title2);
				AllShortestPathByOne(map);
				RenderFunctionBottom();
				system("pause");
				system("cls");
				break;
			case 3:
				system("cls");
				RenderFunctionTop(title3);
				AllSimplePathByTwo(map);
				RenderFunctionBottom();
				system("pause");
				system("cls");
				break;
			case 4:
				system("cls");
				RenderFunctionTop(title4);
				AllShortestPathByTwo(map);
				RenderFunctionBottom();
				system("pause");
				system("cls");
				break;
			case 5:
				system("cls");
				RenderFunctionTop(title5);
				BestVisitPath(map); 
				RenderFunctionBottom();
				system("pause");
				system("cls");
				break;
			case 6:
				system("cls");
				RenderFunctionTop(title6);
				AttractionInformation(map);
				RenderFunctionBottom();
				system("pause");
				system("cls");
				break;
			case 7:
				system("cls");
				RenderFunctionTop(title7);
				Edit(map);
				RenderFunctionBottom();
				system("pause");
				system("cls");
				break; 
			case 8:
				system("cls");
				RenderFunctionTop(title8);
				Add(map);
				RenderFunctionBottom();
				system("pause");
				system("cls");
				break;
			case 9:
				system("cls");
				RenderFunctionTop(title9);
				Delete(map);
				RenderFunctionBottom();
				system("pause");
				system("cls");
				break; 
			case 0:
				MadeMenuCenter();
				printf("成功退出旅游区景点导游系统！！！\n"); 
				return 0;
		}
	}
}
