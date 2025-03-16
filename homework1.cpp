#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<algorithm>
#include<iomanip>
using namespace std;

class Node
{
    public:
        int col;//从第几个字母开始
        int row;//第几行
        Node* next;
        Node ():row(-1),col(-1),next(NULL){}
        Node(int x,int y):row(x),col(y),next(NULL){}
};


class arrayNode
{
public:
    string str;
    Node* pos;//在文章中出现顺序的线性表

    /*理解为dirNext和rateIndex，定义时英文写错了，全改工程太大
        全按照升序序列来排
    */
    int dirIndex;//字典序索引
    int rateIndex;//频次索引


    int count;//出现次数

    arrayNode():str(" "),dirIndex(-1),rateIndex(-1),count(0){ pos = new Node();}
};

//head指向第一个元素，不是哑元
class arrayList
{
public:
    //因为全是插入链表，所以不需要del和freelist
    arrayNode *arr;
    int ratehead,dirhead,tail;
    int maxsize;

    arrayList():ratehead(-1),dirhead(-1),tail(-1),maxsize(100){arr = new arrayNode[100];}

    void insert(string s,int row,int col);

    void print()
    {
        for(int i = 0 ; i < maxsize ; i++)
        {
            cout<<left<<setw(10)<<arr[i].str<<" count:"<<arr[i].count<<" rateIndex:"<<arr[i].rateIndex
            <<" dirIndex:"<<arr[i].dirIndex<<" ";
            Node *curr = arr[i].pos;

            int sum = 1;
            while(curr->next)
            {
                curr = curr->next;
                cout<<"The "<<sum<<" time: ";
                cout<<"row: "<<curr->row<<" col: "<<curr->col<<endl;
                sum++;
            }
        }
    }
};

arrayList solution(string *source,int n)
{
    arrayList arr;
    string temp ="";
    
    for(int i = 0;i < n;i++)
    {
        int index = 0;
        while(source[i][index] == ' ') index++;//跳过行首空格,如果没有这行代码不执行
        if(index > source[i].length() || source[i][index] == '\n') continue;//说明这一整行都是空，直接跳到下一轮循环

        //先记录第一个字母
        temp += source[i][index];
        index++;
        
        while(index < source[i].length())
        {
            if(source[i][index] == ' ' ||source[i][index] =='.'||source[i][index] =='?'||source[i][index] =='!'||source[i][index] ==',' ) //一个单词结束
            {
                if(temp != "")
                {
                    arr.insert(temp,i,index);
                    temp = "";
                }
            }
            else
            {
                temp += source[i][index];
            } 
            index++;
        }
    }
    return arr;
}

int main()
{
    ifstream file("test.txt");
    if (!file.is_open()) 
    {
        cerr << "Unable to open file: test.txt" << endl;
        return 1;
    }
    //cout<<" open file !"<<endl;
    string line;
    vector<string> source;
    while(getline(file,line))
    {   
        source.push_back(line);
    }
    // cout<<"finished push_back\n";
    // for (const auto& l : source) {
    //     cout << l << endl;
    // }

    arrayList arr = solution(source.data(),source.size());
    
    arr.print();
    file.close();
    return 0;
}

void arrayList::insert(string s,int row, int col)
{
    //先将所有单词全转化为全小写字母的形式
    transform(s.begin(), s.end(), s.begin(), ::tolower);

    //处理刚开始录入文章，数组为空的情况
    if(ratehead == dirhead && ratehead == tail && tail == -1)
    {
        tail++;
        arr[tail].str = s;
        arr[tail].pos->next = new Node(row,col);
        arr[tail].count = 1;

        ratehead = tail;
        dirhead = tail;
        return;
    }

    /*
    先检查出没出现过
    如果出现过就频率次加一然后重排频率序，结束函数
    不必更新字典序，因为重复出现只会涉及到频率的改变
    */
    int i = ratehead,prev=ratehead;
    while(i!=-1)
    {
        if(s == arr[i].str)
        {
            //更新这个单词的出现顺序的线性表
            Node *newno = new Node(row,col);
            //pos视作哑元头节点
            Node *curr = arr[i].pos;
            while(curr->next)
                curr = curr->next;
            curr->next = newno;

            arr[i].count++;
            /*(next在这里就是curr)
                prev->i->i.rateindex->......->prev1->curr->curr.rateindex
                prev1-> (next)    ->
            */
            int next = arr[i].rateIndex;
            int change = arr[i].rateIndex; //记录i的下一个，把prev连到它前面
            int prev1 = i;
            while(arr[next].count < arr[i].count)
            {
                prev1 = next;
                next = arr[next].rateIndex;
            }
            arr[i].rateIndex = arr[prev1].rateIndex;
            arr[prev1].rateIndex = i;
            arr[prev].rateIndex = change;
            return;
        }   
        prev = i;
        i = arr[i].rateIndex;
    }

    //这个单词在文章中没出现过
    //如果需要扩容
    if(tail == maxsize - 1)
    {
        maxsize *= 2;
        arrayNode *temp = new arrayNode[maxsize];
        copy(arr,arr + tail + 1,temp);
        delete[] arr;
        arr = temp;
    }

    //更新pos和count
    tail++;
    arr[tail].str = s;
    arr[tail].pos->next = new Node(row,col);
    arr[tail].count = 1;

    /*
        更新rateindex
        因为这里这个新出现的单词一定只出现了一次，而链表中所有元素至少一定也出现了一次，
        所以直接连在head前面，然后更新head即可
    */  
        arr[tail].rateIndex = ratehead;
        ratehead = tail;
    

    //更新dirindex
    int curr = dirhead;

    //如果比头节点字典序还小
    if(arr[tail].str < arr[dirhead].str)
    {
        arr[tail].dirIndex = dirhead;
        dirhead = tail;
    }

    else
    {
        prev = curr;
        while(curr!= -1 && arr[curr].str < arr[tail].str )
        {
            prev = curr;
            curr = arr[curr].dirIndex;
        }

        //需要插入到数组尾部
        if(curr == -1)
        {
            arr[prev].dirIndex = tail;
        }
        else
        {
            arr[tail].dirIndex = curr;
            arr[prev].dirIndex = tail;
        }
        
    }

}