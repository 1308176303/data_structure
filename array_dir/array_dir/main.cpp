#include<iostream>
#include<fstream>
#include<iomanip>
using namespace std;
class Node
{
public:
	int row;
	int col;
	int text;
	Node* next;
	Node(int r = -1, int c = -1, int tx = 0) :row(r), col(c), text(tx), next(NULL) {}
};

class arrayNode
{
public:
	string word;

	int count;
	int dirNext, rateNext;

	Node* pos;

	arrayNode() :word(""), count(0), dirNext(-1), rateNext(-1)
	{
		pos = new Node();
	}
};

class array_List
{
public:
	arrayNode* element;
	int maxsize;

	int ratehead, dirhead;
	int tail;


	array_List() :ratehead(-1), dirhead(-1), tail(0), maxsize(1000)
	{
		element = new arrayNode[1000];
	}

	void insert(string s, int row, int col, int text);

	void print();

	void writeToRes(int n);
};
bool isletter(char c)
{
	if (c >= 'a' && c <= 'z' || c>='A'&&c<='Z') return true;
	return false;
}

string getfile(string path)
{
	//cout << "start get file!\n";
	fstream file;
	string res;
	file.open(path, ios::in);
	if (file.is_open())
	{
		cout << "file open!\n";
		char ch;
		while (file.get(ch))
		{
			if (ch >= 'A' && ch <= 'Z') ch += 32;
			if (!isletter(ch)&&ch!='\n')
			{
				res += " ";
			}
			else
			{
				res += ch;
			}	
		}
	}
	else
	{
		cout << path << " open failed!\n";
	}
	//cout << res;
	return res;
	file.close();
}

void solution(string text, array_List* arr, int n)
{
	int row = 1, col = 0;
	string temp = "";
	int i = 0;
	Node* start = new Node(0, 0);
	/*cout << "start insert!\n";
	cout << "text.length = " << text.length() << endl;*/
	while (i < text.length())
	{
		col++;
		//cout << "the " << i << " times,the character is:"<<text[i]<<",eof \n";
		
		if (text[i]==' ' || text[i] == '\n')
		{
			if (text[i] == '\n')
			{
				//cout << " way 1\n";
				if (temp != "")
				{
					arr->insert(temp, start->row, start->col, n);
					temp = "";
				}
				row++;
				col = 0;
			}
			else
			{
				if (temp != "" && isletter(temp[0]))
					arr->insert(temp, start->row, start->col, n);
				//cout <<temp<< "insert over!\n";
				//arr->print();
				temp = "";
				i++;
				while (text[i]==' ' || text[i] == '\n')
				{
					if (text[i] == '\n')
					{
						row++;
						i++;
						col = 0;
					}
					//cout << "while\n";
					else
					{
						i++;
						col++;
					}

				}
				continue;
			}
			
		}
		//是字母
		else
		{
			//如果是一个单词的开始
			if (i == 0 || text[i - 1]==' ' || text[i - 1] == '\n')
			{
				//cout << "start of a word:" << text[i] << endl;
				start->row = row;
				start->col = col;
				//cout << "start->row:" << start->row << ",start->col:" << start->col << endl;
			}
			//cout << " way3\n";
			temp += text[i];
		}
		i++;
	}
	delete start;
	//cout << " while finished!\n";
}

int main(int argc, char* argv[])
{

	array_List* arr = new array_List();
	for (int i = 1; i < argc; i++)
	{
		string text = getfile(argv[i]);
		solution(text, arr, i);
		arr->writeToRes(i);
		cout << "file1 finished!\n";
	}
	return 0;
}

void array_List::insert(string s, int row, int col, int n)
{
	//如果数组为空,插入第一个元素
	if (ratehead == dirhead && ratehead == tail - 1 && tail == 0)
	{
		element[tail].word = s;
		element[tail].count++;
		element[tail].pos->next = new Node(row, col, n);
		ratehead = tail;
		dirhead = tail;
		tail++;
		return;
	}

	//如果数组是已经存了元素的状态

	//1.空间已满，需要扩容

	if (tail == maxsize - 1)
	{
		maxsize *= 2;
		arrayNode* newnode = new arrayNode[maxsize];
		copy(element, element + tail, newnode);
		delete[] element;
		element = newnode;
	}

	//查看是否已经收录
	int curr = ratehead;
	int pre = ratehead;
	while (curr != -1)
	{
		//如果已经收录，count+1 不用管dir序
		if (element[curr].word == s)
		{
			//cout << "have appeared!\n";
			element[curr].count++;
			//更新pos链表
			Node* c = element[curr].pos;
			while (c->next)
				c = c->next;
			c->next = new Node(row, col, n);

			//cout << element[curr].count << "," << element[element[curr].rateNext].count << endl;
			//更新rate排序
			if (element[curr].rateNext == -1) return;//在数组尾就直接return
			if (element[curr].rateNext != -1 && ((element[curr].count < element[element[curr].rateNext].count) || (element[curr].count == element[element[curr].rateNext].count)))
			{
				//加完之后还是比下一个小，就不用挪，只加就行
				//cout << "still smaller,didn't changed!\n";
				return;
			}

			//如果加完之后比它下一个的count大，需要往后挪
			int temp = element[curr].rateNext, prev = curr;
			while (element[temp].count < element[curr].count && temp != -1)
			{
				prev = temp;
				temp = element[temp].rateNext;
			}
			//需要挪到数组尾部
			element[pre].rateNext = element[curr].rateNext;//把要挪的元素摘出来
			if (temp == -1)
			{
				element[prev].rateNext = curr;
				element[curr].rateNext = -1;
				return;
			}
			//插入temp前面，prev后面
			element[curr].rateNext = temp;
			element[prev].rateNext = curr;
			return;
		}
		pre = curr;
		//cout << "element[" << curr << "].word=" << element[curr].word;
		curr = element[curr].rateNext;
		//cout << " curr = " << curr << endl;
	}

	//执行到这说明没出现过，需要新收录(一定收录在element[tail]上）

	//1.更新rateNext
	element[tail].word = s;
	element[tail].count++;
	element[tail].pos->next = new Node(row, col, n);
	//新插入出现次数必为1，数组中的单词出现次数一定>=1，频数链表是升序存储，所以接在头结点前面即可
	element[tail].rateNext = ratehead;
	ratehead = tail;

	//2.更新dirNext
	if (element[tail].word < element[dirhead].word)//需要插在头结点前面的情况
	{
		element[tail].dirNext = dirhead;
		dirhead = tail;
	}
	else
	{
		int curr = dirhead;
		int prev = dirhead;
		while (curr != -1 && element[curr].word < element[tail].word)
		{
			//cout << "element[" << curr << "]<element[" << tail << "]\n";
			prev = curr;
			curr = element[curr].dirNext;
			//cout << "curr=" << curr << "prev=" << prev << endl;
		}
		//需要插入数组尾部
		if (curr == -1)
		{
			element[prev].dirNext = tail;
			element[tail].dirNext = -1;
			tail++;
			return;
		}
		element[prev].dirNext = tail;
		element[tail].dirNext = curr;
		/*cout << "element[" << prev << "].dirNext = " << element[prev].dirNext <<endl;
		cout<<"element["<<tail<<"].dirNext = "<< element[tail].dirNext <<endl;*/
	}
	//cout << element[tail].word << " never appeared,update over!\n";
	tail++;
}

void array_List::print()
{
	cout << "ratehead:" << ratehead << ", dirhead:" << dirhead << endl;
	for (int i = 0; i < tail; i++)
	{
		cout << left << setw(3) << i << " " << left << setw(10) << element[i].word << " count:" << element[i].count << " rateNext:" <<
			left << setw(3) << element[i].rateNext << " dirNext:" << left << setw(3) << element[i].dirNext << endl;
		int sum = 0;
		Node* curr = element[i].pos;
		cout << "    ";
		while (curr->next)
		{
			curr = curr->next;
			sum++;
			cout << "(" << curr->row << "," << curr->col << ")";
		}
		cout << endl;
	}
}

void array_List::writeToRes(int n)
{
	string path;
	path = "out" + string(1, n + '0') + ".txt";
	/*else if (n == 2) path = "out2.txt";
	else if (n == 3) path = "out3.txt";*/
	ofstream outFile(path);

	if (!outFile) {
		cerr << "无法打开文件: " << path << endl;
		return;
	}

	int curr = dirhead;
	while (curr != -1)
	{
		/*if (!isletter(element[curr].word[0] ))
		{
			curr = element[curr].dirNext;
			continue;
		}*/
		outFile << element[curr].word << ";" << element[curr].count << ";";
		Node* current = element[curr].pos;
		while (current->next)
		{
			current = current->next;
			outFile << "(" << current->text << "," << current->row << "," << current->col << ");";
		}
		outFile << endl;
		curr = element[curr].dirNext;
	}

	outFile.close();
}

