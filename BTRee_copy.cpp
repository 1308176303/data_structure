#include<iostream>
#include<vector>
#include<sstream>
#include<fstream>
#include<cmath>
#include <algorithm> 
using namespace std;
class Node {
public:
	vector<int> key;
	vector<Node*> child;
	Node *parent;
	bool isleaf;
	Node(bool leaf = true) :parent(NULL),isleaf(leaf){}
};
class B_Tree {
public:
	int order;
	Node* root;
	B_Tree(int _order) :root(NULL), order(_order) {};
	void insert(int x);
	void splitChild(Node* parent, int index);
	Node* search(int x,int &index);
	void remove(int x);
	void mergenode(Node* node);
	void toDot(Node* node, ostream& out) {
		if (node == NULL) return;

		out << "node" << node << " [label=\"";
		for (int key : node->key) {
			out << key << "|";
		}
		out << "\"];\n";

		for (Node* child : node->child) {
			out << "node" << node << " -> node" << child << ";\n";
			toDot(child, out);
		}
	}

	string generateDot() {
		ostringstream out;
		out << "digraph BTree {\n";
		toDot(root, out);
		out << "}\n";
		return out.str();
	}
};
int main()
{
    B_Tree btree(5); // 创建三阶 B 树
	char c;
	cin>>c;
	//不许输入0
	while(c!='q')
	{
		if(c=='i')
		{
			int a;
			cin>>a;
			btree.insert(a);
		}
		else if(c=='r')
		{
			int a;
			cin>>a;
			btree.remove(a);
		}
		//生成 DOT 格式字符串
		string dotRepresentation = btree.generateDot();

		// 写入 DOT 文件
		ofstream dotFile("btree.dot");
		dotFile << dotRepresentation;
		dotFile.close();

		cout << "DOT file generated: btree.dot" << endl;
		cin>>c;
	}
    return 0;
}
	

void B_Tree::insert(int x)
{
	if (root == nullptr)
	{
		root = new Node(true);
		root->key.push_back(x);
		return;
	}
	else
	{
		Node *curr=root;
		while(curr->isleaf==false)
		{
			int i=0;
			while(i<curr->key.size()&&x>curr->key[i]) i++;
			/*此时两种情况，1：i==curr->key.size()   说明比这一个节点的所有
			key都大，直接转到最后下角的孩子即child [i]
			2：在中途就弹出循环，此时curr->key[i-1]<x<curr->key[i],
			应该进入child[i]，因为child[i]是小于key[i]但大于key[i-1]
			的节点存放的孩子
			综上直接跳到child[i]*/
			curr=curr->child[i];
		}
		//现在是叶节点了 
		int j=0;
		while(j<curr->key.size()&&x>curr->key[j]) j++;
		if(x==curr->key[j])
		{
			cout<<x<<" already appeared\n";
			return;
		} 
		if (curr->key.size() == order - 1)
		{
			//需要分裂
			if(curr==root)
			{
				//当前如果是根节点
				Node* newroot = new Node(false);
				curr->parent=newroot;
				newroot->child.push_back(curr);
				curr->key.push_back(x);
				sort(curr->key.begin(),curr->key.end());
				splitChild(newroot, 0);
				root = newroot;
			}
			else
			{
				//当前不是根节点，是个普通的叶节点
				Node* par = curr->parent;
				curr->key.push_back(x);
				sort(curr->key.begin(),curr->key.end());
				int index=-1,i=0;
				//这里有问题
				for(i;i<par->key.size();i++)
				{
					if(curr->key[0]>par->key[i])
					{
						continue;
					}
					else
					{
						index=i;
						break;
					}
				}
				if(i==par->key.size()) index=i;
				if(index!=-1) splitChild(par, index);
			}
		}
		else
		{
			//正常插入到叶节点
			curr->key.push_back(x);
			sort(curr->key.begin(),curr->key.end());
		}
	}
}

void B_Tree::splitChild(Node* parent, int i)
{
	//给分裂后的孩子指向的父节点和要分裂的孩子在child数组的索引
	Node* fullchild = parent->child[i];
	Node* newchild = new Node(true);
	newchild->parent=parent;
	int midIndex = (fullchild->key.size())/ 2;

	//把数据转移一下
	parent->key.insert(parent->key.begin() + i, fullchild->key[midIndex]);
	parent->child.insert(parent->child.begin() + i + 1, newchild);
	newchild->key.assign(fullchild->key.begin() + midIndex + 1, fullchild->key.end());
	fullchild->key.resize(midIndex);

	//如果原节点有孩子的话还需要把一部分孩子分配到新节点里
	if (!fullchild->isleaf)
	{
		//重分配子节点
		newchild->isleaf=false;//新节点要被分配孩子，因此也不应该是叶节点
		newchild->child.assign(fullchild->child.begin() + midIndex + 1, fullchild->child.end());
		for(int i=midIndex+1;i<fullchild->child.size();i++)
		{
			//改父节点
			fullchild->child[i]->parent=newchild;
		}
		fullchild->child.resize(midIndex + 1);
		
	}
	//如果分裂后父节点也超了
	if (parent->key.size() > order - 1)
	{
		//parent不是根节点
		if (parent->parent != nullptr)
		{
			int index=-1;
			//找到parent在它的父节点的child容器的索引
			for (int i = 0; i < parent->parent->child.size(); i++)
			{
				if(parent->key[0]>parent->parent->key[i])
					{
						continue;
					}
					else
					{
						index=i;
						break;
					}
			}
			if(index!=-1)	splitChild(parent->parent, index);
		}
		else
		{
			//parent是根节点，需要创建一个新的根节点然后再调用
			Node* newroot = new Node(false);
			newroot->child.push_back(parent);
			parent->parent = newroot;
			root = newroot;
			splitChild(root, 0);
		}
	}
}

Node *B_Tree::search(int x,int &index)
{
	Node *curr=root;
	while(curr!=NULL)
	{
		int i=0;
		while(i<curr->key.size()&&x>curr->key[i]) i++;
		if(x==curr->key[i])
		{
			index=i;
			return curr;
		} 
		curr=curr->child[i];
	}	
    return nullptr;
}

void B_Tree::remove(int x)
{
	//先找到这个节点
	int index=-1;//index是这个x在找到的节点的key数组里的索引值
	Node *node=search(x,index);
	if(node!=NULL)
	{
		//如果是根节点也可以直接删除这个键
		if(node==root)
		{
			node->key.erase(node->key.begin()+index);
			//根节点不需要考虑下溢
			return;
		}
		if(node->isleaf)
		{
			//是叶节点就能直接删除这个键
			node->key.erase(node->key.begin()+index);
			//删除之后如果下溢就需要借或者合并
			cout<<node->key.size()<<",,,,"<<(order+1)/2-1<<endl;
			if(node->key.size() < (order+1)/2-1) mergenode(node);
			return;
		}
		/*不是叶节点就需要用左子树的最大节点的最后一个键
		或者右子树的最小节点的第一个键来代替它，
		再去删那个叶节点
		*/
		//先找左子树的最大节点能不能用
		Node *leftobject=node->child[index];
		while(leftobject->isleaf==false)
			leftobject=leftobject->child[leftobject->child.size()-1];
		//现在leftobject是node的左子树的最右下角
		// if(leftobject->key.size()>(order+1)/2-1)
		// {
			//可以用来代替并删除这个叶节点对应的键
		node->key[index]=leftobject->key[leftobject->key.size()-1];
		leftobject->key.pop_back();
		cout<<leftobject->key.size()<<endl;
		if(leftobject->key.size()<(order+1)/2-1)
			mergenode(leftobject);
			//不会下溢就不检查了
			// return;
		// }
		// //左子树的最右下角key不够用，不能替换
		// //找右子树最左下角
		// Node *robject=node->child[index+1];
		// while(robject->isleaf==false)
		// 	robject=robject->child[0];
		// //现在是右子树的最左下角的叶节点
		// if(robject->key.size()>(order+1)/2-1)
		// {
		// 	//可以用来代替并删除这个叶节点对应的键
		// 	node->key[index]=robject->key[0];
		// 	robject->key.erase(robject->key.begin());
		// 	return;
		// }
		// //左右子树的叶节点都不能用,就直接跟左子树叶节点换
		// node->key[index]=leftobject->key[leftobject->key.size()-1];
		// leftobject->key.pop_back();
		// //既然都不能用，那必然下溢
		// mergenode(leftobject);
	}
}

void B_Tree::mergenode(Node *node)
{
	/*先找左右兄弟能不能借，都不能借再跟左右兄弟合并*/
	//先找当前节点在它父节点的孩子数组中的索引
	Node* parent=node->parent;
	int index=-1;//对应索引
	if(parent)
	{
		for (int i = 0; i < parent->child.size(); i++) 
		{
			if (parent->child[i] == node) 
				{
					index = i;
					break;
				}
			}
	}
	//看左兄弟够不够借的 (首先要有！！！)
	if(index-1>=0&&node->parent->child[index-1]->key.size()>(order+1)/2-1)
	{
		cout<<"1\n";
		//够借
		Node* lbro=node->parent->child[index-1];
		//父节点的key下移
		node->key.insert(node->key.begin(),node->parent->key[index-1]);
		//左兄弟的最大key代替父节点下移的key
		node->parent->key[index-1]=lbro->key[lbro->key.size()-1];
		//删除左兄弟的最大key
		lbro->key.pop_back();
		if(node->isleaf==false)
		{
			//如果是向上处理父节点的下溢问题的话，此处就需要重新分配孩子
			lbro->child[lbro->child.size()-1]->parent=node;
			node->child.insert(node->child.begin(),lbro->child[lbro->child.size()-1]);
			lbro->child.erase(lbro->child.begin()+lbro->child.size()-1);
		}
		return;
	}
	//不够借,看右兄弟够不够借的
	if(index+1<node->parent->child.size()&&node->parent->child[index+1]->key.size()>(order+1)/2-1)
	{
		cout<<"2\n";
		//够借
		Node* rbro=node->parent->child[index+1];
		//父节点的key下移
		node->key.push_back(node->parent->key[index]);
		//用右兄弟的最左key代替父节点下移的key
		node->parent->key[index]=rbro->key[0];
		//删除右兄弟的最左key
		rbro->key.erase(rbro->key.begin());
		//重新分配孩子
		if(rbro->isleaf==false)
		{
			// 右兄弟最左边的孩子分配给node的最右边
			rbro->child[0]->parent=node;//重分配亲爹
			node->child.push_back(rbro->child[0]);
			//删除右兄弟最左边的孩子
			rbro->child.erase(rbro->child.begin());
		}
		return;
	}
	//都不够，就跟左兄弟合并（如果有的话）
	if(index-1>=0)
	{
		cout<<"3\n";
		//左兄弟最后插入父节点的key
		Node* lbro=node->parent->child[index-1];
		lbro->key.push_back(node->parent->key[index-1]);
		//删除父节点那个key
		node->parent->key.erase(node->parent->key.begin()+index-1);
		//再把node合并到左兄弟上
		lbro->key.insert(lbro->key.end(),node->key.begin(),node->key.end());
		node->parent->child[index]=lbro;
		//删除左兄弟
		node->parent->child.erase(node->parent->child.begin()+index-1);
		cout<<"3o\n";
		if(node->parent->key.size()<(order+1)/2-1) mergenode(node->parent);
	}
	//没左兄弟，跟右兄弟合并
	cout<<"4\n";
	Node* rbro=node->parent->child[index+1];
	//父节点key下移
	node->key.push_back(node->parent->key[index]);
	//删除父节点下移的key
	node->parent->key.erase(node->parent->key.begin()+index);
	//右兄弟合并到node上
	node->key.insert(node->key.end(),rbro->key.begin(),rbro->key.end());
	//留下右兄弟作为合并结果
	node->parent->child[index+1]=node;
	//删掉作为空壳的node
	node->parent->child.erase(node->parent->child.begin()+index);
	//查看父节点是否下溢
	if(node->parent->key.size()<(order+1)/2-1) mergenode(node->parent);
}