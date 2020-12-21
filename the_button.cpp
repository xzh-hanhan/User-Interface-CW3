//
// Created by twak on 11/11/2019.
//

#include "the_button.h"
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QHelpEvent>
#include <QLineEdit>
#include <QGridLayout>
#include <QLabel>
#include <QDialog>
#include <QFile>

void TheButton::init(TheButtonInfo* i) {
    setIcon( *(i->icon) );
//    setToolTip(i->url->fileName());
    setText(i->url->fileName());

    //setText("Video Name \nVideo Location\nVideo Length");
    info =  i;
}

void TheButton::mousePressEvent(QMouseEvent *event)
{

    if(event->button() == Qt::LeftButton)
   {
        clicked();
    }
    // 如果是鼠标右键按下
    else if(event->button() == Qt::RightButton)
   {
       rightclick();
    }
}

void TheButton::clicked() {
    emit jumpTo(info);
}

void TheButton::rightclick() {
    emit right();
}

 void TheButton::enterEvent(QEvent *e)
 {
    QEnterEvent *enterEvent = static_cast<QEnterEvent*>(e);
    info->data = "Device:\t\t\t" + info->dev + "\n"
                +"Position:\t\t\t" + info->pos + "\n" + "\n"
                + "RightClick to Update Information.";
    QToolTip::showText(enterEvent->globalPos(),info->data,this);
 }
 void TheButton::leaveEvent(QEvent *e)
 {
     init(info);
 }

