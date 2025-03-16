#include <bits/stdc++.h>
using namespace std;

const int MAXINT = 2147483647;

class graph {
public:
    int nsum; // 结点数量
    int **matrix;
    int *appear;
    
    graph(int n): nsum(n) {
        appear = new int[n + 1];
        for (int i = 0; i <= n; i++) {
            appear[i] = 0;
        }
        matrix = new int*[n + 1];
        for (int i = 0; i <= n; i++) {
            matrix[i] = new int[n + 1];
        }
        for (int i = 0; i <= n; i++) {
            for (int j = 0; j <= n; j++) {
                matrix[i][j] = MAXINT; // 初始化所有边
            }
        }
    }

    void printmatrix(ofstream &out);
    bool isInDriected();
    bool isDriected();
    bool isDAG();
    bool isConnect();
    void DFS(int n);
    bool hasCycleDFS(int node, int *instack);
    int* topologicalSort();
    void dijkstra(int node, ofstream &out);
};

int main(int argc, char *argv[]) {
    if (argc < 3) {
        cout << "请输入图中结点的数量和边的数量。\n";
        return 1;  // 参数不足，退出程序
    }

    int n = atoi(argv[1]);  // 从命令行获取结点数量
    int e = atoi(argv[2]);  // 从命令行获取边的数量

    graph g(n);

    if (argc != 3 + e * 3) {
        cout << "输入的边数与提供的边信息数量不匹配。\n";
        return 1;
    }

    // 创建输出文件流，将输出写入 result.txt
    ofstream out("result.txt");

    // 从命令行参数读取每条边的信息
    int idx = 3;  // 从命令行参数数组中的第三个参数开始读取边信息
    for (int i = 0; i < e; i++) {
        int n1 = atoi(argv[idx++]);  // 获取结点1
        int n2 = atoi(argv[idx++]);  // 获取结点2
        int v = atoi(argv[idx++]);   // 获取边的权重
        g.matrix[n1][n2] = v;
    }

    g.printmatrix(out);

    if (g.isInDriected()) out << "无向图\n";
    else out << "有向图\n";

    if (g.isDAG()) out << "有向无环图\n";
    else out << "不是有向无环图\n";

    if (g.isConnect()) out << "连通图\n";
    else out << "不是连通图\n";

    out << "下为拓扑排序结果：\n";
    int* topoArray = g.topologicalSort();
    if (topoArray == NULL) {
        out << "不是有向无环图，不能拓扑排序\n";
    } else {
        for (int i = 1; i <= g.nsum; i++) {
            out << topoArray[i] << " ";
        }
        out << endl;
    }

    out << "下面求1到其余点的单源最短路径：\n";
    g.dijkstra(1, out);

    out.close();  // 关闭文件流
    return 0;
}

void graph::printmatrix(ofstream &out) {
    for (int i = 1; i <= nsum; i++) {
        for (int j = 1; j <= nsum; j++) {
            if (matrix[i][j] == MAXINT)
                out << "INF   ";
            else
                out << left << setw(5) << matrix[i][j] << " "; // 对齐宽度
        }
        out << endl;
    }
}

bool graph::isInDriected() {
    for (int i = 1; i < nsum; i++)
        for (int j = i + 1; j <= nsum; j++)
            if (matrix[i][j] != MAXINT && matrix[i][j] != matrix[j][i]) return false;
    return true;
}

bool graph::isDriected() {
    return !isInDriected();
}

bool graph::isDAG() {
    if (isInDriected()) return false;
    int *inStack = new int[nsum + 1]; // 辅助数组
    for (int i = 1; i <= nsum; i++) {
        appear[i] = 0;    // 初始化访问状态
        inStack[i] = 0;   // 初始化递归栈状态
    }

    // 遍历所有节点，检查每个连通分量
    for (int i = 1; i <= nsum; i++) {
        if (!appear[i]) {
            if (hasCycleDFS(i, inStack)) {
                delete[] inStack; // 释放内存
                return false;     // 存在环，返回不是 DAG
            }
        }
    }

    delete[] inStack; // 释放内存
    return true;      // 无环，图是 DAG
}

bool graph::isConnect() {
    for (int i = 0; i <= nsum; i++) appear[i] = 0;
    DFS(1);
    for (int i = 1; i <= nsum; i++)
        if (appear[i] == 0) return false;
    return true;
}

void graph::DFS(int node) {
    appear[node] = 1;
    for (int i = 1; i <= nsum; i++) {
        if (matrix[node][i] != MAXINT && !appear[i])
            DFS(i);
    }
}

bool graph::hasCycleDFS(int node, int *inStack) {
    appear[node] = 1;     // 标记节点为已访问
    inStack[node] = 1;    // 标记节点进入递归栈

    for (int i = 1; i <= nsum; i++) {
        if (matrix[node][i] != MAXINT) { // 如果有边
            if (!appear[i]) {            // 如果目标节点未访问
                if (hasCycleDFS(i, inStack)) {
                    return true;         // 如果发现环，立即返回
                }
            } else if (inStack[i]) {     // 如果目标节点已在递归栈中
                return true;             // 存在环
            }
        }
    }

    inStack[node] = 0;    // 递归结束时移出递归栈
    return false;
}

int* graph::topologicalSort() {
    // 创建入度数组，初始化为0
    int* indegree = new int[nsum + 1]();
    int* result = new int[nsum + 1]; // 存储拓扑排序结果
    int* topoOrder = new int[nsum + 1]; // 用于记录拓扑排序的顺序

    // 计算每个节点的入度
    for (int i = 1; i <= nsum; i++) {
        for (int j = 1; j <= nsum; j++) {
            if (matrix[i][j] != MAXINT) {  // 如果存在边i->j
                indegree[j]++;  // 目标节点的入度增加
            }
        }
    }

    // 创建队列，存储所有入度为0的节点
    int front = 0, rear = 0;
    for (int i = 1; i <= nsum; i++) {
        if (indegree[i] == 0) {
            topoOrder[rear++] = i;  // 入度为0的节点加入队列
        }
    }

    int count = 0;
    while (front < rear) {
        int node = topoOrder[front++];  // 取出队列中的节点
        result[++count] = node;  // 将其加入拓扑排序结果

        // 遍历该节点的所有邻接节点，更新它们的入度
        for (int i = 1; i <= nsum; i++) {
            if (matrix[node][i] != MAXINT) {  // 如果存在边 node -> i
                indegree[i]--;  // 将目标节点的入度减1
                if (indegree[i] == 0) {
                    topoOrder[rear++] = i;  // 如果入度为0，将该节点加入队列
                }
            }
        }
    }

    // 如果拓扑排序的结果中包含所有节点，返回结果
    if (count == nsum) {
        delete[] indegree;  // 删除临时数组
        return result;
    } else {
        delete[] indegree;
        delete[] result;
        delete[] topoOrder;
        return NULL;  // 图中存在环，无法进行拓扑排序
    }
}

void graph::dijkstra(int source, ofstream &out) {
    vector<int> dist(nsum + 1, MAXINT);  // 存储最短路径
    dist[source] = 0;  // 源节点到自身的距离为 0

    vector<bool> visited(nsum + 1, false);  // 用于标记节点是否已访问
    vector<int> previous(nsum + 1, -1);  // 用于存储路径的前驱节点，初始化为 -1

    // 执行 nsum 次，以确保每个节点都被访问到
    for (int i = 1; i <= nsum; i++) {
        int u = -1;
        int minDist = MAXINT;

        // 找到当前未访问节点中最短路径的节点 u
        for (int j = 1; j <= nsum; j++) {
            if (!visited[j] && dist[j] < minDist) {
                u = j;
                minDist = dist[j];
            }
        }

        // 如果找不到这样的节点，说明剩余节点不可达，提前结束
        if (u == -1) break;

        visited[u] = true;  // 标记当前节点为已访问

        // 更新与 u 相邻的所有节点的最短路径
        for (int v = 1; v <= nsum; v++) {
            if (matrix[u][v] != MAXINT && !visited[v])  // 如果存在边 u -> v 且 v 未访问
            {
                int newDist = dist[u] + matrix[u][v];
                if (newDist < dist[v])  // 如果找到更短的路径
                {
                    dist[v] = newDist;
                    previous[v] = u;  // 更新 v 的前驱节点为 u
                }
            }
        }
    }

    // 输出从源节点到其他所有节点的最短路径及其路径长度
    for (int i = 2; i <= nsum; i++) {
        if (dist[i] == MAXINT)
            out << "从 " << source << " 到 " << i << " 的路径不存在" << endl;
        else {
            out << "从 " << source << " 到 " << i << " 的最短路径长度为: " << dist[i] << "，路径为: ";
            
            // 输出路径
            vector<int> path;
            for (int node = i; node != -1; node = previous[node]) {
                path.push_back(node);
            }

            // 反转路径并输出
            reverse(path.begin(), path.end());
            for (size_t j = 0; j < path.size(); j++) {
                out << path[j];
                if (j != path.size() - 1) out << " -> ";
            }
            out << endl;
        }
    }
}

