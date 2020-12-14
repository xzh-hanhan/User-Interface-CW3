//
// Created by twak on 11/11/2019.
//

#include "the_button.h"
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include<QHelpEvent>

void TheButton::init(TheButtonInfo* i) {
    setIcon( *(i->icon) );
//    setToolTip(i->url->fileName());
    setText(i->url->fileName());

    //setText("Video Name \nVideo Location\nVideo Length");
    info =  i;
}


void TheButton::clicked() {
    emit jumpTo(info);
}

 void TheButton::enterEvent(QEvent *e)
 {
    QEnterEvent *enterEvent = static_cast<QEnterEvent*>(e);
    QToolTip::showText(enterEvent->globalPos(),info->string,this);
 }
 void TheButton::leaveEvent(QEvent *e)
 {
     init(info);
 }



// bool TheButton::event(QEvent *event)
// {
//     if (event->type() == QEvent::ToolTip) {
//             QHelpEvent *helpEvent = static_cast<QHelpEvent*>(event);


// }
