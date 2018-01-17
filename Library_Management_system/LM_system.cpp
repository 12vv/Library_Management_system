#include<fstream>
#include<iomanip>
#include<iostream>
using namespace std;
#include<stdio.h>
#include<stdlib.h>
#include "LM_system.h"

/*********************��B����صĲ���**********************/
//B���Ĳ��ң���m��B���ϲ���k����r����result
//�����ҳɹ���tag=1��ָ��pt��ָ���ĵ�i��item��keyΪk
//����tag=0����k��itemӦ��λ��pt��ָ����е�i-1���͵�i��item֮��
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

//��p->item[1..p->keynum].key����k
int Search(BTree p, int k) {
	int i = 1;
	while (p->item[i].key < k && i <= p->keynum) i++;
	return i;
}

//B���Ĳ���
//��B��t��q����item[i-1]��item[i]֮�����
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

//�ڲ�����
//�����µĸ��ڵ�(���1�����������2��ԭ������Ϊp��ap)
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

//��q�����ѳ�������㣬ǰһ�뱣����ԭʼ�㣬��һ������ap��ָ�½��
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

//�ؼ���x�ͽ��ap�ֱ����q->item[i].key��q->ptr[i]
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

//�������ʾB���ؼ���
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

//����ɾ��㲻��Ҷ�ӽ�㣬�Һ�������ɾ���
void Successor(BTree &p, int i) {
	BTree q = p;
	p = p->ptr[i];
	while (p->ptr[0] != NULL) p = p->ptr[0];
	q->item[i] = p->item[1];
}

//�ӽڵ�p��ɾ��item[i]
void Remove(BTree &p, int i) {
	for (; i < p->keynum; i++) {
		p->item[i] = p->item[i + 1];
	}
	p->keynum--;
}

Status borrowNode(BTree &q) {
	int i;
	BTree p = q->parent, b = NULL;                   // pָ��q��˫�׽��  
	for (i = 0; p->ptr[i] != q; i++);            // ����q��˫��p������λ��  
	if (i >= 0 && i + 1 <= p->keynum && p->ptr[i + 1]->keynum > (m - 1) / 2)
	{                                       // ��q�����ֵܹؼ��ָ�������(m-1)/2  
		b = p->ptr[i + 1];                       // bָ�����ֵܽ��  
		q->ptr[1] = b->ptr[0];                    // ����ָ��ҲҪͬ���ƶ�  
		q->item[1] = p->item[i + 1];                // �Ӹ��ڵ���i+1���ؼ���  		
		p->item[i + 1] = b->item[1];                // b��һ���ؼ������Ƶ����ڵ�  		
		for (i = 1; i <= b->keynum; i++)          // b��һ���ؼ������ƣ����ʣ���¼ǰ��һλ  
		{
			b->item[i] = b->item[i + 1];			
			b->ptr[i - 1] = b->ptr[i];
		}
	}
	else if (i > 0 && p->ptr[i - 1]->keynum > (m - 1) / 2)
	{                                       // ��q�����ֵܹؼ��ָ�����Լ(m-1)/2  
		b = p->ptr[i - 1];                       // bָ�����ֵܽ��  
		q->ptr[1] = q->ptr[0];
		q->ptr[0] = b->ptr[b->keynum];
		q->item[1] = p->item[i];                    // �Ӹ��ڵ���i���ؼ���  		
		p->item[i] = b->item[b->keynum];         // ��b���һ���ؼ������Ƶ����ڵ�  
		
	}
	else return ERROR;                          // �޹ؼ��ִ���(m-1)/2���ֵ�  
	q->keynum++;
	b->keynum--;
	for (i = 0; i <= q->keynum; i++)
		if (q->ptr[i]) q->ptr[i]->parent = q;  // ˢ��q���ӽ���˫��ָ��  
	return SUCCESS;
}

void Combine(BTree &q) {
	int i, j;
	BTree p = q->parent, b = NULL;//pָ��q�ĸ���  
	for (i = 0; p->ptr[i] != q; i++);//����q�ڸ���p�е�����λ��  
	if (i == 0) {//��Ϊ0������ϲ�Ϊ�ֵܵĵ�һ���ؼ���  
		b = p->ptr[i + 1];
		for (j = b->keynum; j >= 0; j--) {//��b�Ĺؼ��ֺͼ�¼����һλ  
			b->item[j + 1] = b->item[j];
			b->ptr[j + 1] = b->ptr[j];
		}
		b->ptr[0] = q->ptr[0];//�ϲ�  
		b->item[1] = p->item[1];
	}
	else if (i > 0) {//��q�ڸ��׵�����λ�ô���0����ϲ�Ϊ�ֵ�b�����һ���ؼ���  
		b = p->ptr[i - 1];
		b->item[b->keynum + 1] = p->item[i];//�ϲ�  
		b->ptr[b->keynum + 1] = q->ptr[0];
	}
	if (i == 0 || i == 1)//��iΪ0��1���轫�����p�ؼ���ǰ��һλ  
		for (; i < p->keynum; i++) {
			p->item[i] = p->item[i + 1];
			p->ptr[i] = p->ptr[i + 1];
		}
	p->keynum--;
	b->keynum++;
	delete(q);
	q = b;   //qָ���޸ĵ��ֵܽ��  
	for (i = 0; i <= b->keynum; i++)
		if (b->ptr[i]) b->ptr[i]->parent = b;//ˢ��b���ӽ���˫��ָ��
}
//*************************************************************************************************************//
Status  DeleteBTree(BTree &T, RcdType k, BTree &q, int i)
// ��m��B��T��ɾ��q����Ϲؼ���k��������SUCCESS��  
{
	int finished = FALSE;
		if (q->ptr[0]) Successor(q, i);          // ��q���������գ�(�ǵײ���)  
												 // �������̴�֮������qָ�������ڽ��  
		Remove(q, i);                              // ɾ��q��ָ�����е�i���ؼ��ּ���¼  
		if (q->keynum >= (m - 1) / 2 || !q->parent)        // ��ɾ����ؼ��ָ�����С��(m-1)/2��q�Ǹ��ڵ�  
		{
			finished = TRUE;                    // ɾ�����  
			if (q->keynum == 0) T = NULL;        // ��q�Ĺؼ��ָ���Ϊ0 ����Ϊ����  
		}
		while (!finished)
		{
			if (borrowNode(q) == SUCCESS)  finished = TRUE;    // ��q�������ֵܽ��ؼ��ִ���(m-1)/2,��Ӹ�  
												// �ֵܽ������һ����󣨻���С���ؼ��ֵ�  
												// ���ڵ㣬�Ӹ��ڵ��һ�ؼ��ֵ�q  
			else {                              // ��q�����ֵܹؼ��ָ��������ک�m /2��-1  
				Combine(q); // ��q�е�ʣ�ಿ�ֺ�˫���е���عؼ��ֺϲ���q��һ���ֵ���  
				q = q->parent;                   // ���˫��  
				if (q == T && T->keynum == 0)    // ����ɾ���ĸ��ڵ��Ǹ�T��T�Ĺؼ��ָ���Ϊ0  
				{
					T = T->ptr[0];               // �¸�     
					T->parent = NULL;
					delete(q);                    // ɾ��ԭ˫�׽��  
					finished = TRUE;
				}
				else if (q->keynum >= m / 2) finished = TRUE;
			}                               // �ϲ���˫�׹ؼ��ָ���������(m-1)/2�����  
		}
	return SUCCESS;
}
//������
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

/*********************��ϵͳ��صĲ���***********************/
//��ӭ����(���ڿ�ʼʱ����)
void welcome() {
	cout << "\t\t                      �X�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�[" << endl;
	cout << "\t\t                      �U    ��ӭ����ͼ�����ϵͳ  �U" << endl;
	cout << "\t\t                      �^�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�a" << endl;
}
//��ʾ���˵�
void menu() {
	cout << "\n\n\t\t\t\t  *************************************" << endl;
	//cout << "\n\t\t\t\t-------------------------------------------" << endl;
	cout << "\n\t\t\t\t\t1.�ɱ����     2.������    \n\n\t\t\t\t\t3.����ͼ��     4.�黹ͼ��     \n\n\t\t\t\t\t5.ͼ����Ϣ     6.�˳�ϵͳ" << endl;
	//cout << "\n\t\t\t\t-------------------------------------------" << endl;
	cout << "\n\t\t\t\t  *************************************" << endl;
}
//�˳�����
void out() {
	cout << "\t\t                      �X�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�[" << endl;
	cout << "\t\t                      �U    ��ӭ�ٴ�ʹ�ã��ټ�    �U" << endl;
	cout << "\t\t                      �^�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�a" << endl;
}
//�ɱ����
void stockIn(BTree &T) {
	int k;	
	cout << "\t\t\t\t\t�������������ͼ�����Ϣ:(0ֹͣ)" << endl;
	f1 << "�������������ͼ�����Ϣ:(0ֹͣ)" << endl;    //log
	do {
		result r;
		cout << "\t\t\t\t\t���������:";
		f1 << "���������:";    //log
		cin >> k;
		f1 << k << endl;    //log
		if (k == 0) break;
		SearchBTree(T, k, r);
		char name[10];
		char author[10];
		int inventory, amount = 0;
		if (r.tag == 1) {
			cout << "\t\t\t\t\t�Ȿ���Ѿ����ڣ�ԭ���ܿ��Ϊ" << r.pt->item[r.i].book->inventory << "��" << endl;
			cout << "\t\t\t\t\t�������Ӵ���������:";
			f1 << "�Ȿ���Ѿ����ڣ�ԭ���ܿ��Ϊ" << r.pt->item[r.i].book->inventory << "��" << endl;    //log
			cin >> inventory;
			f1 << "�������Ӵ���������:" << inventory << endl;    //log
			r.pt->item[r.i].book->inventory += inventory;
			r.pt->item[r.i].book->amount += inventory;
			cout << "\t\t\t\t\t�����ܿ��Ϊ" << r.pt->item[r.i].book->inventory << "��" << endl;
			f1 << "�����ܿ��Ϊ:" << r.pt->item[r.i].book->inventory << "��" << endl;    //log
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

			
			cout << "\t\t\t\t\t����������:";			
			cin >> name;
			f1 << "����������:" << name << endl;    //log
			strcpy_s(tem->book->name, name);
			cout << "\t\t\t\t\t����������:";
			cin >> author;
			f1 << "����������:" << author << endl;    //log
			strcpy_s(tem->book->author, author);
			cout << "\t\t\t\t\t���������ͼ�������:";
			cin >> inventory;
			f1 << "�������������:" << inventory << endl;    //log
			tem->book->inventory = inventory;
			tem->book->amount = inventory;
			tem->book->record_item = NULL;
			InsertBTree(T, *tem, r.pt, r.i);
			cout << "\t\t\t\t\t������ӡ���:" << endl;
			f1 << "������ӡ���:" << endl;    //log
			int count = 0;
			printBTree(T, count);
		}
	} while (k != 0);
}
//������
void stockOut(BTree &T) {
	int k;

	do {

		result r;
		cout << "\t\t\t\t\t���������ͼ������(0ֹͣ):";
		f1 << "���������ͼ������: " << endl;    //log
		cin >> k;
		if (k == 0) break;
		SearchBTree(T, k, r);
		if (r.tag == 1) {			
			cout << "\t\t\t\t\t�������Ϣ��" << endl;
			cout << "\t\t\t\t\t���:" << r.pt->item[r.i].book->key << endl;
			cout << "\t\t\t\t\t������" << r.pt->item[r.i].book->name << endl;
			cout << "\t\t\t\t\t���ߣ�" << r.pt->item[r.i].book->author << endl;
			cout << "\t\t\t\t\t�ܿ�棺" << r.pt->item[r.i].book->inventory << endl;

			f1 << "�������Ϣ:\n���:" << r.pt->item[r.i].book->key <<  "\n������"<< r.pt->item[r.i].book->name <<endl;    //log
			f1 << "���ߣ�" << r.pt->item[r.i].book->author << "\n�ܿ�棺" << r.pt->item[r.i].book->inventory << endl;    //log
			DeleteBTree(T, r.pt->item[r.i], r.pt, r.i);
			cout << "\t\t\t\t\t�����ѱ�������" << endl;
			cout << "\t\t\t\t\t������ӡ���:" << endl;
			f1 << "�����ѱ�������\n������ӡ���:" << endl;
			int count = 0;
			printBTree(T, count);
		}
		else {
			cout << "\t\t\t\t\t�����û�д��飡" << endl;
			f1 << "�����û�д��飡" << endl;
		}
			
	} while (k != 0);

}

//����ͼ��
void borrow(BTree &T) {
	int k;
	result r;
	cout << "\t\t\t\t\t���������ͼ������:";
	cin >> k;
	f1 << "���������ͼ������:" << k << endl;
	SearchBTree(T, k, r);
	if (r.tag == 1) {
		int id;
		char date[10];
		if (r.pt->item[r.i].book->amount == 0) {
			cout << "\t\t\t\t\t�Ȿ���ִ���Ϊ0���޷����ģ�" << endl;
			f1 << "�Ȿ���ִ���Ϊ0���޷����ģ�" <<endl;
		}
		else {
			cout << "\t\t\t\t\t�Ȿ���ִ���Ϊ " << r.pt->item[r.i].book->amount << " ��" << endl;
			cout << "\t\t\t\t\t���������id:";
			cin >> id;
			if (r.pt->item[r.i].book->record_item == NULL) {
				r.pt->item[r.i].book->record_item = new record;
				r.pt->item[r.i].book->record_item->reader_id = id;
				cout << "\t\t\t\t\t�������������(20170101):";
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
				cout << "\t\t\t\t\t�������������(20170101):";
				cin >> date;
				strcpy_s(p->date, date);
				q->next = p;
				p->next = NULL;
			}
			f1 << "�Ȿ���ִ���Ϊ:" << r.pt->item[r.i].book->amount << " ��\n���������id:" << id << "\n�������������(20170101):"<< date << endl;
			r.pt->item[r.i].book->amount--;
		}			
	}
	else {
		cout << "\t\t\t\t\t�����û�д��飡" << endl;
		f1 << "�����û�д��飡" << endl;
	}
	

}
//�黹ͼ��
void return_book(BTree &T) {
	int k;
	result r;
	cout << "\t\t\t\t\t������黹ͼ������:";
	cin >> k;
	f1 << "������黹ͼ������:" << k << endl;
	SearchBTree(T, k, r);
	if (r.tag == 1) {
		r.pt->item[r.i].book->amount++;
	}
	else {
		cout << "\t\t\t\t\t�����û�д��飡" << endl;
		f1 << "�����û�д��飡" << endl;
	}
}
//ԤԼ����
void reserve(BTree &T) {
	int k;
	result r;
	cout << "\t\t\t\t\t������ԤԼͼ������:";
	cin >> k;
	f1 << "������ԤԼͼ������:" << k << endl;
	SearchBTree(T, k, r);
	if (r.tag == 1) {
		if(r.pt->item[r.i].book->amount == 0)
			cout << "\t\t\t\t\t��ͼ���ִ���Ϊ0:";
		r.pt->item[r.i].book->amount++;
	}
	else {
		cout << "\t\t\t\t\t�����û�д��飡" << endl;
		f1 << "�����û�д��飡" << endl;
	}
}
//ͼ����Ϣ��������Ϣ
void book_info(BTree T) {
	int k;
	cout << "\t\t\t\t\t�����������ѯͼ�����Ϣ:(0ֹͣ)" << endl;
	f1 << "�����������ѯͼ�����Ϣ:(0ֹͣ)" << endl;    //log
	do {
		result r;
		cout << "\t\t\t\t\t������ͼ������:";
		f1 << "������ͼ������: " << endl;    //log
		cin >> k;
		if (k == 0) break;
		SearchBTree(T, k, r);
		if (r.tag == 1) {
			cout << "\t\t\t\t\t�������Ϣ��" << endl;
			cout << "\t\t\t\t\t���:" << r.pt->item[r.i].book->key << endl;
			cout << "\t\t\t\t\t������" << r.pt->item[r.i].book->name << endl;
			cout << "\t\t\t\t\t���ߣ�" << r.pt->item[r.i].book->author << endl;
			cout << "\t\t\t\t\t�ܿ�棺" << r.pt->item[r.i].book->inventory << endl;
			cout << "\t\t\t\t\t�ִ�����" << r.pt->item[r.i].book->amount << endl;
			f1 << "�������Ϣ:\n���:" << r.pt->item[r.i].book->key << "\n������" << r.pt->item[r.i].book->name << endl;    //log
			f1 << "���ߣ�" << r.pt->item[r.i].book->author << "\n�ܿ�棺" << r.pt->item[r.i].book->inventory << endl;    //log
			//������Ϣ
			record* p = r.pt->item[r.i].book->record_item;
			for (; p != NULL; p = p->next) {
				cout << "\n\t\t\t\t\t������Ϣ����:" << endl;
				cout << "\t\t\t\t\tid:" << p->reader_id << endl;
				cout << "\t\t\t\t\t��������:" << p->date << endl;
				f1 << "������Ϣ����:\nid:" << p->reader_id << "\n�������ڣ�" << p->date << endl;    //log
			}
		}
		else {
			cout << "\t\t\t\t\t�����û�д��飡" << endl;
			f1 << "�����û�д��飡" << endl;
		}

	} while (k != 0);
}