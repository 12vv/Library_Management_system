#pragma once
#define m 3
#define TRUE 1
#define FALSE 0

extern ofstream f1;

typedef enum Status
{
	SUCCESS, ERROR
}Status;

typedef int KeyType;

//关键字结构
typedef struct {
	KeyType key;             //关键字
	struct book_item* book;  //指向存储图书信息
}RcdType;

//B树
typedef struct BTNode{
	int keynum;                 //结点当前关键字数
	RcdType item[m+1];          //关键字数组，item[0]未用
	struct BTNode *parent;      //双亲结点指针
	struct BTNode *ptr[m + 1];  //孩子结点指针数组
	//
}BTNode, *BTree;            

//图书信息
typedef struct book_item {
	KeyType key;        //书号
	char name[10];      //书名
	char author[10];    //作者
	int inventory;      //总库存
	int amount;         //图书现存量
	struct record *record_item;     //指向借阅记录
};

//借阅记录
typedef struct record {
	int reader_id;
	char date[10];
	struct record* next;    //指向下一条记录
};

//查找结果
typedef struct {
	BTree pt;         //返回找到的结点
	int i;            //1<=i<=m,在结点中的关键字位序
	int tag;          //查找成功值为1，查找失败值为0
}result;


/*********************与B树相关的操作**********************/
//B数的查找
void SearchBTree(BTree t, int k, result &r);
//在p->item[1..p->keynum].key中找k
int Search(BTree p, int k);
//B树的插入
void InsertBTree(BTree &t, RcdType k, BTree q, int i);
//B数的删除
//void DeleteBTree(BTree &p, int i);
Status  DeleteBTree(BTree &T, RcdType k, BTree &q, int i);
//凹入表显示B树关键字
void printBTree(BTree t,int &count);

//内部操作
//生成新的根节点
void newRoot(BTree &t, BTree p, RcdType x, BTree ap);
//将q结点分裂成两个结点，前一半保留在原始点，后一半移入ap所指新结点
void split(BTree &q, int s, BTree &ap);
//关键字x和结点ap分别插入q->key[i].key和q->ptr[i]
void Insert(BTree &q, int i, RcdType x, BTree ap);
//若被删结点不是叶子结点，找后继替代被删结点
void Successor(BTree &p, int i);
//从节点p中删除item[i]
void Remove(BTree &p, int i);
//删除后调整B树
//void Restore(BTree &p, int i);
//
Status borrowNode(BTree &q);
void Combine(BTree &q);
//测试用
void test(BTree &t);

/*********************与系统相关的操作***********************/
//欢迎界面(仅在开始时调用)
void welcome();
//显示主菜单
void menu();
//采编入库
void stockIn(BTree &T);
//清除库存
void stockOut(BTree &T);
//借阅图书
void borrow(BTree &T);
//归还图书
void return_book(BTree &T);
//预约借书
void reserve(BTree &T);
//图书信息
void book_info(BTree T);
//退出界面
void out();