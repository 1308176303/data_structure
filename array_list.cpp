#include<bits/stdc++.h>
#include<stdexcept>
using namespace std;
template<class T>
class arraylist
{
    protected:
        T *element;
        int arraylength; //最大存储量
        int listsize;//链表长度
        int checkindex(int index) const;//若索引无效，则返回-1，否则返回1
    public:
        //构造函数、析构函数
        arraylist(int initialcapacity = 10);//构造函数
        arraylist(const arraylist<T>&); //拷贝构造函数
        //~arraylist();

        //ADT方法（abstract data type)
        /*
            const---常量成员函数，不能修改类的任何成员变量，除了用mutable修饰的变量
        */
        bool empty() const {return listsize == 0;}
        int size() const {return listsize;}
        T& get(int index) const;//返回对应下标的元素值
        int indexof(const T& element) const;//如果没找到，就返回-1
        void erase(int index) ;//删除对应索引上的元素
        void insert(int index,const T& element);
        void output(ostream& out) const;
        int capacity() const {return arraylength;}
};
template <class T>
int arraylist<T>::checkindex(int index) const
{
    if(index > arraylength-1 ||index < 0) return -1;
    return 1;
}
template <class T>
arraylist<T>::arraylist(int initialcapacity)
{   
    if(initialcapacity < 1)
    {
        ostringstream s;
        cout<<"initialcapacity = "<< initialcapacity <<" must be >0 \n";
        return;
    }
    arraylength = initialcapacity; //1.修改maxsize
    element = new T[arraylength];//2.给element分配内存
    listsize = 0;//3.设置链表长度我为0
}
template <class T>
arraylist<T>::arraylist(const arraylist<T> & other)
{
    arraylength = other.arraylength;
    listsize = other.listsize;
    element = new T[arraylength];
    copy(other.element,other.element+listsize,element);
}
template <class T>
T &arraylist<T>::get(int index) const
{
    // if(check(index) == -1)
    // {
    //     cout<<"invailed index!\n";
    //     return;
    // }
    return element[index];
}
template <class T>
int arraylist<T>::indexof(const T &theElement) const
{
    /*int res = (int)(find(element,element+listsize,theElement)-element);
        要是res=listsize就说明到了最后也没匹配上，就没找到
        if(res == listsize) return -1;
        return res;
    */
    for(int i=0; i < arraylength ; i++)
    {
        if(element[i] == theElement) return i;
    }
    return -1;
}
template <class T>
void arraylist<T>::erase(int index)
{
    int i = index;
    for(i;i < listsize;i++)
        element[i] = element[i+1];
    listsize--;
}
template <class T>
void arraylist<T>::insert(int index, const T &theElement)
{
    if(index > arraylength || index < 0)
    {
        cout<<"invalid index!\n";
        return;
    }
    //要是满了就倍增数组
    if(listsize == arraylength)
    {
        arraylength *= 2;
        T *newarray = new T[arraylength];
        copy(element,element+listsize,newarray);
        delete[] element;
        element = newarray;
    }
    listsize++;
    for(int i = listsize-1;i > index;i--)
    {
        element[i+1] = element[i];
    }
    element[index] = theElement;
}
int main()
{
    return 0;
}