//
// Created by twak on 11/11/2019.
//

#ifndef CW2_THE_PLAYER_H
#define CW2_THE_PLAYER_H


#include <QApplication>
#include <QMediaPlayer>
#include "the_button.h"
#include <vector>
#include <QTimer>
#include <QVideoWidget>
#include <QVBoxLayout>

using namespace std;

class ThePlayer : public QMediaPlayer {

Q_OBJECT

private:
    vector<TheButtonInfo>* infos;
    vector<TheButton*>* buttons;
    QTimer* mTimer;
    long updateCount = 0;
    TheButtonInfo* currentInfo; //info of the video currently playing

    QVideoWidget *videoWidget = 0;
    QPushButton *playButton = 0;
    QPushButton *ffButton = 0;
    QPushButton *rewindButton = 0;
    QPushButton *nextButton = 0;
    QPushButton *restartButton = 0;
    QWidget *display = 0;

public:
    ThePlayer();

    // all buttons have been setup, store pointers here
    void setContent(vector<TheButton*>* b, vector<TheButtonInfo>* i);

    QWidget* getDisplay() const { return display; }

private slots:
    void playClicked();
    void ffClicked();
    void rewindClicked();
    void nextClicked();
    void restartClicked();

    void playStateChanged (QMediaPlayer::State ms);

public slots:
    // start playing this ButtonInfo
    void jumpTo (TheButtonInfo* button);
};

#endif //CW2_THE_PLAYER_H
