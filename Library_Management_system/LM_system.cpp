#include<fstream>
#include<iomanip>
#include<iostream>
using namespace std;
#include<stdio.h>
#include<stdlib.h>
#include "LM_system.h"

/*********************与B树相关的操作**********************/
//B数的查找，在m阶B树上查找k，用r返回result
//若查找成功，tag=1，指针pt所指结点的第i个item的key为k
//否则tag=0，含k的item应该位于pt所指结点中第i-1个和第i个item之间
void SearchBTree(BTree t, int k, result &r) {
	int i = 0, found = 0;
	BTNode *p = t, *q = NULL; 
	while (p != NULL && found == 0) {
		i = Search(p, k);
		if (i <= p->keynum && p->item[i].key == k) found = 1;
		else {
			q = p;
			p = p->ptr[i - 1];
		}
	}
	if (found == 1) {
		r.pt = p;
		r.tag = 1;
		r.i = i;
	}
	else {
		r.pt = q;
		r.tag = 0;
		r.i = i;
	}
}

//在p->item[1..p->keynum].key中找k
int Search(BTree p, int k) {
	int i = 1;
	while (p->item[i].key < k && i <= p->keynum) i++;
	return i;
}

//B树的插入
//在B树t中q结点的item[i-1]和item[i]之间插入
void InsertBTree(BTree &t, RcdType k, BTree q, int i) {
	int s, finished = 0, needNewRoot = 0;
	RcdType x;
	BTree ap;
	if (q == NULL) newRoot(t, NULL, k, NULL);
	else {
		x = k;
		ap = NULL;
		while (finished == 0 && needNewRoot == 0) {
			Insert(q, i, x, ap);
			if (q->keynum < m) finished = 1;
			else {
				s = (m + 1) / 2;
				split(q, s, ap);
				x = q->item[s];
				if (q->parent != NULL) {
					q = q->parent;
					i = Search(q, x.key);
				}
				else {
					needNewRoot = 1;
				}
			}
		}
		if (needNewRoot == 1)
			newRoot(t, q, x, ap);
	}
}

//内部操作
//生成新的根节点(情况1：空树，情况2：原结点分裂为p和ap)
void newRoot(BTree &t, BTree p, RcdType x, BTree ap) {
	t = new BTNode;
	t->keynum = 1;
	t->ptr[0] = p;
	t->ptr[1] = ap;
	t->item[1] = x;
	if (p != NULL) p->parent = t;
	if (ap != NULL) ap->parent = t;
	t->parent = NULL;
}

//将q结点分裂成两个结点，前一半保留在原始点，后一半移入ap所指新结点
void split(BTree &q, int s, BTree &ap) {
	int i, j, n = q->keynum;
	ap = new BTNode;
	ap->ptr[0] = q->ptr[s];
	for (i = s + 1, j = 1; i <= n; i++,j++) {
		ap->item[j] = q->item[i];
		ap->ptr[j] = q->ptr[i];
	}
	ap->keynum = n - s;
	ap->parent = q->parent;
	for (i = 0; i <= n - s; i++) {
		if (ap->ptr[i] != NULL)
			ap->ptr[i]->parent = ap;
	}
	q->keynum = s - 1;
}

//关键字x和结点ap分别插入q->item[i].key和q->ptr[i]
void Insert(BTree &q, int i, RcdType x, BTree ap) {
	int j, n = q->keynum;
	for (j = n; j >= i; j--) {
		q->item[j + 1] = q->item[j];
		q->ptr[j + 1] = q->ptr[j];
	}
	q->item[i] = x;
	q->ptr[i] = ap;
	if (ap != NULL) {
		ap->parent = q;
	}
	q->keynum++;
}

//凹入表显示B树关键字
void printBTree(BTree t,int &count) {
	count = count+2;
	if (t == NULL) return;
	else {
		for (int j = 0; j < count; j++) {
			cout << " ";
		}
		for (int i = 1; i <= t->keynum; i++) {
			cout << t->item[i].key << ' ';
		}
		cout << "\n";
		for (int i = 0; i <= t->keynum; i++) {
			printBTree(t->ptr[i],count);
			count = count-2;
		}		
	}
}

//若被删结点不是叶子结点，找后继替代被删结点
void Successor(BTree &p, int i) {
	BTree q = p;
	p = p->ptr[i];
	while (p->ptr[0] != NULL) p = p->ptr[0];
	q->item[i] = p->item[1];
}

//从节点p中删除item[i]
void Remove(BTree &p, int i) {
	for (; i < p->keynum; i++) {
		p->item[i] = p->item[i + 1];
	}
	p->keynum--;
}

Status borrowNode(BTree &q) {
	int i;
	BTree p = q->parent, b = NULL;                   // p指向q的双亲结点  
	for (i = 0; p->ptr[i] != q; i++);            // 查找q在双亲p的子树位置  
	if (i >= 0 && i + 1 <= p->keynum && p->ptr[i + 1]->keynum > (m - 1) / 2)
	{                                       // 若q的右兄弟关键字个数大于(m-1)/2  
		b = p->ptr[i + 1];                       // b指向右兄弟结点  
		q->ptr[1] = b->ptr[0];                    // 子树指针也要同步移动  
		q->item[1] = p->item[i + 1];                // 从父节点借第i+1个关键字  		
		p->item[i + 1] = b->item[1];                // b第一个关键字上移到父节点  		
		for (i = 1; i <= b->keynum; i++)          // b第一个关键字上移，需把剩余记录前移一位  
		{
			b->item[i] = b->item[i + 1];			
			b->ptr[i - 1] = b->ptr[i];
		}
	}
	else if (i > 0 && p->ptr[i - 1]->keynum > (m - 1) / 2)
	{                                       // 若q的左兄弟关键字个数大约(m-1)/2  
		b = p->ptr[i - 1];                       // b指向左兄弟结点  
		q->ptr[1] = q->ptr[0];
		q->ptr[0] = b->ptr[b->keynum];
		q->item[1] = p->item[i];                    // 从父节点借第i个关键字  		
		p->item[i] = b->item[b->keynum];         // 将b最后一个关键字上移到父节点  
		
	}
	else return ERROR;                          // 无关键字大于(m-1)/2的兄弟  
	q->keynum++;
	b->keynum--;
	for (i = 0; i <= q->keynum; i++)
		if (q->ptr[i]) q->ptr[i]->parent = q;  // 刷新q的子结点的双亲指针  
	return SUCCESS;
}

void Combine(BTree &q) {
	int i, j;
	BTree p = q->parent, b = NULL;//p指向q的父亲  
	for (i = 0; p->ptr[i] != q; i++);//查找q在父亲p中的子树位置  
	if (i == 0) {//若为0，则需合并为兄弟的第一个关键字  
		b = p->ptr[i + 1];
		for (j = b->keynum; j >= 0; j--) {//将b的关键字和记录后移一位  
			b->item[j + 1] = b->item[j];
			b->ptr[j + 1] = b->ptr[j];
		}
		b->ptr[0] = q->ptr[0];//合并  
		b->item[1] = p->item[1];
	}
	else if (i > 0) {//若q在父亲的子树位置大于0，需合并为兄弟b的最后一个关键字  
		b = p->ptr[i - 1];
		b->item[b->keynum + 1] = p->item[i];//合并  
		b->ptr[b->keynum + 1] = q->ptr[0];
	}
	if (i == 0 || i == 1)//若i为0或1，需将父结点p关键字前移一位  
		for (; i < p->keynum; i++) {
			p->item[i] = p->item[i + 1];
			p->ptr[i] = p->ptr[i + 1];
		}
	p->keynum--;
	b->keynum++;
	delete(q);
	q = b;   //q指向修改的兄弟结点  
	for (i = 0; i <= b->keynum; i++)
		if (b->ptr[i]) b->ptr[i]->parent = b;//刷新b的子结点的双亲指针
}
//*************************************************************************************************************//
Status  DeleteBTree(BTree &T, RcdType k, BTree &q, int i)
// 在m阶B树T上删除q结点上关键字k，并返回SUCCESS。  
{
	int finished = FALSE;
		if (q->ptr[0]) Successor(q, i);          // 若q的子树不空，(非底层结点)  
												 // 则以其后继代之，且令q指向后继所在结点  
		Remove(q, i);                              // 删除q所指向结点中第i个关键字及记录  
		if (q->keynum >= (m - 1) / 2 || !q->parent)        // 若删除后关键字个数不小于(m-1)/2或q是根节点  
		{
			finished = TRUE;                    // 删除完成  
			if (q->keynum == 0) T = NULL;        // 若q的关键字个数为0 ，则为空树  
		}
		while (!finished)
		{
			if (borrowNode(q) == SUCCESS)  finished = TRUE;    // 若q的相邻兄弟结点关键字大于(m-1)/2,则从该  
												// 兄弟结点上移一个最大（或最小）关键字到  
												// 父节点，从父节点借一关键字到q  
			else {                              // 若q相邻兄弟关键字个数均等于┌m /2┑-1  
				Combine(q); // 将q中的剩余部分和双亲中的相关关键字合并至q的一个兄弟中  
				q = q->parent;                   // 检查双亲  
				if (q == T && T->keynum == 0)    // 若被删结点的父节点是根T且T的关键字个数为0  
				{
					T = T->ptr[0];               // 新根     
					T->parent = NULL;
					delete(q);                    // 删除原双亲结点  
					finished = TRUE;
				}
				else if (q->keynum >= m / 2) finished = TRUE;
			}                               // 合并后双亲关键字个数不少于(m-1)/2，完成  
		}
	return SUCCESS;
}
//测试用
void test(BTree &t) {
	t = new BTNode;
	BTree p = t,q = NULL;
	p->keynum = 1;         
	p->parent = NULL;
	p->item[1].key = 60;
	q = p;
	p->ptr[0] = new BTNode;
	p = p->ptr[0];
	p->keynum = 1;
	p->parent = q;
	p->item[1].key = 30;
	q = p;
	p->ptr[0] = new BTNode;
	p = p->ptr[0];
	p->keynum = 1;
	p->parent = q;
	p->item[1].key = 20;
	p->ptr[0] = NULL;
	p->ptr[1] = NULL;
	p = q;
	p->ptr[1] = new BTNode;
	p = p->ptr[1];
	p->keynum = 2;
	p->parent = q;
	p->item[1].key = 50;
	p->item[2].key = 52;
	p->ptr[0] = NULL;
	p->ptr[1] = NULL;
	p->ptr[2] = NULL;
	q = q->parent;
	p = q;
	p->ptr[1] = new BTNode;
	p = p->ptr[1];
	p->keynum = 2;
	p->parent = q;
	p->item[1].key = 70;
	p->item[2].key = 72;
	p->ptr[0] = NULL;
	p->ptr[1] = NULL;
	p->ptr[2] = NULL;
}

/*********************与系统相关的操作***********************/
//欢迎界面(仅在开始时调用)
void welcome() {
	cout << "\t\t                      ╔══════════════════════════╗" << endl;
	cout << "\t\t                      ║    欢迎进入图书管理系统  ║" << endl;
	cout << "\t\t                      ╚══════════════════════════╝" << endl;
}
//显示主菜单
void menu() {
	cout << "\n\n\t\t\t\t  *************************************" << endl;
	//cout << "\n\t\t\t\t-------------------------------------------" << endl;
	cout << "\n\t\t\t\t\t1.采编入库     2.清除库存    \n\n\t\t\t\t\t3.借阅图书     4.归还图书     \n\n\t\t\t\t\t5.图书信息     6.退出系统" << endl;
	//cout << "\n\t\t\t\t-------------------------------------------" << endl;
	cout << "\n\t\t\t\t  *************************************" << endl;
}
//退出界面
void out() {
	cout << "\t\t                      ╔══════════════════════════╗" << endl;
	cout << "\t\t                      ║    欢迎再次使用！再见    ║" << endl;
	cout << "\t\t                      ╚══════════════════════════╝" << endl;
}
//采编入库
void stockIn(BTree &T) {
	int k;	
	cout << "\t\t\t\t\t请输入你想入库图书的信息:(0停止)" << endl;
	f1 << "请输入你想入库图书的信息:(0停止)" << endl;    //log
	do {
		result r;
		cout << "\t\t\t\t\t请输入书号:";
		f1 << "请输入书号:";    //log
		cin >> k;
		f1 << k << endl;    //log
		if (k == 0) break;
		SearchBTree(T, k, r);
		char name[10];
		char author[10];
		int inventory, amount = 0;
		if (r.tag == 1) {
			cout << "\t\t\t\t\t这本书已经存在！原来总库存为" << r.pt->item[r.i].book->inventory << "本" << endl;
			cout << "\t\t\t\t\t输入增加此书库存数量:";
			f1 << "这本书已经存在！原来总库存为" << r.pt->item[r.i].book->inventory << "本" << endl;    //log
			cin >> inventory;
			f1 << "输入增加此书库存数量:" << inventory << endl;    //log
			r.pt->item[r.i].book->inventory += inventory;
			r.pt->item[r.i].book->amount += inventory;
			cout << "\t\t\t\t\t现在总库存为" << r.pt->item[r.i].book->inventory << "本" << endl;
			f1 << "现在总库存为:" << r.pt->item[r.i].book->inventory << "本" << endl;    //log
		}
		else {
			book_item* booki;
			RcdType* tem;
			tem = new RcdType;
			booki = new book_item;
			*tem = { k, booki };
			tem->book->key = k;
			char name[10];
			char author[10];
			int inventory, amount = 0;

			
			cout << "\t\t\t\t\t请输入书名:";			
			cin >> name;
			f1 << "请输入书名:" << name << endl;    //log
			strcpy_s(tem->book->name, name);
			cout << "\t\t\t\t\t请输入作者:";
			cin >> author;
			f1 << "请输入作者:" << author << endl;    //log
			strcpy_s(tem->book->author, author);
			cout << "\t\t\t\t\t请输入入库图书的数量:";
			cin >> inventory;
			f1 << "请输入入库数量:" << inventory << endl;    //log
			tem->book->inventory = inventory;
			tem->book->amount = inventory;
			tem->book->record_item = NULL;
			InsertBTree(T, *tem, r.pt, r.i);
			cout << "\t\t\t\t\t凹入表打印书号:" << endl;
			f1 << "凹入表打印书号:" << endl;    //log
			int count = 0;
			printBTree(T, count);
		}
	} while (k != 0);
}
//清除库存
void stockOut(BTree &T) {
	int k;

	do {

		result r;
		cout << "\t\t\t\t\t请输入出库图书的书号(0停止):";
		f1 << "请输入出库图书的书号: " << endl;    //log
		cin >> k;
		if (k == 0) break;
		SearchBTree(T, k, r);
		if (r.tag == 1) {			
			cout << "\t\t\t\t\t此书的信息：" << endl;
			cout << "\t\t\t\t\t书号:" << r.pt->item[r.i].book->key << endl;
			cout << "\t\t\t\t\t书名：" << r.pt->item[r.i].book->name << endl;
			cout << "\t\t\t\t\t作者：" << r.pt->item[r.i].book->author << endl;
			cout << "\t\t\t\t\t总库存：" << r.pt->item[r.i].book->inventory << endl;

			f1 << "此书的信息:\n书号:" << r.pt->item[r.i].book->key <<  "\n书名："<< r.pt->item[r.i].book->name <<endl;    //log
			f1 << "作者：" << r.pt->item[r.i].book->author << "\n总库存：" << r.pt->item[r.i].book->inventory << endl;    //log
			DeleteBTree(T, r.pt->item[r.i], r.pt, r.i);
			cout << "\t\t\t\t\t此书已被清出库存" << endl;
			cout << "\t\t\t\t\t凹入表打印书号:" << endl;
			f1 << "此书已被清出库存\n凹入表打印书号:" << endl;
			int count = 0;
			printBTree(T, count);
		}
		else {
			cout << "\t\t\t\t\t库存中没有此书！" << endl;
			f1 << "库存中没有此书！" << endl;
		}
			
	} while (k != 0);

}

//借阅图书
void borrow(BTree &T) {
	int k;
	result r;
	cout << "\t\t\t\t\t请输入借阅图书的书号:";
	cin >> k;
	f1 << "请输入借阅图书的书号:" << k << endl;
	SearchBTree(T, k, r);
	if (r.tag == 1) {
		int id;
		char date[10];
		if (r.pt->item[r.i].book->amount == 0) {
			cout << "\t\t\t\t\t这本书现存量为0！无法借阅！" << endl;
			f1 << "这本书现存量为0！无法借阅！" <<endl;
		}
		else {
			cout << "\t\t\t\t\t这本书现存量为 " << r.pt->item[r.i].book->amount << " 本" << endl;
			cout << "\t\t\t\t\t请输入你的id:";
			cin >> id;
			if (r.pt->item[r.i].book->record_item == NULL) {
				r.pt->item[r.i].book->record_item = new record;
				r.pt->item[r.i].book->record_item->reader_id = id;
				cout << "\t\t\t\t\t请输入借阅日期(20170101):";
				cin >> date;
				strcpy_s(r.pt->item[r.i].book->record_item->date, date);
				r.pt->item[r.i].book->record_item->next = NULL;
			}
			else {
				record *p, *q;
				q = r.pt->item[r.i].book->record_item;
				while (q->next != NULL) q = q->next;
				p = new record;
				p->reader_id = id;
				cout << "\t\t\t\t\t请输入借阅日期(20170101):";
				cin >> date;
				strcpy_s(p->date, date);
				q->next = p;
				p->next = NULL;
			}
			f1 << "这本书现存量为:" << r.pt->item[r.i].book->amount << " 本\n请输入你的id:" << id << "\n请输入借阅日期(20170101):"<< date << endl;
			r.pt->item[r.i].book->amount--;
		}			
	}
	else {
		cout << "\t\t\t\t\t库存中没有此书！" << endl;
		f1 << "库存中没有此书！" << endl;
	}
	

}
//归还图书
void return_book(BTree &T) {
	int k;
	result r;
	cout << "\t\t\t\t\t请输入归还图书的书号:";
	cin >> k;
	f1 << "请输入归还图书的书号:" << k << endl;
	SearchBTree(T, k, r);
	if (r.tag == 1) {
		r.pt->item[r.i].book->amount++;
	}
	else {
		cout << "\t\t\t\t\t库存中没有此书！" << endl;
		f1 << "库存中没有此书！" << endl;
	}
}
//预约借书
void reserve(BTree &T) {
	int k;
	result r;
	cout << "\t\t\t\t\t请输入预约图书的书号:";
	cin >> k;
	f1 << "请输入预约图书的书号:" << k << endl;
	SearchBTree(T, k, r);
	if (r.tag == 1) {
		if(r.pt->item[r.i].book->amount == 0)
			cout << "\t\t\t\t\t此图书现存量为0:";
		r.pt->item[r.i].book->amount++;
	}
	else {
		cout << "\t\t\t\t\t库存中没有此书！" << endl;
		f1 << "库存中没有此书！" << endl;
	}
}
//图书信息及借阅信息
void book_info(BTree T) {
	int k;
	cout << "\t\t\t\t\t请输入你想查询图书的信息:(0停止)" << endl;
	f1 << "请输入你想查询图书的信息:(0停止)" << endl;    //log
	do {
		result r;
		cout << "\t\t\t\t\t请输入图书的书号:";
		f1 << "请输入图书的书号: " << endl;    //log
		cin >> k;
		if (k == 0) break;
		SearchBTree(T, k, r);
		if (r.tag == 1) {
			cout << "\t\t\t\t\t此书的信息：" << endl;
			cout << "\t\t\t\t\t书号:" << r.pt->item[r.i].book->key << endl;
			cout << "\t\t\t\t\t书名：" << r.pt->item[r.i].book->name << endl;
			cout << "\t\t\t\t\t作者：" << r.pt->item[r.i].book->author << endl;
			cout << "\t\t\t\t\t总库存：" << r.pt->item[r.i].book->inventory << endl;
			cout << "\t\t\t\t\t现存量：" << r.pt->item[r.i].book->amount << endl;
			f1 << "此书的信息:\n书号:" << r.pt->item[r.i].book->key << "\n书名：" << r.pt->item[r.i].book->name << endl;    //log
			f1 << "作者：" << r.pt->item[r.i].book->author << "\n总库存：" << r.pt->item[r.i].book->inventory << endl;    //log
			//借阅信息
			record* p = r.pt->item[r.i].book->record_item;
			for (; p != NULL; p = p->next) {
				cout << "\n\t\t\t\t\t借阅信息如下:" << endl;
				cout << "\t\t\t\t\tid:" << p->reader_id << endl;
				cout << "\t\t\t\t\t借阅日期:" << p->date << endl;
				f1 << "借阅信息如下:\nid:" << p->reader_id << "\n借阅日期：" << p->date << endl;    //log
			}
		}
		else {
			cout << "\t\t\t\t\t库存中没有此书！" << endl;
			f1 << "库存中没有此书！" << endl;
		}

	} while (k != 0);
}