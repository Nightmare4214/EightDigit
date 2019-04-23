#include "eightdigit.h"
#include "ui_eightdigit.h"
#include<iostream>
#include<QMessageBox>
#include<QIcon>
#include"ida_star.cpp"
const int N = 362880;
//阶乘
const int fac[n] = { 1,1,2,6,24,120,720,5040,40320 };
EightDigit::EightDigit(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EightDigit)
{
    ui->setupUi(this);
    //格子总共占用2/3的总界面，放在左上角
    int widget_size=max(height(),width())*2/3;
    ui->widget->setGeometry(0,0,widget_size,widget_size);
    //设置背景颜色
    QPalette pal(ui->widget->palette());
    pal.setColor(QPalette::Background, QColor(255,255,255));
    ui->widget->setAutoFillBackground(true);
    ui->widget->setPalette(pal);

    //按钮占1/3的widget
    button_size=widget_size/3;
    for(int i=0;i<9;++i){
        button[i]=new QPushButton(this);
        button[i]->setObjectName(QString::number(i));
        int temp=i;
        if(temp==0)temp=9;
        button[i]->setStyleSheet(QString("font-size:30px;border-image:url(:/img/%1.png)").arg(temp));
        button[i]->resize(button_size,button_size);
        int t;
        if(i==0){
            t=8;
            button[i]->hide();
        }
        else{
            t=i-1;
            QString s=":/img"+(QString::number(i))+".png";
        }
        moveButtonToPoint(i,t%3,t/3);
        connect(button[i],SIGNAL(clicked(bool)),this,SLOT(moveSpace()));

    }
    timer=new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(updateProgress()), Qt::DirectConnection);
    //10ms移动一次
    timer->setInterval(10);
    //thread=new QThread();
    //速度
    speed=button_size/10;
}

EightDigit::~EightDigit()
{
    delete ui;
}

void EightDigit::moveSpace()
{
    if(timer->isActive())return;
    //获得被点中的按钮
    QPushButton* t=(QPushButton*)sender();
    num=t->objectName().toInt();
    int spaceX,spaceY;
    //获得空格和点击的按钮的位置
    getPosById(num,preX,preY);
    getPosById(0,spaceX,spaceY);
    //空格是否与按钮相邻
    if(preX==spaceX&&abs(preY-spaceY)==1||
            preY==spaceY&&abs(preX-spaceX)==1){
        button[0]->hide();
        step_size=1;
        step_i=0;
        step[0][0]=spaceX-preX;
        step[0][1]=spaceY-preY;
        timer->start();
    }
}

bool EightDigit::moveReached()
{
    int spaceX,spaceY;
    getPosById(0,spaceX,spaceY);
    int spacePosX=spaceX*button_size;
    int spacePosY=spaceY*button_size;
    return abs(button[num]->x()-spacePosX)<speed&&abs(button[num]->y()-spacePosY)<speed;
}

bool EightDigit::solved()
{
    for(int i=0;i<9;++i){
        if(i==8)return digit[8]==0;
        else if(digit[i]!=i+1)return false;
    }
    return true;
}

int EightDigit::getInv()
{
    int cnt = 0;
    for (int i = 1; i < n; ++i) {
        if (digit[i] == 0)continue;
        for (int j = 0; j < i; ++j) {
            if (digit[j] == 0)continue;
            if (digit[i] < digit[j])
                ++cnt;
        }
    }
    return cnt&1;
}

void EightDigit::reverseCantor(int hash)
{
    bool visited[n] = {};
    int temp;
    for (int i = 0; i < n; ++i) {
        temp = hash / fac[n - 1 - i];
        for (int j = 0; j < n; ++j) {
            if (!visited[j]) {
                if (temp == 0) {
                    digit[i] = j;
                    visited[j] = true;
                    break;
                }
                --temp;
            }
        }
        hash %= fac[8 - i];
    }
}

void EightDigit::on_generate_btn_clicked()
{
    if(timer->isActive())return;
    while(true){
        int hash=rd()%N;
        //获得局面
        reverseCantor(hash);
        //逆序
        if(0==getInv()&&46233!=hash)break;
    }
    for(int i=0;i<n;++i)moveButtonToPoint(digit[i],i%3,i/3);
}

void EightDigit::on_AI_solution_btn_clicked()
{
    if(timer->isActive())return;
    if(solved())return;
    int spaceX,spaceY;
    getPosById(0,spaceX,spaceY);
    //ida*求解
    IDA_star(digit,getIDByXY(spaceX,spaceY),step,step_size);
    step_i=0;
    updateData(spaceX,spaceY);
    timer->start();
}

void EightDigit::updateData(int spaceX,int spaceY)
{
    preX=spaceX-step[step_i][0];
    preY=spaceY-step[step_i][1];
    num=digit[getIDByXY(preX,preY)];
}

void EightDigit::updateProgress()
{
    //移动
    button[num]->move(button[num]->x()+step[step_i][0]*speed,
            button[num]->y()+step[step_i][1]*speed);
    //如果移动到了指定的位置
    if(moveReached()){
        int spaceX,spaceY;
        //获得空格的位置
        getPosById(0,spaceX,spaceY);
        //移动空格和按钮
        moveButtonToPoint(num,spaceX,spaceY);
        moveButtonToPoint(0,preX,preY);
        ++step_i;
        //移动结束
        if(step_i==step_size){
            timer->stop();
            //到达终点
            if(solved()){
                button[0]->show();
                QMessageBox::about(NULL, "胜利", "<font color='red'>wryyyyyy</font>");
                button[0]->hide();
            }
            return;
        }
        swap(preX,spaceX);
        swap(preY,spaceY);
        updateData(spaceX,spaceY);
    }
}

int EightDigit::getIDByXY(int x, int y)
{
    return x+y*3;
}

void EightDigit::moveButtonToPoint(int i, int x, int y)
{
    button[i]->move(x*button_size,y*button_size);
    digit[x+y*3]=i;
}

void EightDigit::getPosById(int i, int &x, int &y)
{
    x=button[i]->x()/button_size;
    y=button[i]->y()/button_size;
}
