#include"Head.h"
#include"Operates.cpp"

int main() {
	int x;
	char title1[]="����������ͼ��",title2[]="����һ���㵽�������о����������̼�·����·�̡�",
		 title3[]="�����������������м�·����·�̡�",title4[]="��������������������̼�·����·�̡�",
		 title5[]="���������·�߼�·�̡�",title6[]="����ѯ���㼰���ھ��������Ϣ��",title7[]="���༭��",title8[]="����ӡ�",
		 title9[]="��ɾ����";
	Map map;
	CreatMap(map);
	while(1) {
		RenderMenu();
		MadeMenuCenter();
		printf("��ѡ���ܱ�ţ�");
		do {
			scanf("%d",&x);
			if(x<0||x>9) {
				MadeMenuCenter();
				printf("�������ܱ�ŷ�Χ�����������룺");
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
				printf("�ɹ��˳����������㵼��ϵͳ������\n"); 
				return 0;
		}
	}
}
