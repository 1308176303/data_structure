#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <cctype>
#include <chrono>

using namespace std;

// 常量定义
const int DEFAULT_TABLE_SIZE = 10007; // 哈希表大小，选择一个质数
const long long MOD = 1000000007;     // 模数，用于防止哈希值溢出
const int G1 = 131;                    // 哈希方法一的基数
const int G2 = 31;                    // 哈希方法二的基数

// 链表节点结构
struct Node {
    string key;
    Node* next;
    Node(const string& s) : key(s), next(nullptr) {}
};

// 哈希表类
class HashTable {
private:
    Node** table;          // 哈希表数组，每个位置指向链表的头节点
    int size;              // 哈希表大小
    int collision_count;   // 冲突计数

public:
    // 构造函数
    HashTable(int sz = DEFAULT_TABLE_SIZE) : size(sz), collision_count(0) {
        table = new Node*[size];
        for(int i = 0; i < size; ++i) {
            table[i] = nullptr;
        }
    }

    // 析构函数
    ~HashTable() {
        for(int i = 0; i < size; ++i) {
            Node* current = table[i];
            while(current) {
                Node* temp = current;
                current = current->next;
                delete temp;
            }
        }
        delete[] table;
    }

    // 哈希函数一：多项式滚动哈希（编码字母和数字，字母使用ASCII码）
    long long hash1(const string& s) const {
        long long hash = 0;
        for(char c : s) {
            if(isalpha(c)) {
                hash = (hash * G1 + static_cast<int>(tolower(c))) % MOD;
            }
            else if(isdigit(c)) {
                hash = (hash * G1 + (c - '0')) % MOD;
            }
            // 非字母非数字字符已在 cleanWord 中移除，不需处理
        }
        return hash;
    }

    // 哈希函数二：基于辅音字母和数字编码的哈希（忽略元音字母，字母使用ASCII码）
    long long hash2(const string& s) const {
        long long hash = 0;
        for(char c : s) {
            char lower = tolower(c);
            // 检查是否为辅音字母或数字
            if( (isalpha(lower) && !strchr("aeiou", lower)) || isdigit(c) ) {
                if(isalpha(lower)){
                    hash = (hash * G2 + static_cast<int>(lower)) % MOD;
                }
                else { // 数字
                    hash = (hash * G2 + (c - '0')) % MOD;
                }
            }
            // 元音字母和非字母非数字字符忽略
        }
        return hash;
    }

    // 插入单词，选择哈希方法
    void insert(const string& key, int method) {
        long long hash_val = (method == 1) ? hash1(key) : hash2(key);
        int index = hash_val % size;

        Node* current = table[index];
        while(current) {
            if(current->key == key) {
                // 单词已存在，跳过插入
                return;
            }
            current = current->next;
        }

        // 单词不存在，检查是否需要增加冲突计数
        if(table[index] != nullptr) {
            collision_count++; // 该位置已有其他单词，发生冲突
        }

        // 插入到链表头
        Node* new_node = new Node(key);
        new_node->next = table[index];
        table[index] = new_node;
    }

    // 查找单词，返回查找步数（从1开始），如果未找到则返回-1
    int find(const string& key, int method) const {
        long long hash_val = (method == 1) ? hash1(key) : hash2(key);
        int index = hash_val % size;
        int steps = 1;
        Node* current = table[index];
        while(current) {
            if(current->key == key) {
                return steps;
            }
            current = current->next;
            steps++;
        }
        return -1; // 未找到
    }

    // 获取总冲突次数
    int getCollisionCount() const {
        return collision_count;
    }
};

// 工具函数：将字符串转换为小写并去除非字母非数字字符
string cleanWord(const string& word) {
    string clean;
    for(char c : word) {
        if(isalpha(c) || isdigit(c)) {
            clean += tolower(c);
        }
    }
    return clean;
}

// 工具函数：从文件中读取单词
int readWords(const string& filename, string* words, int max_words) {
    ifstream infile(filename);
    if(!infile.is_open()) {
        cout << "无法打开文件: " << filename << endl;
        return 0;
    }
    int count = 0;
    string word;
    while(infile >> word && count < max_words) {
        string clean = cleanWord(word);
        if(!clean.empty()) {
            words[count++] = clean;
        }
    }
    infile.close();
    return count;
}

int main() {
    // 文件路径
    const string file1 = "article1.txt"; // 用于构建哈希表
    const string file2 = "article2.txt"; // 用于查找

    // 读取第一篇文章的单词
    const int MAX_WORDS = 100000;
    string* words1 = new string[MAX_WORDS];
    int count1 = readWords(file1, words1, MAX_WORDS);
    cout << "第一篇文章读取了 " << count1 << " 个单词。" << endl;

    // 创建两个哈希表，分别使用两种哈希方法
    HashTable ht1; // 方法一：多项式滚动哈希
    HashTable ht2; // 方法二：基于辅音字母和数字编码的哈希

    // 插入单词到哈希表，并记录时间
    auto start_insert1 = chrono::high_resolution_clock::now();
    for(int i = 0; i < count1; ++i) {
        ht1.insert(words1[i], 1);
    }
    auto end_insert1 = chrono::high_resolution_clock::now();
    chrono::duration<double> duration_insert1 = end_insert1 - start_insert1;
    cout << "哈希方法一插入完成，耗时: " << duration_insert1.count() << " 秒。" << endl;

    auto start_insert2 = chrono::high_resolution_clock::now();
    for(int i = 0; i < count1; ++i) {
        ht2.insert(words1[i], 2);
    }
    auto end_insert2 = chrono::high_resolution_clock::now();
    chrono::duration<double> duration_insert2 = end_insert2 - start_insert2;
    cout << "哈希方法二插入完成，耗时: " << duration_insert2.count() << " 秒。" << endl;

    // 读取第二篇文章的单词
    string* words2 = new string[MAX_WORDS];
    int count2 = readWords(file2, words2, MAX_WORDS);
    cout << "第二篇文章读取了 " << count2 << " 个单词。" << endl;

    // 查找单词并统计平均查找长度
    long long total_steps1 = 0;
    long long found1 = 0;
    auto start_find1 = chrono::high_resolution_clock::now();
    for(int i = 0; i < count2; ++i) {
        int steps = ht1.find(words2[i], 1);
        if(steps != -1) {
            total_steps1 += steps;
            found1 += 1;
        }
    }
    auto end_find1 = chrono::high_resolution_clock::now();
    chrono::duration<double> duration_find1 = end_find1 - start_find1;
    double avg_steps1 = (found1 == 0) ? 0.0 : ((double)total_steps1 / found1);
    cout << "哈希方法一查找完成，耗时: " << duration_find1.count() << " 秒。" << endl;
    cout << "哈希方法一平均查找长度: " << avg_steps1 << endl;

    long long total_steps2 = 0;
    long long found2 = 0;
    auto start_find2 = chrono::high_resolution_clock::now();
    for(int i = 0; i < count2; ++i) {
        int steps = ht2.find(words2[i], 2);
        if(steps != -1) {
            total_steps2 += steps;
            found2 += 1;
        }
    }
    auto end_find2 = chrono::high_resolution_clock::now();
    chrono::duration<double> duration_find2 = end_find2 - start_find2;
    double avg_steps2 = (found2 == 0) ? 0.0 : ((double)total_steps2 / found2);
    cout << "哈希方法二查找完成，耗时: " << duration_find2.count() << " 秒。" << endl;
    cout << "哈希方法二平均查找长度: " << avg_steps2 << endl;

    // 输出冲突率
    cout << "哈希方法一总冲突数: " << ht1.getCollisionCount() << endl;
    cout << "哈希方法二总冲突数: " << ht2.getCollisionCount() << endl;

    // 计算并输出冲突率
    double collision_rate1 = (count1 > 0) ? ((double)ht1.getCollisionCount() / count1) : 0.0;
    double collision_rate2 = (count1 > 0) ? ((double)ht2.getCollisionCount() / count1) : 0.0;
    cout << "哈希方法一冲突率: " << collision_rate1 * 100 << " %" << endl;
    cout << "哈希方法二冲突率: " << collision_rate2 * 100 << " %" << endl;

    // 清理内存
    delete[] words1;
    delete[] words2;

    return 0;
}
