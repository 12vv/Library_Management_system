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

//�ؼ��ֽṹ
typedef struct {
	KeyType key;             //�ؼ���
	struct book_item* book;  //ָ��洢ͼ����Ϣ
}RcdType;

//B��
typedef struct BTNode{
	int keynum;                 //��㵱ǰ�ؼ�����
	RcdType item[m+1];          //�ؼ������飬item[0]δ��
	struct BTNode *parent;      //˫�׽��ָ��
	struct BTNode *ptr[m + 1];  //���ӽ��ָ������
	//
}BTNode, *BTree;            

//ͼ����Ϣ
typedef struct book_item {
	KeyType key;        //���
	char name[10];      //����
	char author[10];    //����
	int inventory;      //�ܿ��
	int amount;         //ͼ���ִ���
	struct record *record_item;     //ָ����ļ�¼
};

//���ļ�¼
typedef struct record {
	int reader_id;
	char date[10];
	struct record* next;    //ָ����һ����¼
};

//���ҽ��
typedef struct {
	BTree pt;         //�����ҵ��Ľ��
	int i;            //1<=i<=m,�ڽ���еĹؼ���λ��
	int tag;          //���ҳɹ�ֵΪ1������ʧ��ֵΪ0
}result;


/*********************��B����صĲ���**********************/
//B���Ĳ���
void SearchBTree(BTree t, int k, result &r);
//��p->item[1..p->keynum].key����k
int Search(BTree p, int k);
//B���Ĳ���
void InsertBTree(BTree &t, RcdType k, BTree q, int i);
//B����ɾ��
//void DeleteBTree(BTree &p, int i);
Status  DeleteBTree(BTree &T, RcdType k, BTree &q, int i);
//�������ʾB���ؼ���
void printBTree(BTree t,int &count);

//�ڲ�����
//�����µĸ��ڵ�
void newRoot(BTree &t, BTree p, RcdType x, BTree ap);
//��q�����ѳ�������㣬ǰһ�뱣����ԭʼ�㣬��һ������ap��ָ�½��
void split(BTree &q, int s, BTree &ap);
//�ؼ���x�ͽ��ap�ֱ����q->key[i].key��q->ptr[i]
void Insert(BTree &q, int i, RcdType x, BTree ap);
//����ɾ��㲻��Ҷ�ӽ�㣬�Һ�������ɾ���
void Successor(BTree &p, int i);
//�ӽڵ�p��ɾ��item[i]
void Remove(BTree &p, int i);
//ɾ�������B��
//void Restore(BTree &p, int i);
//
Status borrowNode(BTree &q);
void Combine(BTree &q);
//������
void test(BTree &t);

/*********************��ϵͳ��صĲ���***********************/
//��ӭ����(���ڿ�ʼʱ����)
void welcome();
//��ʾ���˵�
void menu();
//�ɱ����
void stockIn(BTree &T);
//������
void stockOut(BTree &T);
//����ͼ��
void borrow(BTree &T);
//�黹ͼ��
void return_book(BTree &T);
//ԤԼ����
void reserve(BTree &T);
//ͼ����Ϣ
void book_info(BTree T);
//�˳�����
void out();