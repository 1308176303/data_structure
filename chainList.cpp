#include<iostream>
using namespace std;
template<class T>
struct chainNode
{
    //数据成员
    T element;
    chainNode<T> *next;

    //方法
    chainNode() {}
    chainNode(const T& element)
    {
        this->element = element;
        this->next = NULL;    
    }
    chainNode(const T& element,chainNode<T>* next)
    {
        this->element = element;
        this->next = next;
    }
};

template<class T>
class chain
{
    protected:
        chainNode<T> *firstnode;
        int listsize;
    
    public:
    //构造析构
        chain(int initialCapacity = 10): listsize(0),firstnode(NULL){}
        chain(const chain<T>&);
        ~chain();

    //抽象数据类型的方法
    bool empty() {return listsize == 0;}
    int size () const {return listsize;}
    T& get(int index) const ;
    int indexOf(const T& element) const;
    void erase(int index);
    void insert(int index,T& theElement);
    void output(ostream& ) const;

};

template <class T>
chain<T>::chain(const chain<T> &other)
{
    listsize = other.listsize;   
    if(listsize == 0) 
    {
        firstnode = NULL;
        return;
    }
    chainNode<T> *sourceNode = other.firstnode;
    firstnode = new chainNode<T>(sourceNode->element);
    chainNode<T> *currNode = firstnode;
    sourceNode = sourceNode->next;

    while(sourceNode != NULL)
    {
        currNode->next = new chainNode<T>(sourceNode->element);
        currNode = currNode->next;
        sourceNode = sourceNode->next;
    }
}

template <class T>
chain<T>::~chain()
{
    while(firstnode)
    {
        chainNode<T> temp = firstnode;
        firstnode = firstnode->next;
        delete temp;
    }
}

//很明显index是指这个节点在链表里位于第几个
template <class T>
T &chain<T>::get(int index) const
{
    chainNode<T> *curr = firstnode;
    for(int i = 0;i < index ;i++)
        curr = curr->next;
    return curr->element;
}

template <class T>
int chain<T>::indexOf(const T& element) const
{
    int i=0;
    chainNode<T> *curr = firstnode;
    while(curr->element != element && curr)
    {
        curr = curr->next;
        i++;
    }
    return curr ? i : -1; //-1代表没有找到对应节点
}

template <class T>
void chain<T>::erase(int index)
{
    //1.删掉头节点
    if(index == 0)
    {
        chainNode<T> *temp = firstnode;
        firstnode = firstnode->next;
        delete temp;
    }

    //2.删掉其他节点
    chainNode<T> *curr = firstnode;
    for(int i = 0;i < index-1;i++)
        curr = curr->next;
    chainNode<T> *temp = curr->next;
    curr->next = curr->next->next;
    delete temp;
    listsize--;
}

template <class T>
void chain<T>::insert(int index, T &theElement)
{
    chainNode<T> *newNode = new chainNode<T>(theElement);
    if(index == 0)
    {
        newNode->next = firstnode;
        firstnode = newNode;
        //firstnode = new chainNode<T>(theElement,firstnode);
    }
    else
    {
        chainNode<T> *curr = firstnode;
        for(int i = 0;i < index -1 ;i++)
            curr = curr->next;
        newNode->next = curr->next;
        curr->next = newNode;
    }
    listsize++;
}

template <class T>
void chain<T>::output(ostream &s) const
{
    chainNode<T> *curr = firstnode;
    while(curr)
    {
        s<<curr->element<<" ";
        curr = curr->next;
    }
}

template <class T>
ostream& operator <<(ostream& s,const chain<T>& x)
{
    x.output(s);
    return s;
}

int main()
{

    return 0;
}