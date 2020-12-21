#ifndef CLASSBTN_H
#define CLASSBTN_H

#include <QPushButton>
#include <QWidget>
#include <vector>
#include "the_button.h"

using namespace std;

class classBtn : public QPushButton {
    Q_OBJECT
public:
//    void enterEvent(QEvent *e);

public slots:
    void devto(TheButtonInfo*);
    void posto(TheButtonInfo*);
    void nameto(TheButtonInfo*);
};


#endif // CLASSBTN_H
