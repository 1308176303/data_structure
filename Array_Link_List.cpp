//这里的头节点是哑元
#include<iostream>
using namespace std;
const int MAX_ARRAY = 100;
class Node
{
public:
    int data, next;
};
class Array_List
{
public:
    Node arr[MAX_ARRAY];
    int freelist = -1 , lastuse = -1, head = 0;
    int size = 0;
    int insert(int pos, int val);
    int new_node();//建立一个新节点，不加入链表中，等于重写的new方法
    int curr_pos(int pos);//该位置是链表里第几个节点
    int setpos(int pos);
    void del(int delt_index);
    bool empty();
    bool full();
    void print();
};

int main()
{
    // string s = "";
    // s += 'a';
    // cout<<s;
    return 0;
}
int Array_List::insert(int pos, int val)
{
    if(pos <= 0 || pos >= MAX_ARRAY ) return -1;
    int newnode = new_node();
    if(newnode == -1) return -1;
    arr[newnode].data = val;
    // 作为新的头节点插入
    if( pos == 1 )
    {
            arr[newnode].next = arr[head].next;
            arr[head].next = newnode;
    }
    else
    {
        int curr = setpos(pos-1);
        arr[newnode].next = arr[curr].next;
        arr[curr].next = newnode;
    }
    size++;
    return 0;
}
// 建立一个新节点，先不把它加入到链表里
int Array_List::new_node()
{
    int newnode;
    //freelist里有节点就优先用freelist里的
    if(freelist != -1)
    {
        newnode = freelist;
        freelist = arr[freelist].next;
    }
    else
    {
        lastuse++;
        newnode = lastuse;
    }
    arr[newnode].next = -1;
    return newnode;
}

int Array_List::curr_pos(int index)
{
    if(index < 0 || index > lastuse)
        return -1;
    int curr = head;
    int sum = 0;
    while(curr != index)
    {
        sum ++;
        curr = arr[curr].next; 
    }
    if(sum != 0)
        return sum;
    return -1;//如果最终也没遍历到，就是在freelist里
}
//pos是在链表里的位置
int Array_List::setpos(int pos)
{
    if(pos < 0 || pos > size) return -1;
    int curr = head;
    for(int i = 1; i <= pos ;i++ )
        curr = arr[curr].next;
    return curr;
}
//delt_index 是在数组的下标
void Array_List::del(int delt_index)
{
    if(delt_index < 0 || delt_index >= lastuse ) return;
    if(delt_index == arr[head].next)
    {
        arr[head].next = arr[arr[head].next].next;
    }
    else
    {
        int curr = head;
        while(arr[curr].next != delt_index)
            curr = arr[curr].next;
        arr[curr].next = arr[delt_index].next;
    }
}

bool Array_List::empty()
{
    return size == 0;
}

bool Array_List::full()
{
    return size == MAX_ARRAY;
}

void Array_List::print()
{
    int curr = head;
    while(curr != -1)
    {
        curr = arr[curr].next;
        cout<<arr[curr].data<<" ";
    }
    cout<<endl;
}
