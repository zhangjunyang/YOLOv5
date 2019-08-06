// 就像是把一条链子先一分为二，然后用一个环节再把它们连接起来
void insert(node * &head,char keyWord,char newdata)//keyWord是查找关键字符
{
   node *newnode=new node;//新建结点
   newnode->data=newdata;//newdata是新结点的数据
   node *pGuard=search(head,keyWord);//pGuard是插入位置前的结点指针
   if (head==NULL || pGuard==NULL)//如果链表没有结点或找不到关键字结点
   {//则插入表头位置
      newnode->next=head;//先连
      head=newnode;//后断
   }
   else//否则
   {//插入在pGuard之后
      newnode->next=pGuard->next;//先连
      pGuard->next=newnode;//后断
   }
}