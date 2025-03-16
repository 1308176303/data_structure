#include<iostream>
#include<vector>
#include<fstream>
#include<cstdlib>
#include<string>
using namespace std;
#define BTNodePosi BTNode*

class BTNode {
public:
    BTNodePosi parent;
    vector<int> key;
    vector<BTNodePosi> child;
    BTNode() {
        parent = nullptr;
        child.push_back(nullptr);//插入一个空孩子节点

    }
    //创建新节点
    BTNode(int e, BTNodePosi lc = nullptr, BTNodePosi rc = nullptr) {
        parent = nullptr;//作为根节点，初始时只有一个关键码以及两个孩子
        key.push_back(e);
        child.push_back(lc);
        child.push_back(rc);
        if (lc) lc->parent = this;
        if (rc) rc->parent = this;

    }
};

class B_Tree {
private:
    int _size; // B树的元素数量
    int _order; // 阶数
    BTNodePosi _root; // 根节点
    BTNodePosi _hot; // BTree::search()最后访问的非空（除非树空）的节点位置
    void solveOverflow(BTNodePosi);
    void solveUnderflow(BTNodePosi);
    void release(BTNodePosi);
public:
    B_Tree(int order = 3) :_order(order), _size(0), _hot(nullptr) {
        _root = new BTNode();
    }
    ~B_Tree() { if (_root) release(_root); }

    bool insert(const int& e); // 插入
    bool remove(const int& e); // 删除
    void generateDOT(const std::string& filename); // 生成 dot 文件

    int const order() { return _order; } // 返回阶数
    int const size() { return _size; } // 元素数量
    BTNodePosi& root() { return _root; }
    bool empty() { return !_root; } // 判断是否为空
    BTNodePosi search(const int& e); // 查找，返回节点

    // 生成 DOT 文件的私有方法
    void generateDOT(BTNodePosi node, std::ofstream& file) {
        if (!node) return;

        // 为当前节点生成一个唯一的标识符
        static int nodeCount = 0;
        int currentNode = nodeCount++;

        // 写入当前节点的信息
        file << "  node" << currentNode << " [label=\"{";
        for (size_t i = 0; i < node->key.size(); ++i) {
            file << node->key[i];
            if (i < node->key.size() - 1) {
                file << " | ";
            }
        }
        file << "}\"];\n";

        // 递归生成子节点
        for (size_t i = 0; i < node->child.size(); ++i) {
            if (node->child[i]) {
                file << "  node" << currentNode << " -> node" << nodeCount << ";\n";
                generateDOT(node->child[i], file);
            }
        }
    }
};

// 生成 DOT 文件的公共方法
void B_Tree::generateDOT(const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        cerr << "无法打开文件: " << filename << endl;
        return;
    }
    file << "digraph BTree {\n";
    generateDOT(_root, file);
    file << "}\n";
    file.close();
}
//首先实现查找的代码
BTNodePosi B_Tree::search(const int& e) {
    BTNodePosi t = _root;          //临时变量接收树根
    while (t) {
        int i = 0;
        while (i < t->key.size() && e > t->key[i]) i++;
        if (i < t->key.size() && e == t->key[i]) return t;//找到了返回该节点
        _hot = t;//将叶子节点进行记录
        t = t->child[i];//如果没有找到，进入孩子节点 继续找
    }
    return nullptr;//找到叶子节点，还没找到，说明不在树中
}

//然后实现插入的操作
bool B_Tree::insert(const int& e) {
    //先判断数据是否在树中
    BTNodePosi node = search(e);
    if (node) {
        //如果找到了，说明不能插入
        cout << "插入失败！该节点已经在树中了！" << endl;
        return false;
    }

    int i = 0;
    while (i < _hot->key.size() && e > _hot->key[i]) i++;
    _hot->key.insert(_hot->key.begin() + i, e);//将数值添加进树
    _hot->child.insert(_hot->child.begin() + i, nullptr);//添加孩子指针
    _size++;//数量增加
    solveOverflow(_hot);
    return true;//添加成功
}

//解决上溢的问题
void B_Tree::solveOverflow(BTNodePosi v) {

    if (v->child.size() <= _order) return;//递归基 ：当前节点并未上溢出

    int mid = _order / 2;//轴点（此时应有_order = key.size() = child.size() - 1）,目前是向下取整的结果
    BTNodePosi new_node = new BTNode();//新节点已经有了一个孩子,将节点清空
    new_node->child.clear();

    for (int i = mid + 1; i < v->key.size(); i++) {
        new_node->key.push_back(v->key[i]);
        new_node->child.push_back(v->child[i]);//将旧节点中后半部分的数据转移到新节点中
        if (v->child[i]) v->child[i]->parent = new_node;//如果子节点不为空，重新指认父节点
    }
    new_node->child.push_back(v->child[v->child.size() - 1]);//将最后一个节点转移到新节点中，并重新指认父节点
    if (v->child[v->child.size() - 1]) v->child[v->child.size() - 1]->parent = new_node;

    if (v->parent) {//如果父节点不为空
        int i = 0;//先找到父节点中指向该子节点的指针的位置
        while (i < v->parent->key.size() && v->key[0] > v->parent->key[i]) i++;
        //然后将中间节点key 和指针插入进父节点中
        v->parent->key.insert(v->parent->key.begin() + i, v->key[mid]);
        v->parent->child.insert(v->parent->child.begin() + i + 1, new_node);
        new_node->parent = v->parent;//指认新节点的父节点
    }
    else {
        BTNodePosi new_root = new BTNode(v->key[mid], v, new_node);
        v->parent = new_root;
        new_node->parent = new_root;
        _root = new_root;
    }
    if (v->parent->key.size() > _order - 1) solveOverflow(v->parent);
    //把原来节点中的多余的数据给删除
    v->key.erase(v->key.begin() + mid, v->key.end());
    v->child.erase(v->child.begin() + mid + 1, v->child.end());

}


//元素删除
bool B_Tree::remove(const int& e) {
    BTNodePosi v = search(e);
    if (!v) {
        cout << "删除失败，树中不含有数据 " << e <<" ！"<< endl;
        return false;//如果没有找到e就返回false
    }
    int i = 0;
    while (i < v->key.size() && e > v->key[i]) i++;

    if (v->child[1]) {
        //若node非叶子，则e的后继必属亍某叶节点
        BTNodePosi u = v->child[i + 1];//在右子树中一直向左
        while (u->child[0]) u = u->child[0];
        //之后交换位置
        v->key[i] = u->key[0];
        v = u; i = 0;//v指向最下层待删除节点 i 为待删除的位置
    }
    v->key.erase(v->key.begin() + i);
    v->child.erase(v->child.begin() + i +  1);
    _size--;
    //如果有需要，做旋转与合并
    solveUnderflow(v);
    return true;
}
//解决下溢
void B_Tree::solveUnderflow(BTNodePosi v) {

    if ((_order + 1) / 2 <= v->child.size()) return;//递归基，节点没有下溢

    BTNodePosi p = v->parent;
    if (!p) {//递归基：已经到根节点，没有找到孩子的下线
        if (!v->key.size() && v->child[0]) {
            //但倘若作为树根的v已不含元素，却有（唯一的）非空孩子，则
            _root = v->child[0]; _root->parent = NULL; //这个节点可被跳过
            v->child[0] = NULL; release(v); //并因不再有用而被销毁
        }//整棵树高度降低一层
        return;
    }
    int i = 0;
    while (p->child[i] != v) i++;
    //确定v是p的第i个孩子——此时v可能不含关键码，故不能通过关键码查找
    
    //情况一：向左兄弟借关键码
    if (0 < i) {//如果v不是p的第一个孩子
        BTNodePosi ls = p->child[i - 1];//左兄弟必然存在
        if ((_order + 1) / 2 < ls->child.size()) {//如果左兄弟足够胖
            v->key.insert(v->key.begin(), p->key[i - 1]);//添加关键码
            p->key[i - 1] = ls->key.back(); ls->key.pop_back();//将左孩子结点的最大值上去并删除
            v->child.insert(v->child.begin(), ls->child.back());//将左兄弟的孩子过继给v
            if (ls->child.back()) ls->child.back()->parent = v;//如果不为空，将其父节点指向后爹
            ls->child.pop_back();//在左兄弟中删除
            return;
        }

    }
    //情况二：向右兄弟借关键码
    if (p->child.size() - 1 > i) {//若v不是p的最后一个孩子，
        BTNodePosi rs = p->child[i + 1];//右兄弟必然存在
        if ((_order + 1) / 2 < rs->child.size()) {//如果右兄弟足够胖
            v->key.push_back(p->key[i]);
            p->key[i] = rs->key[0]; rs->key.erase(rs->key.begin());
            v->child.push_back(rs->child[0]);
            if (rs->child[0]) rs->child[0]->parent = v;
            rs->child.erase(rs->child.begin());
            return;
        }

    }
    //情况三：左右孩子要么为空，要么太瘦,合并
    if (0 < i) {
        BTNodePosi ls = p->child[i - 1];//左兄弟必存在
        ls->key.push_back(p->key[i - 1]); p->key.erase(p->key.begin() + i - 1);
        p->child.erase(p->child.begin() + i);//
        //p 的 第i  - 1 个关机码转入ls ， v不再是 p 的第 i 个孩子
        ls->child.push_back(v->child[0]); v->child.erase(v->child.begin());
        //如果不为空
        if (ls->child.back())
            ls->child.back()->parent = ls;
        while (!v->key.empty()) {
            ls->key.push_back(v->key[0]); v->key.erase(v->key.begin());
            ls->child.push_back(v->child[0]); v->child.erase(v->child.begin());
            if (ls->child.back())
                ls->child.back()->parent = ls;
        }
        release(v);

    }
    else {//与右兄弟合并
        BTNodePosi rs = p->child[i + 1];

        rs->key.insert(rs->key.begin(), p->key[i]); p->key.erase(p->key.begin() + i);
        p->child.erase(p->child.begin() + i);
        //p的第i个关键码转入rs，v不再是p的第i个孩子
        rs->child.insert(rs->child.begin(), v->child.back()); v->child.pop_back();
        if (rs->child[0]) rs->child[0]->parent = rs;
        while (!v->key.empty()) {
            rs->key.insert(rs->key.begin(), v->key.back()); v->key.pop_back();
            rs->child.insert(rs->child.begin(), v->child.back()); v->child.pop_back();
            if (rs->child[0]) rs->child[0]->parent = rs;
        }
        release(v);
    }
    solveUnderflow(p);
    return;
}
//释放树
void B_Tree::release(BTNodePosi v) {
    if (!v) return;
    for (int i = 0; i < v->child.size(); i++) {
        release(v->child[i]);
    }
    delete v;
    return;
}

int main() {
    // cout << "请输入关键字的个数及要建立的B树的阶数"
    //     << endl;
    // int key_number, my_order;
    // cin >> key_number >> my_order;
    B_Tree tree(5);
    // cout << "请输入所有的关键字，以回车分隔" << endl;
    // for (int i = 0; i < key_number; i++) {
    //     int value;
        
    //     while (true) {
    //         cin >> value;
    //         break; // 输入有效，退出循环
    //     }
       
    //     tree.insert(value);

    // }
    // tree.generateDOT("BTree.dot");

    char temp;
    char temp2;
    string str;
    int num;
    while (cin>>temp) {
        if (temp == 'i') {
            while (cin.get(temp2)) {
                if (temp2 >= '0' && temp2 <= '9') {
                    str += temp2;
                }
                else {
                    if (!str.empty()) {
                        num = stoi(str);
                        if (tree.insert(num))
                            cout << "插入成功：" << endl;
                        str.clear();
                    }
                    if (temp2 == '#')
                        break;
                }
            }
           tree.generateDOT("BTree.dot"); 
        }
        else if (temp == 'd') {

            while (cin.get(temp2)) {
                if (temp2 >= '0' && temp2 <= '9') {
                    str += temp2;
                }
                else {
                    if (!str.empty()) {
                        num = stoi(str);
                        if (tree.remove(num))
                            cout << "删除成功：" << endl;
                        str.clear();
                    }
                    if (temp2 == '#')
                        break;
                }
            }
            tree.generateDOT("BTree.dot");

        }
        else if (temp == 'F') {
            break;
        }
        else {
            cout << "输入不合法，请重新输入" << endl;
        }
        // tree.generateDOT("BTree.dot");

    }
    return 0;
}
