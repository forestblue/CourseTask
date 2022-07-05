#include "MainWindow.h"
#include "ui_mainwindow.h"
#include<stdio.h>
#include<stdlib.h> 
#include<ctype.h> 
#include<String>
#define MAX 256// 标识符的最大长度符
#define MAXSIZE 70// 关键字和特殊符号表长度 
#define NUM 30// 表中存储的标识符和常数的容量

typedef struct Key_word{   // 关键字和特殊符号表结构
    char keyWord[MAX];      // 单词符号 
    int keyNum;             //种别码 
   // char keySign[MAX];      //助记符 
    char keyValue[MAX];     //内码值 
}Key_word;                 

int line ;		 // 扫描字符的行号 
int error ;       // 错误统计数
QString str1;        //源程序字符串
QString str2;        //单词的属性字<种别码，内码/指针> 标识符表和常数表
int index ;       //处理字符str1的下标
char ch;             //处理的字符

char token[MAX];    //字符组，存放构成单词符号的字符串  //未清0
char id[NUM][MAX];  // 标识符 
char consts[NUM][MAX];  // 常数 
char lownum[NUM][MAX] = { "0","1","2","3","4","5","6","7","8","9", "10","11","12",
                        "13","14","15","16","17","18","19" "20","21","22","23",
                        "24","25","26","27","28","29" }; //自定义表

Key_word Key[MAXSIZE] = {          
{"int",	1,"-"},
{"char",2,"-"},
{"float",3,"-"},
{"void",4,"-"},
{"do",5,"-"},
{"for",6,"-"},
{"if",7,"-"},
{"else",8,"-" },
{"then"	,9,"-"},
{"while",10,"-"},
{"switch",11,"-"},
{"break",12,"-"},
{"case",13,"-"},
{"return",14,"-"},
{"goto", 15, "-"},
{"default",16,"-"},
{"continue"	,17,"-"},
{"struct",18,"-"},
{"enum",19,"-"},
{"sizeof",20,"-"},
{"+",21,"-"},
{"-",22,"-"},
{"*",23,"-"},
{"/",24,"-"},
{"%",25,"-"},
{"++",26,"-" },
{"--",27,"-"},
{"==",28,"-"},
{"!=", 29,"-"},
{">",30,"-"},
{"<",31,"-"},
{">",32,"-"},
{"<=", 33,"-"},
{"&&", 34,"-"},
{"||" ,35,"-"},
{"!",36,"-"},
{"=",37,"-"},
{"+=",38,"-"},
{"-=", 39 ,"-"},
{"*=",40,"-"},
{"/=",41,"-"},
{"%=",42,"-"},
{"<<=", 43,"-"},
{">>=", 44,"-"},
{"&=", 45,"-"},
{"^=", 46,"-"},
{"|=",47,"-"},
{"&", 48,"-"},
{"|", 49,"-"},
{"^",50,"-"},
{"~",51,"-"},
{"~-",52,"-"},
{"<<",52,"-"},
{">>", 53,"-"},
{",",54,"-"},
{".",55,"-"	},
{"->",56,"-"},
{"[",57,"-"},
{"]",58,"-"},
{"(",59,"-"},
{")",60,"-"},
{";",61,"-"},
{":",62,"-"},
{"{",63,"-"},
{"}",64,"-"},
{" ", 65," "},	  //标识符 
{" ",66," "},	   //常数
{"//",67,"-" },
{"/*",68,"-"}
};

void Output(int n, int m);
int Is_alpha();
int Is_number();
void Is_anotation(int to);
void Is_other(int to);
void Scanner();
void move();
void writefile();
bool is_letter();

MainWindow::MainWindow(QWidget* parent) : 
    QMainWindow(parent),        //显式调用基类的构造函数
    ui(new Ui::MainWindowClass)      //初始化ui指针 //起初new UI::MainWindow报错是未定义的类型，查找相关资料后，原来在VS中uimainwindow.h中的类名会加上后缀Class_
{
    ui->setupUi(this);
    this->setWindowTitle("Lexical Analyzer");         //标题
    this->setFixedSize(1600, 1000);                   //窗口大小
    this->setWindowIcon(QIcon("icon.png"));           //图标
    this->setStyleSheet("background-color:MistyRose"); //背景颜色：粉色

    createMenuBar();        //菜单栏
    createToolBar();        //工具栏
    createlabel();          //文本提示栏
    createIn();             //文本编辑栏
    createOut();            //输出显示栏
}
MainWindow::~MainWindow()
{
    delete ui;
}

void  MainWindow::clearFile() {
    if (QMessageBox::question(this, "Tip：", "您确定清空吗？") == QMessageBox::Yes)
        this->text->clear();
}
void MainWindow::newFile() {                                        //新建文件的slot
    if (QMessageBox::question(this, "Tip：", "您是否需要保存当前文件？") == QMessageBox::Yes) 
        this->saveFile();
        this->text->clear();
    }
void MainWindow::openFile() {                                //打开文件的slot
    QString filepath = QFileDialog::getOpenFileName(this);
    QFile f(filepath);
    if (f.open(QIODevice::ReadWrite)) {
        QTextStream t(&f);
        t.setCodec(QTextCodec::codecForName("UTF-8"));      //文件转码，防止出现中文乱码问题
        text->setText(t.readAll());
    }
    else
        QMessageBox::warning(this, "警告", "文件打开失败");
    f.close();
}

void MainWindow::saveFile() {                             //文件保存的slot
    QString filePath = QFileDialog::getSaveFileName(this);//得到文件保存路径
    QString data = text->toPlainText();                    //toPlainText为纯文本
    QFile f(filePath);
    if (f.open(QIODevice::ReadWrite)) {
        QTextStream t(&f);
        t << data;
    }
    else {
        QMessageBox::warning(this, "警告", "文件保存失败");//MessageBox为消息对话框
    }
    f.close();
}

//--------------------------------------工具栏-------------------------------------------------

void MainWindow::createlabel() {                        //两个提示标签，告诉用户在哪里输入，哪里输出
    QLabel* Label1 = new QLabel("请输入源程序：", this);
    QLabel* Label2 = new QLabel("词法分析结果：", this);
    QFont font;

    font.setPointSize(14);

    Label1->setFont(font);
    Label2->setFont(font);
    Label1->setStyleSheet("color:Blue;");
    Label2->setStyleSheet("color:Blue;");
    Label1->resize(300, 50);
    Label2->resize(300, 50);
    Label1->move(100, 100);
    Label2->move(850, 100);
}

void MainWindow::createMenuBar() {                      //菜单栏，包括"文件"和"运行"两个可选项
    QMenuBar* menuBar = this->menuBar();
    QMenu* fileMenu = menuBar->addMenu("File(F)");  
    QMenu* runMenu = menuBar->addMenu("Run(R)");

    this->saveAct = fileMenu->addAction("Save(L)");       //保存
    this->newAct = fileMenu->addAction("New(N)");         //新建
    this-> openAct = fileMenu->addAction("Open(O)");       //打开
    this-> clearAct = fileMenu->addAction("Clear(C)");     //清空
    this-> runAct = runMenu->addAction("Run(R)");          //运行
  
    connect(saveAct, &QAction::triggered, this, &MainWindow::saveFile);
    connect(newAct, &QAction::triggered, this, &MainWindow::newFile);
    connect(openAct, &QAction::triggered, this, &MainWindow::openFile);
    connect(clearAct, &QAction::triggered, this, &MainWindow::clearFile);
    connect(runAct, &QAction::triggered, this, &MainWindow::runFile);

   
}

void MainWindow::createToolBar() {         //工具栏，5个操作
    QToolBar* toolBar = this->addToolBar("toolBar");
    toolBar->addAction(newAct);          //新建
    toolBar->setStyleSheet("background-color:PeachPuff;");
    QToolBar* toolBar1 = this->addToolBar("toolBar1");
    toolBar1->addAction(saveAct);           //保存
    toolBar1->setStyleSheet("background-color:Violet;");
    QToolBar* toolBar2 = this->addToolBar("toolBar2");
    toolBar2->addAction(openAct);           //打开
    toolBar2->setStyleSheet("background-color:lavender;");
    toolBar->addSeparator();
    QToolBar* toolBar3 = this->addToolBar("toolBar3");
    toolBar3->addAction(runAct);           //运行
    toolBar3->setStyleSheet("background-color:Skyblue;");
 
   QToolBar* toolBar4 = this->addToolBar("toolBar4");
   toolBar4->addAction(clearAct);           //清空
     toolBar4->setStyleSheet("background-color:LightGreen;");
}


void MainWindow::createIn(){                 //左侧文本编辑栏
    this->text=new QTextEdit(this);
    this->text->resize(700,700);
    text->move(50, 200);
}
void MainWindow::createOut() {               //右侧结果输出栏
    this->browser = new QTextBrowser(this);
    this->browser->resize(700, 700);
    browser->move(850, 200);
}

void MainWindow::runFile()              //处理文本输入框text中的内容str1，将分析结果存储在str2中，输入至浏览框browser，并且写到文件"result.txt"中
{
    line = 1;		 
    error=0;         
    index=0;    
    memset(id, '\0', sizeof(id));               //每次运行，清空上次的标识符表
    memset(consts, '\0', sizeof(consts));       //每次运行，清空上次的常数表
  
    str1 = text->toPlainText();     //获得文本框text中的内容存储在字符串str1中
    ch = str1.at(index).unicode();
    if (str1 != NULL) {
        str2 = "词法分析结果如下：\n";  //存储结果的字符串str2初始化
        Scanner();
        str2.append("\n标识符表 \n");    //id

        for (int i = 0; i < NUM; i++)
        {
            if (id[i][0] != NULL) {
                str2.append("<");
                str2.append(QString::number(i, 10));
                str2.append(",");
                str2.append(id[i]);
                str2.append(">\n");
            }
        }
        str2.append("\n\n常数表:\n");

        for (int i = 0; i < NUM; i++) {
            if (consts[i][0] != NULL) {
                str2.append("       <");
                str2.append(QString::number(i, 10));
                str2.append(",");
                str2.append(consts[i]);
                str2.append(">\n");
            }
        }
        str2.append("\n------------------------------------------------------\n");
    }
    else
        str2 = "请输入程序或打开文件！！！\n";

    this->browser->insertPlainText(str2);
   writefile();
}

void writefile() {
   std::string a=str2.toStdString();  //由QString类型转换为string类型，避免文件乱码
    FILE* fp;
    int i = 0;
    fp = fopen("result.txt", "w");
    while (i<a.length())
    {
        char ch = a[i++];
        fprintf(fp, "%c",ch);
}
   fclose(fp);
}
void move() {                           //字符串指针后移
    ch = str1.at(++index).unicode();  
}

int Is_alpha()                          //遇到字母得到的token，先分析关键字，再分析标识符
{
    for (int i = 0; i < 20; i++) {          // 20个关键字，循环匹配 
        if (strcmp(token, Key[i].keyWord) == 0) {      //str字符串相等时，返回0
            Output(i, -1);  return 0;                           //匹配到对应的关键字，传回关键字在数组中的位置，-1表示是关键字 
        }
    }
    for (int i = 0; i < NUM; i++) {          //标识符 Word[NUM][MAX]                                      
        if ((id[i][0] != NULL) && strcmp(token, id[i]) == 0) {      //如果标识符表i位置不为空，并且标识符表i中存在该标识符 
            Output(65, i);  return 0;
        }
        if (id[i][0] == NULL) {        //如果标识符表i位置为空 
            strcpy(id[i], token);      //将token中的值复制到标识符表id[i]中 
            Output(65, i); return 0;
        }
    }
}
                                      
int Is_number(){                               //consts[NUM][MAX] 
    for (int i = 0; i < NUM; i++){
        if ((consts[i][0] != NULL) && strcmp(token, consts[i]) == 0){             //如果数字表(consts[i]位置不为空，并且数字表中存在该数字 
            Output(66, i); return 0;
        }
        if (consts[i][0] == NULL){                       // 如果数字表(consts[i]位置为空
                                                         //将token中的值复制到数字表consts[i]中 
            strcpy(consts[i], token);
            Output(66, i);
            return 0;
        }
    }
    return 0;
}
                                                                          
void Is_anotation(int to){                                  //遇到'/'时,分析"/="，分析注释"//"和"/*",结束要指向下一个字符
    char pre;                                       //记录当前符号        
    if (index+1< str1.length()) 
        move();
    else {
        index++;
        Output(20, -1); return;
    }
    if (ch == '=') {                            // 是 "/=" 
        token[to++] = ch;
        Output(40, -1);
        if (++index == str1.length()) return;
        move();
    }
    else if (ch == '*') {        
        token[to++] = ch;
        Output(67, -1);             // 是 "/*" 
        do {
            if (index+1>=str1.length()){                      
                str2.append("\nLine ");
                str2.append(QString::number(line, 10));
                str2.append(" 注释错误: /*缺少结束注释*/\n");
                return;
            }
            pre = ch;
            if (index+1 <str1.length()) 
                move();
        } while (pre != '*' || ch != '/');
        if (index + 1 < str1.length())
            move();
        else index++;
    }
    else if (ch == '/'){            //是   "//" 
        token[to++] = ch;
        do {
            if (index+1>= str1.length()) {
                index++; break;
            }
            move();
        }while (ch != '\n');
        Output(66, -1);
    }
    else                           //   "/" 
        Output(23, -1);
}
                                                                               
void Is_other(int to) {                //结束时index应指向下一个字符
    char pre;
    switch (token[0]) {
    case '*':
    case '%':
    case '^':
        if (index + 1 >= str1.length()) {
            index++; break;
        }
        move();
        if (ch == '=') {
            token[to++] = ch;
            if (index + 1 >= str1.length())
                index++;
            else move();
        }
        break;
    case '<':
    case '>':
        pre = ch;
        if (index + 1 >= str1.length()) {
            index++; break;
        }
        move();
        if (ch == '=') {
            token[to++] = ch;
            if (index + 1 >= str1.length()) {
                index++; break;
            }
            move();
        }
        else if (ch == pre) {
            token[to++] = ch; {
                if (index + 1 >= str1.length())
                    index++;
                else
                    move();
                if (ch == '=') {
                    token[to++] = ch;
                    if (index + 1 >= str1.length())
                        index++;
                    else
                        move();
                }
            }

        }

        break;
    case '&':
    case '|':
    case '+':
        pre = ch;
        if (index + 1 >= str1.length()) {
            index++; break;
        }
        move();
        if (ch == '=' || ch == pre){
            token[to++] = ch;
        if (index + 1 >= str1.length())
            index++;
        else
            move();
    }

        break;

    case '-':
        pre = ch;
        if (index + 1 >= str1.length()) {
            index++; break;
            if (index >= str1.length()) {
                index++; break;
            }
            move();
        }
        move();
        if (ch == '=' || ch == pre || ch == '>') {
            token[to++] = ch;
            if (index + 1 >= str1.length())
                index++;
            else
                move();
        
        }

        break;
        default:
            if (index >= str1.length()) 
                index++; 
            else move();
            
    
    }
    for (int i = 20; i <= 68; i++) {      //匹配字符表是否存在该字符 
        if (i == 68) {                     //如果不存在，输出非法字符 
            error++;
            str2.append("\nLine  ");
            str2.append(QString::number(line, 10));
            str2.append(" error:字符错误 ");
            str2.append(token);
            str2.append("非法\n");
        }
        if (strcmp(token, Key[i].keyWord) == 0) {
            Output(i, -1);          //-1表示特殊字符 
            break;
        }
    }
}
bool Is_letter(){                       //判断当前字符是否是字母或下划线
    return (((ch >= 'a') && (ch <= 'z')) || (ch >= 'A') && (ch <= 'Z') || ch == '_');
}
void Output(int n, int m){
    str2.append(token);
    str2.append("       <");
    str2.append(QString::number(Key[n].keyNum, 10)); //<class,value>
    str2.append(",");
    if (m == -1)                             //m==-1,关键字或特定符号
        str2.append(Key[n].keyValue);                      
    else  str2.append(lownum[m]);// 下标     /m!=-1,标识符,存入标识符在自定义表中对应的下标的值（Key[n].keySign，lownum[m]
    str2.append(">\n");
}
void Scanner(){
    while(index<str1.length()){                //超出范围的数组访问  无限循环
        int  to = 0;
        memset(token, '\0', sizeof(token));
       while (ch == ' ' || ch == '\t' || ch == '\n'|| ch == '\r') {      // 词法分析器遇到空格时,后移，判断是否换行，统计行数
           if (ch == '\n')
               line++;
           if (index + 1 == str1.length()) return;
                move();
         }
       if (Is_letter()) {
           do {
               token[to++] = ch;    
               if (index + 1 == str1.length()) { Is_alpha();return ; }
               move();
           } while ((Is_letter() || isdigit(ch)));           //当ch为非字母或数字时，终止循环 
            Is_alpha();     
        }                                                       
       else if (isdigit(ch)) {            //数字
           while (isdigit(ch)) {
               token[to++] = ch;
               if (index + 1 == str1.length()) { Is_number(); return; }
               move();
           }
           if (ch == '.')                  //如果下一位为'.' ,则为实数
           {
               token[to++] = ch;
               if (index + 1< str1.length()) move();
               while (isdigit(ch)) {
                   token[to++] = ch;
                   if (index + 1 == str1.length()) { Is_number(); return; }
                   move();
               }
           }
           Is_number();
       }
        else if (ch == '/'){                   //区分 "/=" , "/*" 
           token[to++] = ch;
            Is_anotation(to);
        }
      else{           
           token[to++] = ch;
             Is_other(to);
      }
    }
}
    