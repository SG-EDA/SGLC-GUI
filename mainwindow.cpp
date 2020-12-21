#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include "script.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_setLine_triggered()
{
    //fix:需弹个输入框
}

void MainWindow::on_resetTrigger_triggered()
{
    s.getManager()->resetTri();
}

void MainWindow::on_run_triggered()
{
    s.getManager()->run(); //fix:需改输出，输出到文本框
}


void MainWindow::on_truthTable_triggered()
{
    s.getManager()->trueTable();
    //fix:需改输出，弹一个显示表的新窗口
}

void MainWindow::on_stateTruthTable_triggered()
{
    s.getManager()->trueTable(0,true);
    //fix:需改输出，弹一个显示表的新窗口
}

void MainWindow::on_run2_triggered()
{
    s.getManager()->middleVar();
    //fix:需改输出，画到图形框
}

void MainWindow::on_elementCount_triggered()
{
    s.getManager()->gateNum(); //fix:需改输出，输出到文本框
}

void MainWindow::on_lineComplexity_triggered()
{
    s.getManager()->multiplexing(); //fix:需改输出，输出到文本框
}

void MainWindow::on_closeCircuitScript_triggered()
{
    s.clear();
    //fix:清空文本框和图形框里的内容
}

void MainWindow::on_load_triggered()
{
    //fix:弹个浏览框让用户选文件
}
