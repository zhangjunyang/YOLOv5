// 顺序査找是一种简单的査找算法，其实现方法是从序列的起始元素开始，逐个将序列中的元素与所要查找的元素进行比较，如果序列中有元素与所要查找的元素相等，那么査找成功，如果査找到序列的最后一个元素都不存在一个元素与所要査找的元素值相等，那么表明査找失败。
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

int ordersearch(int a[], int n, int des){
    int i;
    for(i=0; i<n; i++)
        if(des==a[i])
            return 1;

    return 0;
}

int main(){
    int i, val;
    int a[8] = {32,12,56,78,76,45,43,98};
    int ret;

    for(i=0; i<8; i++)
        printf("%d\t", a[i]);
   
    printf("\n请输入所要查找的元素：");
    while(1){
        scanf("%d", &val);
        fflush(stdin);
        ret = ordersearch(a, 8, val);

        if(1 == ret)
            printf ("查找成功！");
        else
            printf ("查找失败！");

        printf("\n请输入所要查找的元素：");
    }

    return 0;
}