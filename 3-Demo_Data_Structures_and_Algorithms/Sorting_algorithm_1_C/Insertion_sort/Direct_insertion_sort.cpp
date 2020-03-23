// 插入排序的基本思想是，将元素逐个添加到已经排序好的数组中去，同时要求，插入的元素必须在正确的位置，这样原来排序好的数组是仍然有序的。
#include<stdio.h>
#include<stdlib.h>

#define N 8

void insert_sort(int a[],int n);


//插入排序实现,这里按从小到大排序
void insert_sort(int a[],int n)//n为数组a的元素个数
{
    //进行N-1轮插入过程
    for(int i=1; i<n; i++)
    {
        //首先找到元素a[i]需要插入的位置
        int j=0;
        while( (a[j]<a[i]) && (j<i))
        {
            j++;
        }

        //将元素插入到正确的位置
        if(i != j)  //如果i==j，说明a[i]刚好在正确的位置
        {
            int temp = a[i];
            for(int k = i; k > j; k--)
            {
                a[k] = a[k-1];
            }
            a[j] = temp;
        }
    }
}


int  main()
{
    int num[N] = {89, 38, 11, 78, 96, 44, 19, 25};

    insert_sort(num, N);

    for(int i=0; i<N; i++)
        printf("%d  ", num[i]);

    printf("\n");


    system("pause");
    return 0;
}