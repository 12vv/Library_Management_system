#include<iostream>
using namespace std;
#include<stdio.h>
#include<stdlib.h>
#include "LM_system.h"

//B数的删除
//删除B树上p节点的第i个关键字
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
//		for (j = 0; q->ptr[j] != p; j++);      //找到此结点在父节点中的位置
//		//情况1:此结点右兄弟为3-node,即含有两个关键字
//		if ((j + 1) <= q->keynum && q->ptr[j + 1]->keynum == 2) {
//			b = q->ptr[j + 1];
//			p->item[1] = q->item[j + 1];        //父节点中取一个
//			q->item[j + 1] = b->item[1];        //右兄弟中一个上移
//			for (k = 1; k <= b->keynum; k++) {
//				b->item[k] = b->item[k + 1];
//				b->ptr[k - 1] = b->ptr[k];
//			}
//			p->keynum++;
//			b->keynum--;
//		}
//		//情况2:此结点左兄弟为3-node,即含有两个关键字
//		else if (j > 0 && q->ptr[j - 1]->keynum == 2) {
//			b = q->ptr[j - 1];
//			p->item[1] = q->item[j];                //父节点中取一个
//			q->item[j] = b->item[b->keynum];        //右兄弟中一个上移
//			p->keynum++;
//			b->keynum--;
//		}
//		else
//			return ERROR;
//		for (k = 0; k <= p->keynum; k++)
//			if (p->ptr[k]) p->ptr[k]->parent = p;  // 刷新q的子结点的双亲指针  
//		return SUCCESS;
//	}
//}

//删除后调整B树
//void Restore(BTree &p, int i) {
//	BTree q = p, b = NULL;
//	int j, k;
//	if (q->parent == NULL) return;
//	else {
//		q = p->parent;
//		for (j = 0; q->ptr[j] != p; j++);      //找到此结点在父节点中的位置
//		//情况1:此结点右兄弟为3-node,即含有两个关键字
//		if ((j + 1) <= q->keynum && q->ptr[j + 1]->keynum == 2) {
//			b = q->ptr[j + 1];			
//			p->item[1] = q->item[j + 1];        //父节点中取一个
//			q->item[j + 1] = b->item[1];        //右兄弟中一个上移
//			//p->ptr[1] = b->ptr[0];              //旁边指针跟着移动
//			for (k = 1; k <= b->keynum; k++) {
//				b->item[k] = b->item[k+1];
//				b->ptr[k-1] = b->ptr[k];
//			}
//			p->keynum++;
//			b->keynum--;
//		}
//		//情况2:此结点左兄弟为3-node,即含有两个关键字
//		else if (j > 0 && q->ptr[j - 1]->keynum == 2) {
//			b = q->ptr[j - 1];
//			p->item[1] = q->item[j];                //父节点中取一个
//			q->item[j] = b->item[b->keynum];        //右兄弟中一个上移
//			//q->ptr[j] = b->ptr[b->keynum];          //旁边指针跟着移动
//			p->keynum++;
//			b->keynum--;
//		}//情况3:此结点存在左右兄弟均为2-node,即含有一个关键字
//		else {
//			//情况a:此结点有右兄弟2-node
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
//			//情况b:此结点有左兄弟2-node
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
//				cout << "什么情况？！" << endl;
//				return;
//			}
//			q->keynum--;
//			b->keynum++;
//		}
//
//		for (i = 0; i <= b->keynum; i++)
//			if (b->ptr[i] != NULL) b->ptr[i]->parent = b;  // 刷新b的子结点的双亲指针 
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

//B数的删除
//删除B树上p节点的第i个关键字
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