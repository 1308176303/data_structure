#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>

using namespace std;  // 引入命名空间

void generateRandomNumbers(const string& filename, int count) {
    // 打开文件以写入
    ofstream outFile(filename);

    // 检查文件是否成功打开
    if (!outFile) {
        cerr << "无法打开文件: " << filename << endl;
        return;
    }

    // 使用当前时间作为随机数种子
    srand(time(0));

    // 生成并写入随机数
    for (int i = 0; i < count; ++i) {
        int randomNum = rand(); // 随机数
        outFile << randomNum << " ";
    }

    // 关闭文件
    outFile.close();
}

void readRandomNumbers(const string& filename, int* numbers, int count) {
    // 打开文件以读取
    ifstream inFile(filename);

    // 检查文件是否成功打开
    if (!inFile) {
        cerr << "无法打开文件: " << filename << endl;
        return;
    }

    // 读取文件中的数字到数组
    for (int i = 0; i < count; ++i) {
        inFile >> numbers[i]; // 将数字存入数组
    }

    // 关闭文件
    inFile.close();
}

void bubbleSort(int* numbers, int count,int &comCount,int &exchCount ) {
    bool flag = false;
    // 进行冒泡排序
    for (int i = 0; i < count - 1; i++) {
        flag = false;
        for (int j = count - 1; j > i; j--) {
            comCount++;  // 每次比较
            if (numbers[j] < numbers[j - 1]) {
                // 交换元素
                int temp = numbers[j];
                numbers[j] = numbers[j - 1];
                numbers[j - 1] = temp;
                exchCount++;  // 每次交换
                flag = true;
            }
        }
        // 如果没有发生交换，提前退出
        if (!flag) break;
    }
}
void insertionSort(int* numbers, int count,int &comCount,int &exchCount ) 
{
    for(int i=1;i<count;i++)
    {
        int key=numbers[i];
        int j=i;
        while(j>0&&numbers[j-1]>key)
        {
            comCount++;  // 每次比较
            exchCount++;  // 每次交换
            numbers[j]=numbers[j-1];
            j--;
        }
        numbers[j]=key;
        comCount++;
    }
}
void shellSort(int* numbers, int count, int &comCount, int &exchCount) {
    int gap = count / 2;
    while (gap > 0) {   
        for (int i = gap; i < count; i++) {
            int key = numbers[i];  
            int j = i - gap;    
            while (j >= 0 && numbers[j] > key) {
                comCount++;  // 每次比较
                exchCount++;  // 每次交换
                numbers[j + gap] = numbers[j];
                j = j - gap;
            }
            numbers[j + gap] = key;
            comCount++;
        }
        gap = gap / 2;
    }

    
}
void quickSort(int* numbers, int left, int right, int &comCount, int &exchCount) 
{
    if (left < right) 
    {
        int pivot = numbers[(left + right) / 2];  // 选取中间元素作为基准
        int i = left - 1;  // 左指针
        int j = right + 1;  // 右指针
        
        while (i < j) 
        {
            // 移动左指针，直到找到一个大于基准的元素
            while (numbers[++i] < pivot) 
            {
                comCount++;  // 每次比较
            }
            
            // 移动右指针，直到找到一个小于基准的元素
            while (numbers[--j] > pivot) 
            {
                comCount++;  // 每次比较
            }

            if (i < j) 
            {
                // 交换元素
                int temp = numbers[i];
                numbers[i] = numbers[j];
                numbers[j] = temp;
                exchCount++;  // 每次交换
            }
        }

        // 递归排序左右两边
        quickSort(numbers, left, j, comCount, exchCount);
        quickSort(numbers, j + 1, right, comCount, exchCount);  
    }
}

void heapify(int* numbers, int count, int i, int &comCount, int &exchCount) 
{
    int smallest = i;  // 假设当前节点为最小值
    int left = 2 * i + 1;  // 左子节点
    int right = 2 * i + 2;  // 右子节点

    // 找出最小值
    if (left < count && numbers[left] < numbers[smallest]) 
    {
        comCount++;  // 每次比较
        smallest = left;
    }

    if (right < count && numbers[right] < numbers[smallest]) 
    {
        comCount++;  // 每次比较
        smallest = right;
    }

    // 如果找到更小的子节点，交换节点
    if (smallest != i) 
    {
        int temp = numbers[i];
        numbers[i] = numbers[smallest];
        numbers[smallest] = temp;
        exchCount++;  // 每次交换

        // 递归调整子树
        heapify(numbers, count, smallest, comCount, exchCount);
    }   
}
void heapSort(int* numbers, int count, int &comCount, int &exchCount) 
{
    // 建立小根堆
    for (int i = count / 2 - 1; i >= 0; i--) 
    {
        heapify(numbers, count, i, comCount, exchCount);
    }

    // 排序
    for (int i = count - 1; i > 0; i--) 
    {
        // 交换堆顶元素和最后一个元素
        int temp = numbers[0];
        numbers[0] = numbers[i];
        numbers[i] = temp;
        exchCount++;  // 每次交换

        // 调整堆
        heapify(numbers, i, 0, comCount, exchCount);
    }
    // 将排序结果写入 result.txt 文件
    ofstream outFile("result.txt");
    if (!outFile) {
        cerr << "无法打开文件: result.txt" << endl;
        return;
    }

    // 输出排序结果到文件
    for (int i = 0; i < count; i++) {
        outFile << numbers[i] << " ";  // 用空格分隔每个数字
    }

    outFile.close();  // 关闭文件
}   


int main() {
    const int count = 10000;
    int numbers[count];  // 声明一个数组来存储随机数
    int bubblecomcount = 0;  // 记录冒泡排序的比较次数
    int bubblexchcount = 0;  // 记录冒泡排序的交换次数
    int insertioncomcount = 0;  // 记录插入排序的比较次数
    int insertionexchcount = 0;  // 记录插入排序的交换次数
    int shellcomcount = 0;  // 记录希尔排序的比较次数
    int shellexchcount = 0;  // 记录希尔排序的交换次数
    int quickcomcount = 0;  // 记录快速排序的比较次数
    int quickexchcount = 0;  // 记录快速排序的交换次数
    int heapcomcount = 0;   
    int heapexchcount = 0;

    // 生成随机数并写入文件
    generateRandomNumbers("random.txt", count);

    // 从文件中读取数字
    readRandomNumbers("random.txt", numbers, count);

    // 对数组进行冒泡排序，并输出结果
    bubbleSort(numbers, count,bubblecomcount,bubblexchcount);
    insertionSort(numbers,count,insertioncomcount,insertionexchcount);
    shellSort(numbers,count,shellcomcount,shellexchcount);
    quickSort(numbers, 0, count - 1, quickcomcount, quickexchcount);
    heapSort(numbers, count, heapcomcount, heapexchcount);

     // 输出比较次数和交换次数到文件
    ofstream outFile("result.txt");

    if (!outFile) {
        cerr << "无法打开文件: result.txt" << endl;
        return 1;  // 如果无法打开文件，返回错误
    }

    // 写入排序统计信息到文件
    outFile << "Bubble Sort: 比较次数 = " << bubblecomcount << ", 交换次数 = " << bubblexchcount << endl;
    outFile << "Insertion Sort: 比较次数 = " << insertioncomcount << ", 交换次数 = " << insertionexchcount << endl;
    outFile << "Shell Sort: 比较次数 = " << shellcomcount << ", 交换次数 = " << shellexchcount << endl;
    outFile << "Quick Sort: 比较次数 = " << quickcomcount << ", 交换次数 = " << quickexchcount << endl;
    outFile << "Heap Sort: 比较次数 = " << heapcomcount << ", 交换次数 = " << heapexchcount << endl;

    // 关闭文件
    outFile.close();
}
