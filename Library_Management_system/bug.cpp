#include<iostream>
using namespace std;
#include<stdio.h>
#include<stdlib.h>
#include "LM_system.h"

//B����ɾ��
//ɾ��B����p�ڵ�ĵ�i���ؼ���
//void DeleteBTree(BTree &p, int i) {
//	if (p->ptr[i] != NULL) {
//		Successor(p, i);
//		DeleteBTree(p, 1);
//	}
//	else {
//		Remove(p, i);
//		if (p->keynum == 0)
//			Restore(p, i);
//	}
//}

//Status borrowNode(BTree &p) {
//	BTree q = p, b = NULL;
//	int j, k;
//	if (q->parent == NULL) return ERROR;
//	else {
//		q = p->parent;
//		for (j = 0; q->ptr[j] != p; j++);      //�ҵ��˽���ڸ��ڵ��е�λ��
//		//���1:�˽�����ֵ�Ϊ3-node,�����������ؼ���
//		if ((j + 1) <= q->keynum && q->ptr[j + 1]->keynum == 2) {
//			b = q->ptr[j + 1];
//			p->item[1] = q->item[j + 1];        //���ڵ���ȡһ��
//			q->item[j + 1] = b->item[1];        //���ֵ���һ������
//			for (k = 1; k <= b->keynum; k++) {
//				b->item[k] = b->item[k + 1];
//				b->ptr[k - 1] = b->ptr[k];
//			}
//			p->keynum++;
//			b->keynum--;
//		}
//		//���2:�˽�����ֵ�Ϊ3-node,�����������ؼ���
//		else if (j > 0 && q->ptr[j - 1]->keynum == 2) {
//			b = q->ptr[j - 1];
//			p->item[1] = q->item[j];                //���ڵ���ȡһ��
//			q->item[j] = b->item[b->keynum];        //���ֵ���һ������
//			p->keynum++;
//			b->keynum--;
//		}
//		else
//			return ERROR;
//		for (k = 0; k <= p->keynum; k++)
//			if (p->ptr[k]) p->ptr[k]->parent = p;  // ˢ��q���ӽ���˫��ָ��  
//		return SUCCESS;
//	}
//}

//ɾ�������B��
//void Restore(BTree &p, int i) {
//	BTree q = p, b = NULL;
//	int j, k;
//	if (q->parent == NULL) return;
//	else {
//		q = p->parent;
//		for (j = 0; q->ptr[j] != p; j++);      //�ҵ��˽���ڸ��ڵ��е�λ��
//		//���1:�˽�����ֵ�Ϊ3-node,�����������ؼ���
//		if ((j + 1) <= q->keynum && q->ptr[j + 1]->keynum == 2) {
//			b = q->ptr[j + 1];			
//			p->item[1] = q->item[j + 1];        //���ڵ���ȡһ��
//			q->item[j + 1] = b->item[1];        //���ֵ���һ������
//			//p->ptr[1] = b->ptr[0];              //�Ա�ָ������ƶ�
//			for (k = 1; k <= b->keynum; k++) {
//				b->item[k] = b->item[k+1];
//				b->ptr[k-1] = b->ptr[k];
//			}
//			p->keynum++;
//			b->keynum--;
//		}
//		//���2:�˽�����ֵ�Ϊ3-node,�����������ؼ���
//		else if (j > 0 && q->ptr[j - 1]->keynum == 2) {
//			b = q->ptr[j - 1];
//			p->item[1] = q->item[j];                //���ڵ���ȡһ��
//			q->item[j] = b->item[b->keynum];        //���ֵ���һ������
//			//q->ptr[j] = b->ptr[b->keynum];          //�Ա�ָ������ƶ�
//			p->keynum++;
//			b->keynum--;
//		}//���3:�˽����������ֵܾ�Ϊ2-node,������һ���ؼ���
//		else {
//			//���a:�˽�������ֵ�2-node
//			if ((j + 1) <= q->keynum && q->ptr[j + 1] != NULL) {
//				b = q->ptr[j + 1];
//				for (k = b->keynum; k >= 0; k--) {
//					b->item[k + 1] = b->item[k];
//					b->ptr[k + 1] = b->ptr[k];
//				}
//				b->item[1] = q->item[j+1];
//				for (k = 0; k < q->keynum; k++) {
//					q->ptr[k] = q->ptr[k + 1];
//					q->item[k+1] = q->item[k+2];
//				}				
//			}
//			//���b:�˽�������ֵ�2-node
//			else if (j > 0 && q->ptr[j - 1] != NULL) {
//				b = q->ptr[j - 1];
//				//for (j = 0; j <= b->keynum; j++) {
//				//	b->item[j + 1] = b->item[j];
//				//	b->ptr[j + 1] = b->ptr[j];
//				//}
//				b->item[2] = q->item[j];
//				b->ptr[2] = NULL;
//			}
//			else {
//				cout << "ʲô�������" << endl;
//				return;
//			}
//			q->keynum--;
//			b->keynum++;
//		}
//
//		for (i = 0; i <= b->keynum; i++)
//			if (b->ptr[i] != NULL) b->ptr[i]->parent = b;  // ˢ��b���ӽ���˫��ָ�� 
//
//
//		if (p->parent->keynum == 0) {
//			BTree s = p->parent;
//			for (int k = 1; k <= 3; k++) {
//				s->item[k] = b->item[k];
//				s->ptr[k-1] = NULL;
//			}
//			s->keynum = 2;
//			delete(p);
//		}
//	}
//}

//B����ɾ��
//ɾ��B����p�ڵ�ĵ�i���ؼ���
//void DeleteBTree(BTree &p, int i) {
//	if (p->ptr[i] != NULL) {
//		Successor(p, i);
//		DeleteBTree(p, 1);
//	}
//	else {
//		Remove(p, i);
//		if (p->keynum == 0)
//			Restore(p, i);
//	}
//}