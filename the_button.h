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
    QString string;//information to disaplay

    TheButtonInfo ( QUrl* url, QIcon* icon,QString string) : url (url), icon (icon),string(string) {}
};

class TheButton : public QPushButton {
    Q_OBJECT

public:
    TheButtonInfo* info;

     TheButton(QWidget *parent) :  QPushButton(parent) {
         parent->setStyleSheet("QPushButton { background-color: #3949ab; color: white;} QPushButton:hover {background-color: #6f74dd;}");
         setIconSize(QSize(200,110));
         connect(this, SIGNAL(released()), this, SLOT (clicked() )); // if QPushButton clicked...then run clicked() below
    }

    void enterEvent(QEvent *e);
    void leaveEvent(QEvent *e);
//    bool event(QEvent *e);

    void init(TheButtonInfo* i);


private slots:
    void clicked();

signals:
    void jumpTo(TheButtonInfo*);

};

#endif //CW2_THE_BUTTON_H
