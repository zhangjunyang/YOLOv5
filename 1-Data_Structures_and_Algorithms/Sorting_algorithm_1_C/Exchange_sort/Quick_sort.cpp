// 快速排序算法的基本思想是：将所要进行排序的数分为左右两个部分，其中一部分的所有数据都比另外一 部分的数据小，然后将所分得的两部分数据进行同样的划分，重复执行以上的划分操作，直 到所有要进行排序的数据变为有序为止。
#include <stdio.h>
#include <stdlib.h>
#define N 6

int partition(int arr[], int low, int high){
    int key;
    key = arr[low];
    while(low<high){
        while(low <high && arr[high]>= key )
            high--;
        if(low<high)
            arr[low++] = arr[high];
        while( low<high && arr[low]<=key )
            low++;
        if(low<high)
            arr[high--] = arr[low];
    }
    arr[low] = key;

    return low;
}

void quick_sort(int arr[], int start, int end){
    int pos;
    if (start<end){
        pos = partition(arr, start, end);
        quick_sort(arr,start,pos-1);
        quick_sort(arr,pos+1,end);
    }

    return;
}

int main(void){
    int i;
    int arr[N]={32,12,7, 78, 23,45};

    printf("排序前 \n");
    for(i=0;i<N;i++)
        printf("%d\t",arr[i]);
    quick_sort(arr,0,N-1);

    printf("\n 排序后 \n");
    for(i=0; i<N; i++)
        printf("%d\t", arr[i]);
    printf ("\n");

    system("pause");
    return 0;
}