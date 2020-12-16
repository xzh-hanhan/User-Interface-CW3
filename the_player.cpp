//
// Created by twak on 11/11/2019.
//

#include "the_player.h"

using namespace std;

ThePlayer::ThePlayer() : QMediaPlayer(NULL) {
    setVolume(0); // be slightly less annoying
    setNotifyInterval(1);

    connect (this, SIGNAL (stateChanged(QMediaPlayer::State)), this, SLOT (playStateChanged(QMediaPlayer::State)) );

    videoWidget = new QVideoWidget;
    this->setVideoOutput(videoWidget);

//    timeSlider = new QSlider(Qt::Horizontal);


    const QString qVidButtonStyleSheet = "QPushButton { background-color: #3949ab; color: white; border-radius: 8px;} QPushButton:hover {background-color: #6f74dd;}";

    //set up the playback buttons
    playButton = new QPushButton;
    playButton->setIconSize(QSize(90,40));
    playButton->setStyleSheet(qVidButtonStyleSheet);
    playButton->setIcon(QIcon((":/playback_images/pause.png")));
    connect(playButton, SIGNAL (released()), this, SLOT (playClicked()));

    ffButton = new QPushButton;
    ffButton->setIconSize(QSize(90,40));
    ffButton->setStyleSheet(qVidButtonStyleSheet);
    ffButton->setIcon(QIcon((":/playback_images/ff.png")));
    connect(ffButton, SIGNAL (released()), this, SLOT (ffClicked()));

    rewindButton = new QPushButton;
    rewindButton->setIconSize(QSize(90,40));
    rewindButton->setStyleSheet(qVidButtonStyleSheet);
    rewindButton->setIcon(QIcon((":/playback_images/rewind.png")));
    connect(rewindButton, SIGNAL (released()), this, SLOT (rewindClicked()));

    nextButton = new QPushButton;
    nextButton->setIconSize(QSize(90,40));
    nextButton->setStyleSheet(qVidButtonStyleSheet);
    nextButton->setIcon(QIcon((":/playback_images/next.png")));
    connect(nextButton, SIGNAL (released()), this, SLOT (nextClicked()));

    restartButton = new QPushButton;
    restartButton->setIconSize(QSize(90,40));
    restartButton->setStyleSheet(qVidButtonStyleSheet);
    restartButton->setIcon(QIcon((":/playback_images/restart.png")));
    connect(restartButton, SIGNAL (released()), this, SLOT (restartClicked()));

    listBtn = new QPushButton;
    listBtn->setIconSize(QSize(90,40));
    listBtn->setStyleSheet(qVidButtonStyleSheet);
    listBtn->setCheckable(true);
    listBtn->setIcon(QIcon((":/playback_images/hidden.png")));
    connect(listBtn,SIGNAL(clicked(bool)),this,SIGNAL(sigOpenList(bool)));

    //set up the layout
    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->addWidget(videoWidget);
    mainLayout->addWidget(timeSlider);
    QHBoxLayout *controlsLayout = new QHBoxLayout();
    controlsLayout->addWidget(restartButton);
    controlsLayout->addWidget(rewindButton);
    controlsLayout->addWidget(playButton);
    controlsLayout->addWidget(ffButton);
    controlsLayout->addWidget(nextButton);
    controlsLayout->addWidget(listBtn);
    mainLayout->addLayout(controlsLayout);

    display = new QWidget;
    display->setLayout(mainLayout);

//    connect(timeSlider,SIGNAL(sliderPressed()),this,SLOT(onPress()));
//    connect(timeSlider,SIGNAL(sliderReleased()),this,SLOT(onRelease()));

//    connect(this,SIGNAL(positionChanged(qint64)),this,SLOT(onPosition(qint64)));
//    connect(this,SIGNAL(durationChanged(qint64)),this,SLOT(onDuration(qint64)));
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

//void ThePlayer::onPosition(qint64 time)
//{
//    if(!isPress)
//        timeSlider->setValue(time);
//}

//void ThePlayer::onDuration(qint64 time)
//{
//    timeSlider->setRange(0,time);
//}

//void ThePlayer::onPress()
//{
//    isPress = true;
//}

//void ThePlayer::onRelease()
//{
//    isPress = false;
//    this->setPosition(timeSlider->value());
//}

void ThePlayer::playClicked() {

    if (playbackRate() != 1) {
        setPlaybackRate(1);
        playButton->setIcon(QIcon((":/playback_images/pause.png")));
        return;
    }

    switch (this->state()) {
    case QMediaPlayer::State::StoppedState:
        play();
        playButton->setIcon(QIcon((":/playback_images/pause.png")));
        break;
    case QMediaPlayer::State::PausedState:
        play();
        playButton->setIcon(QIcon((":/playback_images/pause.png")));
        break;
    case QMediaPlayer::State::PlayingState:
        pause();
        playButton->setIcon(QIcon((":/playback_images/play.png")));
        break;
    }
}

void ThePlayer::ffClicked() {
    playButton->setIcon(QIcon((":/playback_images/play.png")));
    setPlaybackRate(2);
}

void ThePlayer::rewindClicked() {
    playButton->setIcon(QIcon((":/playback_images/play.png")));
    setPlaybackRate(-2);
}

void ThePlayer::nextClicked() {
    TheButtonInfo *temp = currentInfo;

    currentInfo = buttons->at(0)->info;
    setMedia(* currentInfo->url);
    setPlaybackRate(1);
    play();

    //move all of the videos in up next up by 1 and replace last video with the one that was previously playing
    for (unsigned long long i=0; i < buttons->size()-1; i++) {
        buttons->at(i)->init(buttons->at(i+1)->info);
    }
    buttons->at(buttons->size()-1)->init(temp);
}

void ThePlayer::restartClicked() {
    setMedia(*currentInfo->url);
    setPlaybackRate(1);
    play();
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
