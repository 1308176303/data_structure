#include <iostream>
#include <cstring> // 使用 memset 初始化
using namespace std;

int main() {
    int N, M, K;
    cin >> N >> M >> K;

    // 动态分配内存：仅考虑最多 N 行可能的模式
    int* patterns = new int[N];       // 存储每行的模式
    int* pattern_count = new int[N]; // 存储每种模式的出现次数
    int* zeros_count = new int[N];   // 存储每种模式中 0 的数量
    memset(pattern_count, 0, N * sizeof(int));

    int pattern_index = 0; // 已记录的模式数量

    // 输入矩阵并记录模式
    for (int i = 0; i < N; ++i) {
        int pattern = 0;
        for (int j = 0; j < M; ++j) {
            char c;
            cin >> c;
            if (c == '1') {
                pattern |= (1 << j); // 将对应列标记为 1
            }
        }

        // 检查该模式是否已经存在
        bool found = false;
        for (int p = 0; p < pattern_index; ++p) {
            if (patterns[p] == pattern) {
                pattern_count[p]++; // 模式出现次数增加
                found = true;
                break;
            }
        }
        if (!found) {
            // 新模式
            patterns[pattern_index] = pattern;
            pattern_count[pattern_index] = 1;

            // 计算该模式中 0 的数量
            int zeros = 0;
            for (int j = 0; j < M; ++j) {
                if ((pattern & (1 << j)) == 0) {
                    zeros++;
                }
            }
            zeros_count[pattern_index] = zeros;
            pattern_index++;
        }
    }

    // 遍历模式以找到最大行数
    int max_rows = 0;
    for (int p = 0; p < pattern_index; ++p) {
        int zeros = zeros_count[p];
        if (zeros <= K && (K - zeros) % 2 == 0) { // 满足条件
            max_rows = max_rows > pattern_count[p] ? max_rows : pattern_count[p];
        }
    }

    // 输出结果
    cout << max_rows << endl;

    // 释放动态分配的内存
    delete[] patterns;
    delete[] pattern_count;
    delete[] zeros_count;

    return 0;
}
