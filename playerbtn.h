#ifndef PLAYERBTN_H
#define PLAYERBTN_H

#include <QPushButton>
#include <QUrl>
#include <QToolTip>
#include <QHelpEvent>


class playerBtn : public QPushButton
{
    Q_OBJECT
public:
    playerBtn(QString n);

    QString name;
    const QString qVidButtonStyleSheet = "QPushButton { background-color: #3949ab; color: white; border-radius: 8px;} QPushButton:hover {background-color: #6f74dd;}";

    void enterEvent(QEvent *e);
    void leaveEvent(QEvent *e);

};

#endif // PLAYERBTN_H
