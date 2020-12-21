#include "classbtn.h"
#include<QEnterEvent>


//void classBtn::enterEvent(QEvent *e)
//{
//   QEnterEvent *enterEvent = static_cast<QEnterEvent*>(e);
//   QToolTip::showText(enterEvent->globalPos(),"intro",this);
//}

void classBtn::devto(TheButtonInfo* i){
    setText(i->dev);
}

void classBtn::posto(TheButtonInfo* i){
    setText(i->pos);
}

void classBtn::nameto(TheButtonInfo* i){

    QString data =i->url->toString();
    data = data.right(5);
    data = data.left(data.length()-4);
    setText(data);
}
