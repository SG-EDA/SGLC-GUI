#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include "script.h"
#include <QLabel>
#include <QSpinBox>
#include <QFormLayout>
#include <QDialogButtonBox>
#include <QLineEdit>
#include <QGraphicsScene>
#include <QFileDialog>
#include <QTextStream>

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->Graph->setScene(new QGraphicsScene(0,0,1000,1000));
    ui->textEdit->setText("hello");
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_setLine_triggered()
{
    if(loaded==false)
        QMessageBox::information(NULL, "提示", "未加载电路脚本");
    else
    {
        QDialog dialog(this);
        QFormLayout form(&dialog);
        form.addRow(new QLabel("设置线路变量:"));
        // Value1
        QLineEdit *spinbox1 = new QLineEdit(this);
        form.addRow("变量名: ", spinbox1);
        // Value2
        QSpinBox *spinbox2 = new QSpinBox(&dialog);
        form.addRow("变量值: ", spinbox2);
        // Add Cancel and OK button
        QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
            Qt::Horizontal, &dialog);
        form.addRow(&buttonBox);
        QObject::connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
        QObject::connect(&buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));

        // Process when OK button is clicked
        if (dialog.exec() == QDialog::Accepted)
        {
            QString  name=spinbox1->text();
            QString  value=spinbox2->text();
            s.lineMap[name.toStdString()]->constVal=help::toint(value.toStdString());
            spinbox2->text();
        }
    }
}

void MainWindow::on_resetTrigger_triggered()
{
    if(loaded==false)
        QMessageBox::information(NULL, "提示", "未加载电路脚本");
    else
    s.getManager()->resetTri();
}

void MainWindow::on_run_triggered()
{
    if(loaded==false)
        QMessageBox::information(NULL, "提示", "未加载电路脚本");
    s.getManager()->run(); //fix:需改输出，输出到文本框
}


void MainWindow::on_truthTable_triggered()
{
    if(loaded==false)
        QMessageBox::information(NULL, "提示", "未加载电路脚本");
    s.getManager()->trueTable();
    //fix:需改输出，弹一个显示表的新窗口
}

void MainWindow::on_stateTruthTable_triggered()
{
    if(loaded==false)
        QMessageBox::information(NULL, "提示", "未加载电路脚本");
    s.getManager()->trueTable(0,true);
    //fix:需改输出，弹一个显示表的新窗口
}

void MainWindow::on_run2_triggered()
{
    if(loaded==false)
        QMessageBox::information(NULL, "提示", "未加载电路脚本");
    s.getManager()->middleVar();
    //fix:需改输出，画到图形框
}

void MainWindow::on_elementCount_triggered()
{
    if(loaded==false)
        QMessageBox::information(NULL, "提示", "未加载电路脚本");
    s.getManager()->gateNum(); //fix:需改输出，输出到文本框
}

void MainWindow::on_lineComplexity_triggered()
{
    if(loaded==false)
        QMessageBox::information(NULL, "提示", "未加载电路脚本");
    s.getManager()->multiplexing(); //fix:需改输出，输出到文本框
}

void MainWindow::on_closeCircuitScript_triggered()
{
    if(loaded==false)
        QMessageBox::information(NULL, "提示", "未加载电路脚本");
    s.clear();
    ui->textEdit->clear();
    ui->Graph->scene()->clear();
    //fix:清空文本框和图形框里的内容
}

QString ReadTXT(QString path)
{
    QFile f(path);
    f.open(QIODevice::ReadOnly);
    QTextStream text(&f);
    return text.readAll();
}

void MainWindow::on_load_triggered()
{
    QString filename = QFileDialog::getOpenFileName(
       this,
       "Open Script",
       QDir::currentPath(),
       "Circuit File(*.lc *.txt);;All files(*.*)");

    if (!filename.isNull())
    {
       this->loaded=true; //选完了设true
       s.eval(ReadTXT(filename).toStdString());
    }
    else // 用户取消选择
       QMessageBox::information(NULL, "提示", "未加载电路脚本");
}
