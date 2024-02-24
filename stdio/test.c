#include<stdio.h>
#include<string.h>
#include<stdlib.h>
int main()
{
	FILE *txt = NULL;
	txt = fopen("tt.txt","r");
	int a = 'C';
	char haha[15];
	// printf("请输入：\n");
	fgets(haha,12,txt);
	
	int length = strlen(haha);
	printf("长度为%d\n",length);
	printf("%d\n",(haha[9]));
	printf("%s\n",haha);
	fgets(haha,12,txt);
	
	
	printf("%s",haha);
	
	printf("%d\n",a);
	return 0;
}
