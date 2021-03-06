//
// Created by twak on 11/11/2019.
//

#include "the_player.h"

using namespace std;

ThePlayer::ThePlayer() : QMediaPlayer(NULL) {
    setVolume(0); // be slightly less annoying

    connect (this, SIGNAL (stateChanged(QMediaPlayer::State)), this, SLOT (playStateChanged(QMediaPlayer::State)) );

    videoWidget = new QVideoWidget;
 //   videoWidget->setFixedHeight(400);
    this->setVideoOutput(videoWidget);

    const QString qVidButtonStyleSheet = "QPushButton { background-color: #3949ab; color: white; border-radius: 8px;} QPushButton:hover {background-color: #6f74dd;}";

    //set up the playback buttons
    playButton = new QPushButton;
    playButton->setIconSize(QSize(40,40));
    playButton->setStyleSheet(qVidButtonStyleSheet);
    playButton->setIcon(QIcon((":/playback_images/pause.png")));

    ffButton = new QPushButton;
    ffButton->setIconSize(QSize(40,40));
    ffButton->setStyleSheet(qVidButtonStyleSheet);
    ffButton->setIcon(QIcon((":/playback_images/ff.png")));

    rewindButton = new QPushButton;
    rewindButton->setIconSize(QSize(40,40));
    rewindButton->setStyleSheet(qVidButtonStyleSheet);
    rewindButton->setIcon(QIcon((":/playback_images/rewind.png")));

    nextButton = new QPushButton;
    nextButton->setIconSize(QSize(40,40));
    nextButton->setStyleSheet(qVidButtonStyleSheet);
    nextButton->setIcon(QIcon((":/playback_images/next.png")));

    restartButton = new QPushButton;
    restartButton->setIconSize(QSize(40,40));
    restartButton->setStyleSheet(qVidButtonStyleSheet);
    restartButton->setIcon(QIcon((":/playback_images/restart.png")));

    //set up the layout
    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->addWidget(videoWidget);

    QHBoxLayout *controlsLayout = new QHBoxLayout();
    controlsLayout->addWidget(restartButton);
    controlsLayout->addWidget(rewindButton);
    controlsLayout->addWidget(playButton);
    controlsLayout->addWidget(ffButton);
    controlsLayout->addWidget(nextButton);
    mainLayout->addLayout(controlsLayout);

    display = new QWidget;
    display->setLayout(mainLayout);
}

// all buttons have been setup, store pointers here
void ThePlayer::setContent(std::vector<TheButton*>* b, std::vector<TheButtonInfo>* i) {
    buttons = b;
    infos = i;
    currentInfo = buttons->at(0)->info;
    setMedia(* currentInfo->url);
    play();
    buttons->at(0)->init(&infos->at(infos->size()-1));
}

void ThePlayer::playStateChanged (QMediaPlayer::State ms) {
    setPlaybackRate(1);

    switch (ms) {
    case QMediaPlayer::State::StoppedState:
        //play(); // starting playing again...
        playButton->setIcon(QIcon((":/playback_images/play.png")));
        break;
    case QMediaPlayer::State::PausedState:
        playButton->setIcon(QIcon((":/playback_images/play.png")));
        break;
    case QMediaPlayer::State::PlayingState:
        playButton->setIcon(QIcon((":/playback_images/pause.png")));
        break;
    default:
        break;
    }
}

void ThePlayer::jumpTo (TheButtonInfo* button) {
    //swap clicked button info with current playing info
    for (unsigned long long i=0; i < buttons->size(); i++) {
        if (buttons->at(i)->info == button) {
            for (unsigned long long j=i; j < buttons->size()-1; j++)
                buttons->at(j)->init(buttons->at(j+1)->info);
            buttons->at(buttons->size()-1)->init(currentInfo);
        }
    }

    currentInfo = button;
    setMedia(* currentInfo->url);
    setPlaybackRate(1);
    play();
}
