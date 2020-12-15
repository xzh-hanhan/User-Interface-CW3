//
// Created by twak on 11/11/2019.
//

#include "the_button.h"
#include <QNetworkRequest>
#include <QNetworkAccessManager>


void TheButton::init(TheButtonInfo* i) {
    setIcon( *(i->icon) );
    setText(i->url->fileName());
    //setText("Video Name \nVideo Location\nVideo Length");
    info =  i;
}


void TheButton::clicked() {
    emit jumpTo(info);
}
