#include<fstream>
#include<iomanip>
#include<iostream>
using namespace std;
#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include "LM_system.h"

ofstream f1;

int main() {

	f1.open("log.txt");
	if (!f1.is_open())
		cout << "fail to open the file" << endl;
	//else {
	//	cout << "open the file successfully" << endl;
	//}

	int option;
	int k;
	result r;
	BTree T = NULL;

	welcome();	
	do {
		menu();
		cout << "\t\t\t\t\t>>>>>>请选择操作: ";
		f1 << ">>>>>>请选择操作: ";
		cin >> option;
		f1 << option << endl;
		if (option <= 0 || option > 6) {
			cout << "\t\t\t\t\t请输入正确操作序号1--6";
			continue;
		}
		switch (option) {
		case 1:stockIn(T); break;
		case 2:stockOut(T); break;
		case 3:borrow(T); break;
		case 4:return_book(T); break;
		case 5:book_info(T); break;
		case 6:out(); break;
		}
	} while (option != 6);

	//test
	//int temp;
	////test(T);	
	//int count = 0;
	////printBTree(T,count);
	//int a[] = { 35, 16, 18, 70, 5, 50, 22, 60, 13, 17, 12 , 45, 25, 42, 15, 90, 30, 7 };


	//	RcdType tem;
	//	//cout << "请输入你想要增加的节点:";
	//	//cin >> temp;
	//	for (int j = 0; j < 18; j++) {
	//		tem.key = a[j];
	//		SearchBTree(T, tem.key, r);

	//		if (r.tag == 0)
	//			InsertBTree(T, tem, r.pt, r.i);

	//	}
	//	count = 0;
	//	printBTree(T, count);		

	//do {
	//	cout << "请输入你想要删除的节点:";
	//	cin >> temp;
	//	tem.key = temp;
	//	SearchBTree(T, temp, r);
	//	if(r.tag == 1)
	//		DeleteBTree(T, tem, r.pt, r.i);
	//	count = 0;
	//	printBTree(T, count);
	//} while (temp != 0);

	f1.close();//关闭文件
	system("pause");
	return 0;
}