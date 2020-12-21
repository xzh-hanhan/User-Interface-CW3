#include "playerbtn.h"

playerBtn::playerBtn(QString n)
{
    name = n;
    QString icon = ":/playback_images/" + name + ".png";
    setIconSize(QSize(60,40));
    setStyleSheet(qVidButtonStyleSheet);
    setIcon(QIcon((icon)));

}

void playerBtn::enterEvent(QEvent *e)
{
   QEnterEvent *enterEvent = static_cast<QEnterEvent*>(e);
   QToolTip::showText(enterEvent->globalPos(),name,this);
}

void playerBtn::leaveEvent(QEvent *e)
{
   return;
}
