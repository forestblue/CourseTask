#include "MainWindow.h"
#include "ui_mainwindow.h"
#include<stdio.h>
#include<stdlib.h> 
#include<ctype.h> 
#include<String>
#define MAX 256// ��ʶ������󳤶ȷ�
#define MAXSIZE 70// �ؼ��ֺ�������ű��� 
#define NUM 30// ���д洢�ı�ʶ���ͳ���������

typedef struct Key_word{   // �ؼ��ֺ�������ű�ṹ
    char keyWord[MAX];      // ���ʷ��� 
    int keyNum;             //�ֱ��� 
   // char keySign[MAX];      //���Ƿ� 
    char keyValue[MAX];     //����ֵ 
}Key_word;                 

int line ;		 // ɨ���ַ����к� 
int error ;       // ����ͳ����
QString str1;        //Դ�����ַ���
QString str2;        //���ʵ�������<�ֱ��룬����/ָ��> ��ʶ����ͳ�����
int index ;       //�����ַ�str1���±�
char ch;             //������ַ�

char token[MAX];    //�ַ��飬��Ź��ɵ��ʷ��ŵ��ַ���  //δ��0
char id[NUM][MAX];  // ��ʶ�� 
char consts[NUM][MAX];  // ���� 
char lownum[NUM][MAX] = { "0","1","2","3","4","5","6","7","8","9", "10","11","12",
                        "13","14","15","16","17","18","19" "20","21","22","23",
                        "24","25","26","27","28","29" }; //�Զ����

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
{" ", 65," "},	  //��ʶ�� 
{" ",66," "},	   //����
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
    QMainWindow(parent),        //��ʽ���û���Ĺ��캯��
    ui(new Ui::MainWindowClass)      //��ʼ��uiָ�� //���new UI::MainWindow������δ��������ͣ�����������Ϻ�ԭ����VS��uimainwindow.h�е���������Ϻ�׺Class_
{
    ui->setupUi(this);
    this->setWindowTitle("Lexical Analyzer");         //����
    this->setFixedSize(1600, 1000);                   //���ڴ�С
    this->setWindowIcon(QIcon("icon.png"));           //ͼ��
    this->setStyleSheet("background-color:MistyRose"); //������ɫ����ɫ

    createMenuBar();        //�˵���
    createToolBar();        //������
    createlabel();          //�ı���ʾ��
    createIn();             //�ı��༭��
    createOut();            //�����ʾ��
}
MainWindow::~MainWindow()
{
    delete ui;
}

void  MainWindow::clearFile() {
    if (QMessageBox::question(this, "Tip��", "��ȷ�������") == QMessageBox::Yes)
        this->text->clear();
}
void MainWindow::newFile() {                                        //�½��ļ���slot
    if (QMessageBox::question(this, "Tip��", "���Ƿ���Ҫ���浱ǰ�ļ���") == QMessageBox::Yes) 
        this->saveFile();
        this->text->clear();
    }
void MainWindow::openFile() {                                //���ļ���slot
    QString filepath = QFileDialog::getOpenFileName(this);
    QFile f(filepath);
    if (f.open(QIODevice::ReadWrite)) {
        QTextStream t(&f);
        t.setCodec(QTextCodec::codecForName("UTF-8"));      //�ļ�ת�룬��ֹ����������������
        text->setText(t.readAll());
    }
    else
        QMessageBox::warning(this, "����", "�ļ���ʧ��");
    f.close();
}

void MainWindow::saveFile() {                             //�ļ������slot
    QString filePath = QFileDialog::getSaveFileName(this);//�õ��ļ�����·��
    QString data = text->toPlainText();                    //toPlainTextΪ���ı�
    QFile f(filePath);
    if (f.open(QIODevice::ReadWrite)) {
        QTextStream t(&f);
        t << data;
    }
    else {
        QMessageBox::warning(this, "����", "�ļ�����ʧ��");//MessageBoxΪ��Ϣ�Ի���
    }
    f.close();
}

//--------------------------------------������-------------------------------------------------

void MainWindow::createlabel() {                        //������ʾ��ǩ�������û����������룬�������
    QLabel* Label1 = new QLabel("������Դ����", this);
    QLabel* Label2 = new QLabel("�ʷ����������", this);
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

void MainWindow::createMenuBar() {                      //�˵���������"�ļ�"��"����"������ѡ��
    QMenuBar* menuBar = this->menuBar();
    QMenu* fileMenu = menuBar->addMenu("File(F)");  
    QMenu* runMenu = menuBar->addMenu("Run(R)");

    this->saveAct = fileMenu->addAction("Save(L)");       //����
    this->newAct = fileMenu->addAction("New(N)");         //�½�
    this-> openAct = fileMenu->addAction("Open(O)");       //��
    this-> clearAct = fileMenu->addAction("Clear(C)");     //���
    this-> runAct = runMenu->addAction("Run(R)");          //����
  
    connect(saveAct, &QAction::triggered, this, &MainWindow::saveFile);
    connect(newAct, &QAction::triggered, this, &MainWindow::newFile);
    connect(openAct, &QAction::triggered, this, &MainWindow::openFile);
    connect(clearAct, &QAction::triggered, this, &MainWindow::clearFile);
    connect(runAct, &QAction::triggered, this, &MainWindow::runFile);

   
}

void MainWindow::createToolBar() {         //��������5������
    QToolBar* toolBar = this->addToolBar("toolBar");
    toolBar->addAction(newAct);          //�½�
    toolBar->setStyleSheet("background-color:PeachPuff;");
    QToolBar* toolBar1 = this->addToolBar("toolBar1");
    toolBar1->addAction(saveAct);           //����
    toolBar1->setStyleSheet("background-color:Violet;");
    QToolBar* toolBar2 = this->addToolBar("toolBar2");
    toolBar2->addAction(openAct);           //��
    toolBar2->setStyleSheet("background-color:lavender;");
    toolBar->addSeparator();
    QToolBar* toolBar3 = this->addToolBar("toolBar3");
    toolBar3->addAction(runAct);           //����
    toolBar3->setStyleSheet("background-color:Skyblue;");
 
   QToolBar* toolBar4 = this->addToolBar("toolBar4");
   toolBar4->addAction(clearAct);           //���
     toolBar4->setStyleSheet("background-color:LightGreen;");
}


void MainWindow::createIn(){                 //����ı��༭��
    this->text=new QTextEdit(this);
    this->text->resize(700,700);
    text->move(50, 200);
}
void MainWindow::createOut() {               //�Ҳ��������
    this->browser = new QTextBrowser(this);
    this->browser->resize(700, 700);
    browser->move(850, 200);
}

void MainWindow::runFile()              //�����ı������text�е�����str1������������洢��str2�У������������browser������д���ļ�"result.txt"��
{
    line = 1;		 
    error=0;         
    index=0;    
    memset(id, '\0', sizeof(id));               //ÿ�����У�����ϴεı�ʶ����
    memset(consts, '\0', sizeof(consts));       //ÿ�����У�����ϴεĳ�����
  
    str1 = text->toPlainText();     //����ı���text�е����ݴ洢���ַ���str1��
    ch = str1.at(index).unicode();
    if (str1 != NULL) {
        str2 = "�ʷ�����������£�\n";  //�洢������ַ���str2��ʼ��
        Scanner();
        str2.append("\n��ʶ���� \n");    //id

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
        str2.append("\n\n������:\n");

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
        str2 = "������������ļ�������\n";

    this->browser->insertPlainText(str2);
   writefile();
}

void writefile() {
   std::string a=str2.toStdString();  //��QString����ת��Ϊstring���ͣ������ļ�����
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
void move() {                           //�ַ���ָ�����
    ch = str1.at(++index).unicode();  
}

int Is_alpha()                          //������ĸ�õ���token���ȷ����ؼ��֣��ٷ�����ʶ��
{
    for (int i = 0; i < 20; i++) {          // 20���ؼ��֣�ѭ��ƥ�� 
        if (strcmp(token, Key[i].keyWord) == 0) {      //str�ַ������ʱ������0
            Output(i, -1);  return 0;                           //ƥ�䵽��Ӧ�Ĺؼ��֣����عؼ����������е�λ�ã�-1��ʾ�ǹؼ��� 
        }
    }
    for (int i = 0; i < NUM; i++) {          //��ʶ�� Word[NUM][MAX]                                      
        if ((id[i][0] != NULL) && strcmp(token, id[i]) == 0) {      //�����ʶ����iλ�ò�Ϊ�գ����ұ�ʶ����i�д��ڸñ�ʶ�� 
            Output(65, i);  return 0;
        }
        if (id[i][0] == NULL) {        //�����ʶ����iλ��Ϊ�� 
            strcpy(id[i], token);      //��token�е�ֵ���Ƶ���ʶ����id[i]�� 
            Output(65, i); return 0;
        }
    }
}
                                      
int Is_number(){                               //consts[NUM][MAX] 
    for (int i = 0; i < NUM; i++){
        if ((consts[i][0] != NULL) && strcmp(token, consts[i]) == 0){             //������ֱ�(consts[i]λ�ò�Ϊ�գ��������ֱ��д��ڸ����� 
            Output(66, i); return 0;
        }
        if (consts[i][0] == NULL){                       // ������ֱ�(consts[i]λ��Ϊ��
                                                         //��token�е�ֵ���Ƶ����ֱ�consts[i]�� 
            strcpy(consts[i], token);
            Output(66, i);
            return 0;
        }
    }
    return 0;
}
                                                                          
void Is_anotation(int to){                                  //����'/'ʱ,����"/="������ע��"//"��"/*",����Ҫָ����һ���ַ�
    char pre;                                       //��¼��ǰ����        
    if (index+1< str1.length()) 
        move();
    else {
        index++;
        Output(20, -1); return;
    }
    if (ch == '=') {                            // �� "/=" 
        token[to++] = ch;
        Output(40, -1);
        if (++index == str1.length()) return;
        move();
    }
    else if (ch == '*') {        
        token[to++] = ch;
        Output(67, -1);             // �� "/*" 
        do {
            if (index+1>=str1.length()){                      
                str2.append("\nLine ");
                str2.append(QString::number(line, 10));
                str2.append(" ע�ʹ���: /*ȱ�ٽ���ע��*/\n");
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
    else if (ch == '/'){            //��   "//" 
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
                                                                               
void Is_other(int to) {                //����ʱindexӦָ����һ���ַ�
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
    for (int i = 20; i <= 68; i++) {      //ƥ���ַ����Ƿ���ڸ��ַ� 
        if (i == 68) {                     //��������ڣ�����Ƿ��ַ� 
            error++;
            str2.append("\nLine  ");
            str2.append(QString::number(line, 10));
            str2.append(" error:�ַ����� ");
            str2.append(token);
            str2.append("�Ƿ�\n");
        }
        if (strcmp(token, Key[i].keyWord) == 0) {
            Output(i, -1);          //-1��ʾ�����ַ� 
            break;
        }
    }
}
bool Is_letter(){                       //�жϵ�ǰ�ַ��Ƿ�����ĸ���»���
    return (((ch >= 'a') && (ch <= 'z')) || (ch >= 'A') && (ch <= 'Z') || ch == '_');
}
void Output(int n, int m){
    str2.append(token);
    str2.append("       <");
    str2.append(QString::number(Key[n].keyNum, 10)); //<class,value>
    str2.append(",");
    if (m == -1)                             //m==-1,�ؼ��ֻ��ض�����
        str2.append(Key[n].keyValue);                      
    else  str2.append(lownum[m]);// �±�     /m!=-1,��ʶ��,�����ʶ�����Զ�����ж�Ӧ���±��ֵ��Key[n].keySign��lownum[m]
    str2.append(">\n");
}
void Scanner(){
    while(index<str1.length()){                //������Χ���������  ����ѭ��
        int  to = 0;
        memset(token, '\0', sizeof(token));
       while (ch == ' ' || ch == '\t' || ch == '\n'|| ch == '\r') {      // �ʷ������������ո�ʱ,���ƣ��ж��Ƿ��У�ͳ������
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
           } while ((Is_letter() || isdigit(ch)));           //��chΪ����ĸ������ʱ����ֹѭ�� 
            Is_alpha();     
        }                                                       
       else if (isdigit(ch)) {            //����
           while (isdigit(ch)) {
               token[to++] = ch;
               if (index + 1 == str1.length()) { Is_number(); return; }
               move();
           }
           if (ch == '.')                  //�����һλΪ'.' ,��Ϊʵ��
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
        else if (ch == '/'){                   //���� "/=" , "/*" 
           token[to++] = ch;
            Is_anotation(to);
        }
      else{           
           token[to++] = ch;
             Is_other(to);
      }
    }
}
    