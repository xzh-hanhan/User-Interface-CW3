//
// Created by twak on 11/11/2019.
//

#ifndef CW2_THE_BUTTON_H
#define CW2_THE_BUTTON_H


#include <QPushButton>
#include <QUrl>
#include <QToolTip>


class TheButtonInfo {

public:
    QUrl* url; // video file to play
    QIcon* icon; // icon to display
    QString data;//information to disaplay
    QString dev;
    QString time;
    QString pos;
    QString size;



    TheButtonInfo ( QUrl* url, QIcon* icon,QString dev,QString time,QString pos,QString size) : url (url), icon (icon),
        dev(dev), time(time), pos(pos), size(size) {}
};

class TheButton : public QPushButton {
    Q_OBJECT

public:
    TheButtonInfo* info;

     TheButton(QWidget *parent) :  QPushButton(parent) {
         parent->setStyleSheet("QPushButton {color: black;} QPushButton:hover {background-color: #b5b5b5;}");
         setIconSize(QSize(200,110));
         connect(this, SIGNAL(released()), this, SLOT (clicked() )); // if QPushButton clicked...then run clicked() below
    }

    void mousePressEvent(QMouseEvent *event);
    void enterEvent(QEvent *e);
    void leaveEvent(QEvent *e);
//    bool event(QEvent *e);

    void init(TheButtonInfo* i);


private slots:
    void clicked();
    void rightclick();

signals:
    void jumpTo(TheButtonInfo*);
    void right();

};

#endif //CW2_THE_BUTTON_H
