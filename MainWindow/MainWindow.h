#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#if _MSC_VER >= 1600        //解决中文乱码问题
#pragma execution_character_set("utf-8")
#endif

#include <QMainWindow>
#include<QLabel>
#include<QTextEdit>
#include<QTextBrowser>
#include<QFileDialog>
#include<QMessageBox>
#include<QString>

#include<QTextStream>
#include<QTextCodec>



//#include<string.h> 
#include<QToolBar>
//#include<QPushButton>
//#include<QToolButton>
#include<QStatusBar>
#include<QFont>

class QMenuBar;//前置声明
class QMenu;
class QToolBar;
class QAction;
class QStatusBar;
class QWidget;
class QString;


namespace Ui {
    class MainWindowClass;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = 0);       //显示构造函数
    ~MainWindow();
    void createMenuBar();
    void createToolBar();
    void createIn();
    void createOut();
    void createlabel();

    QLabel* Label1;
    QLabel* Label2;
    QTextEdit* text;
    QTextBrowser* browser;

private slots:
    void saveFile();
    void newFile();
    void openFile();
    void runFile();
    void clearFile();
  
   
private:
    Ui::MainWindowClass* ui;
    QAction* saveAct;
    QAction* runAct;
    QAction* newAct;
    QAction* openAct;
    QAction* clearAct;
};

#endif // MAINWINDOW_H