#include <stdio.h>
#include<string.h>

int main() {
    // 定义一个字符串
    char my_string[10] = "123456789a";
    // char *p;
    // p= my_string;
    // 遍历字符串中的每个字符
    for (int i = 0; my_string[i]; i++)
    {
        // 检查字符是否为'\0'
        printf("第%d个：",i);
        printf("%p\n",&my_string[i]);
        // p=my_string+1;
    }
    // printf("第十个%d\n",&my_string[13]);
    // printf("\n");
    // printf("%d\n",my_string[6]);
    printf("长度为%d\n",strlen(my_string));
    // 如果循环结束仍未找到'\0'，表示不存在
    // if (my_string[i] != '\0') {
    //     printf("字符串中不存在\\0\n");
    // }
    int a = 100;
    printf("%d",a);

    return 0;
}
