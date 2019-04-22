#include "showpoint.h"
#include "ui_showpoint.h"
#include <QPainter>
#include <QDateTime>
#include <QFile>
#include <QTextStream>
#include <iostream>
using namespace std;
ShowPoint::ShowPoint(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ShowPoint)
{
    ui->setupUi(this);
    this->showFullScreen();
    theTimer.start(3000);
    connect(&theTimer, &QTimer::timeout, this, &ShowPoint::updateImage);
}

ShowPoint::~ShowPoint()
{
    delete ui;
}
void ShowPoint::paintEvent(QPaintEvent *e){
    QPainter painter(this);
    painter.setPen(Qt::black);
    painter.setBrush(QBrush(Qt::black));

    int seed = QDateTime::currentDateTime().time().second();
    srand(seed);
    QTime time = QTime::currentTime();
    qsrand(time.msec()*qrand()*qrand()*qrand()*qrand()*qrand()*qrand());
    //int x, y;
    int x[9] = {1657,255, 977, 1657, 255, 977, 1657, 255, 977};
    int y[9] = {908, 112, 112, 112, 543, 543, 543, 908, 908};
    if(i == 9){
        i = 0;
    }
   /* for( int i = 0; i < 9; i++){
        if(i % 3 == 0){
            x = qrand()%633;
        }else if(i % 3 == 1){
            x = qrand()%(633*2);
        }else {
            x = qrand()%(633*3);
        }
        if(i < 3){
            y = qrand()%360;
        }else if(i < 6 && i >=3){
            y = qrand()%720;
        }else{
            y = qrand()%1080;
        }
    }*/

   /* QFile data("point.txt");
    if(data.open(QFile::WriteOnly|QIODevice::Append)){
        QTextStream out(&data);
        out << x << " " << y << endl;
    }*/
    painter.drawEllipse(x[i],y[i],30,30);
    //std::cout<< i << "........" << endl;
    i++;


}

void ShowPoint::updateImage(){
    this->update();
}
