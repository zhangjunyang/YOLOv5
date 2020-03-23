// 数组为了保持其顺序存储的特性，在删除某个数据时，其后的数据都要依次前移
void Delete(node * &head,char keyWord)//可能要操作表头指针，所以head是引用
{
   if (head!=NULL)//如果链表没有结点，就直接输出提示
   {
      node *p;
      node *pGuard=head;//初始化pGuard指针
      if (head->data==keyWord)//如果头结点数据符合关键字
      {
         p=head;//头结点是待删除结点
        head=head->next;//先连
         delete p;//后断
         cout <<"The deleted node is " <<keyWord <<endl;
         return;//结束函数运行
      }
      else//否则
      {
         while (pGuard->next!=NULL)//当pGuard没有达到表尾
         {
            if (pGuard->next->data==keyWord)//如果pGuard后继结点数据符合关键字
            {
               p=pGuard->next;//pGuard后继结点是待删除结点 
               pGuard->next=p->next;//先连
               delete p;//后断
               cout <<"The deleted node is " <<keyWord <<endl;
               return;//结束函数运行
            }
            pGuard=pGuard->next;//pGuard指针向后移动
          }
      }
   }
   cout <<"The keyword node is not found or the link list is empty!" <<endl;//输出提示信息
}