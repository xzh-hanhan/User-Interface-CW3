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
#include <QSlider>
#include <QToolTip>
#include<QLabel>
#include "playerbtn.h"
#include<QFileInfo>

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
    playerBtn *playButton = 0;
    playerBtn *ffButton = 0;
    playerBtn *rewindButton = 0;
    playerBtn *nextButton = 0;
    playerBtn *restartButton = 0;
    playerBtn* listBtn = 0;
    playerBtn* volumeBtn = 0;
    playerBtn* fullScreenBtn = 0;
    QSlider* volumeSlider = 0;
    QSlider* timeSlider = 0;
    QWidget *display = 0;
    QLabel *name;

    bool isPress = false;
public:
    ThePlayer();
    // all buttons have been setup, store pointers here
    void setContent(vector<TheButton*>* b, vector<TheButtonInfo>* i);

    QWidget* getDisplay() const { return display; }
    TheButtonInfo* getInfo() { return currentInfo; }


    bool eventFilter(QObject *watched, QEvent *event);
signals:
    void sigOpenList(bool flag);

private slots:
    void playClicked();
    void ffClicked();
    void rewindClicked();
    void nextClicked();
    void restartClicked();
    void sltPosition(qint64 time);
    void sltDuration(qint64 time);
    void sltPress();
    void sltRelease();
    void sltMute(bool flag);
    void sltVolumeChanged(int val);

public slots:
    // start playing this ButtonInfo
    void jumpTo (TheButtonInfo* button);
};

#endif //CW2_THE_PLAYER_H
