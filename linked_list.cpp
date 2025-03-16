/*
Actions on LinkedList
单链表
用template模板的时候，List类的first需要Node<T>*,因为Node只是一个抽象的类模板，需要先实例化为类型定下来的模板类
first虽然是一个哑元，但它初始化的时候赋值不能是nullptr，因为后续插入需要用到first的next，如果是空指针调用会出错
插入和删除的时候应该迭代到pos的前一个，用1开始计数的话就是p=1，p<pos
ppt中还用一个成员变量current_pos来记录当前curr所在位置,一个Node* 的curr来记录当前节点
*/
#include<iostream>
using namespace std;
template<class T>
class Node
{
public:
    T data;
    Node * next;
    Node(T n): data(n),next(NULL){}
};
template<class T>
class List
{
public:
    Node<T>* first;
    int count;
    List():first(new Node<T>(0)), count(0) {}
    int size();
    bool empty() 
    {
        return first->next == NULL;
    }
    void insert(int pos,const T &x);//1
    void findrepos(int pos);//2
    void reverse();//3
    void findmid();//4
    void del(int pos);
    void print();
};
template<class T>
void List<T>::findrepos(int pos)
{
    Node<T>* slow = first;
    Node<T>* fast = first;
    for(int i=1 ; i <= pos ; i++)
        fast = fast -> next;
    while(fast)
    {
        fast = fast -> next;
        slow = slow -> next;
    }
    cout<<slow -> data<<endl;
}
template<class T>
void List<T>::findmid()
{
    Node<T>* slow = first;
    Node<T>* fast = first;
    while(fast)
    {
        fast = fast -> next;
        if(!fast)
        {
            cout<<slow -> data<<endl;
            return;
        }
        fast = fast -> next;
        slow = slow -> next;
    }
    cout<<slow -> data<<endl;
}
template<class T>
void List<T>::print()
{
    Node<T>* curr = first;
    while(curr->next)
    {
        curr = curr->next;
        cout<<curr->data<<" ";
    }
}
template<class T>
int List<T>::size()
{
    return count;
}
template<class T>
void List<T>::reverse()
{
    T* temp = new T[count];
    Node<T>* curr = first;
    int j=0;
    while(curr->next)
    {
        curr = curr->next;
        temp[count-1-j] = curr->data;
        j++;
    }
    j=0;
    curr = first;
    while(curr->next)
    {
        curr = curr->next;
        curr->data = temp[j];
        j++;
    }
    delete[] temp;
}
template<class T>
void List<T>::insert(int pos,const T &x)
{
    Node<T>* curr=first;    
    for(int p=1 ; p<pos; p++)
    {
        curr=curr->next;
    }
    Node<T>* newnode = new Node<T>(x);
    newnode->next = curr->next;
    curr->next = newnode;
    count++;
}
template<class T>
void List<T>::del(int pos )
{
    Node<T>* curr=first;
    for(int p = 1; p < pos ; p++)
    {
        curr = curr->next;
    }
    Node<T>* temp = curr->next;
    curr->next = curr->next->next;
    delete temp;
    /*
        return temp;
    */
    count--;
}

int main()
{
    List<int> l;
    l.insert(1,1);
    l.insert(2,2);
    l.insert(3,4);
    l.insert(4,5);
    l.insert(5,6);
    l.insert(6,7);
    l.insert(7,9);
    l.print();
    cout<<endl;
    l.insert(4,33);
    l.print();
    cout<<endl;
    l.del(2);
    l.print();
    cout<<endl;
    l.reverse();
    l.print();
    cout<<endl;
    cout<<l.size()<<endl;
    l.findrepos(3);
    l.findmid();
    return 0;
}