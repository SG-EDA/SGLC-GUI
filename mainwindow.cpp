#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "excep.h"
#include <QMessageBox>
#include "script.h"
#include <QLabel>
#include <QSpinBox>
#include <QFormLayout>
#include <QDialogButtonBox>
#include <QLineEdit>
#include <QFileDialog>
#include <QTextStream>
#include <QGraphicsLineItem>

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->resize(QSize(1920,1080));
    this->scene=new QGraphicsScene(0,0,1920,1080);
    ui->Graph->setScene(scene);
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
                QString name=spinbox1->text();
                QString value=spinbox2->text();

                try
                {
                    // s.set(name,value); //变量名和值都传string就行了，不用类型转换了
                }
                catch(undefinedVariableExcep e)
                {
                     QString name=spinbox1->text();
                     QString value=spinbox2->text();
                     s.lineMap[name.toStdString()]->constVal=help::toint(value.toStdString());
            //undefinedVariableExcep在头文件excep.h里有，这是我更新之后新加的，记得复制过去然后添加到项目
            //如果变量名不存在会触发这个异常，在这里弹个窗提示用户
                 }
             }
    }
}

void MainWindow::on_resetTrigger_triggered()
{
    if(loaded==false)
        QMessageBox::information(NULL, "提示", "未加载电路脚本");
    else
    {
        s.getManager()->resetTri();
    }
}

void MainWindow::on_run_triggered()
{
    if(loaded==false)
        QMessageBox::information(NULL, "提示", "未加载电路脚本");
    else
    {
        string result=s.getManager()->run();
        ui->textEdit->append(QString::fromStdString(result));
    }
}

void MainWindow::on_truthTable_triggered()
{
    if(loaded==false)
        QMessageBox::information(NULL, "提示", "未加载电路脚本");
    else
    {
        string result=s.getManager()->trueTable();
        ui->textEdit->append(QString::fromStdString(result));
    }
}

void MainWindow::on_stateTruthTable_triggered()
{
    if(loaded==false)
        QMessageBox::information(NULL, "提示", "未加载电路脚本");
    else
    {
       string result=s.getManager()->trueTable(0,true);
       ui->textEdit->append(QString::fromStdString(result));
    }
}

void MainWindow::on_run2_triggered()
{

    if(loaded==true)
        QMessageBox::information(NULL, "提示", "未加载电路脚本");
    else
    {
        QGraphicsLineItem *line=new QGraphicsLineItem;
        line->setLine(x(),y(),1000,1000);
        scene->addItem(line);
        QGraphicsEllipseItem *Ellipse=new QGraphicsEllipseItem;
        Ellipse->setRect(x(),y(),100,100);
        scene->addItem(Ellipse);
        s.getManager()->middleVar();
    }
    //fix:需改输出，画到图形框
}

void MainWindow::on_elementCount_triggered()
{
    if(loaded==false)
        QMessageBox::information(NULL, "提示", "未加载电路脚本");
    else
    {
        string result=s.getManager()->gateNum();
        ui->textEdit->append(QString::fromStdString(result));
    }
}

void MainWindow::on_lineComplexity_triggered()
{
    if(loaded==false)
        QMessageBox::information(NULL, "提示", "未加载电路脚本");
    else
    {
        string result=s.getManager()->multiplexing();
        ui->textEdit->append(QString::fromStdString(result));
    }
}

void MainWindow::on_closeCircuitScript_triggered()
{
    if(loaded==false)
        QMessageBox::information(NULL, "提示", "未加载电路脚本");
    else
    {
        s.clear();
        ui->textEdit->clear();
        ui->Graph->scene()->clear();
    }
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
       //s.eval(ReadTXT(filename).toStdString());
       s.runFile(filename.toStdString());
    }
    else // 用户取消选择

       QMessageBox::information(NULL, "提示", "未加载电路脚本");
}
