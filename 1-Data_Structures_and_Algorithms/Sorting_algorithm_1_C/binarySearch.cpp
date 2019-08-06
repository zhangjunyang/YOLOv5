// 二分査找也称折半査找，其优点是查找速度快，缺点是要求所要査找的数据必须是有序序列。该算法的基本思想是将所要査找的序列的中间位置的数据与所要査找的元素进行比较，如果相等，则表示査找成功，否则将以该位置为基准将所要査找的序列分为左右两部分。
#include <stdio.h>
binarySearch(int a[], int n, int key){
    int low = 0;
    int high = n - 1;
    while(low<= high){
        int mid = (low + high)/2;
        int midVal = a[mid];
        if(midVal<key)
            low = mid + 1;
        else if(midVal>key)
            high = mid - 1;
        else
            return mid;
    }
    return -1;
}
int main(){
    int i, val, ret;
    int a[8]={-32, 12, 16, 24, 36, 45, 59, 98};
    for(i=0; i<8; i++)
        printf("%d\t", a[i]);
    printf("\n请输人所要查找的元素：");
    scanf("%d",&val);
    ret = binarySearch(a,8,val);
    if(-1 == ret)
        printf("查找失败 \n");
    else
        printf ("查找成功 \n");
    return 0;
}