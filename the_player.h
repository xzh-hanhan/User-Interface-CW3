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

    //set up the layout
    QVBoxLayout *mainLayout = new QVBoxLayout();
    QVideoWidget *videoWidget = new QVideoWidget;
    QHBoxLayout *timeLayout = new QHBoxLayout();
    QHBoxLayout *controlsLayout = new QHBoxLayout();

    playerBtn *playButton = new playerBtn("play");
    playerBtn *ffButton = new playerBtn("ff");
    playerBtn *rewindButton = new playerBtn("rewind");
    playerBtn *nextButton = new playerBtn("next");
    playerBtn *restartButton = new playerBtn("restart");
    playerBtn* listBtn = new playerBtn("hidden");
    playerBtn* volumeBtn = new playerBtn("volume");
    playerBtn* fullScreenBtn = new playerBtn("full");
    QSlider* volumeSlider = new QSlider(Qt::Horizontal);
    QSlider* timeSlider = new QSlider(Qt::Horizontal);
    QWidget *display = new QWidget;
    QLabel *name = new  QLabel();
    QLabel* timelabel = new QLabel();

    bool isPress = false;
public:
    ThePlayer();
    // all buttons have been setup, store pointers here
    void setContent(vector<TheButton*>* b, vector<TheButtonInfo>* i);

    QWidget* getDisplay() const { return display; }
    TheButtonInfo* getInfo() { return currentInfo; }
    void setfunctions();

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
    void sltMute();
    void sltVolumeChanged(int val);


    void sltSender();

    void sltState(QMediaPlayer::State newstate);

    void sltPlay(bool flag);
public slots:
    // start playing this ButtonInfo
    void jumpTo (TheButtonInfo* button);
};

#endif //CW2_THE_PLAYER_H
