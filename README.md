# Library_Management_system
Curriculum design 数据结构课程设计---图书管理系统
## IDE
Visual Studio 2015
## 文件目录
在Visual Studio 2015中新建工程后会自动生成工程目录</br>
自行建立的只有三个文件
- Library_Management_system.cpp
- Library_Management_system.h
- main.cpp
## 实现过程
### 需求分析
实现图书管理基本业务活动，其中包括：对一本书的采编入库、清除库存、借阅和归还等等。试设计一个图书管理系统，将上述业务活动借助于计算机系统完成。
	基本要求如下：
- 每种书的登记内容至少包括书号、书名、著者、现存量和总库存量等五项。
- 作为演示系统，不必使用文件，全部数据可以都在内存存放。但是由于上述四项基本业务活动都是通过书号(即关键字)进行的，所以要用B树(2-3树)对书号建立索引，以获得高效率。
- 系统应实现的操作及其功能定义如下：
  - 采编入库：新购入一种书，经分类和确定书号之后登记到图书账目中去。如果这种书在帐中已有，则只将总库存量增加。
  - 清除库存：某种书已无保留价值，将它从图书账目中注销。
  - 借阅：如果一种书的现存量大于零，则借出一本，登记借阅者的图书证号和归还期限。
  - 归还：注销对借阅者的登记，改变该书的现存量。
  - 显示：以凹入表的形式显示B树。这个操作是为了调试和维护的目的而设置的。</br>
实现的附加要求
- 将一次会话过程(即程序一次运行)中的全部人机对话记入一个日志文件“log”中去。
- 增加列出某种书状态的操作。状态信息除了包括这种书记录的全部信息外还包括最早到期(包括已逾期)的借阅者证号，日期可用整数实现，以求简化。
### 本程序中数据类型：
- 存储图书信息的结构体：包含书号、书名、著者、现存量和总库存量，还有指向借阅记录的指针。
- 借阅记录的结构体：包含读者的id号(整型)，和借阅日期(char类型数组)。
- B树及B树节点的结构体：包含当前节点关键字数组，当前节点存储的关键字数，直线双亲结点的指针和孩子节点的指针数组。
- 查找结果的结构体：包含指向找到的节点的指针，在节点中关键字位序，还有一个标记存储是否查找成功的结果。
## 参考资料
- 《数据结构(C语言版)》 严蔚敏 吴伟民
- 《数据结构》吴伟民
## 补充
此课程设计完成于2018.01.10
