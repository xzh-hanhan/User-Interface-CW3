//
// Created by twak on 11/11/2019.
//

#include "the_player.h"
#include<QHelpEvent>
#include<QToolTip>

using namespace std;


ThePlayer::ThePlayer() : QMediaPlayer(NULL) {
    setVolume(0); // be slightly less annoying
    setNotifyInterval(1);

    videoWidget = new QVideoWidget;
    this->setVideoOutput(videoWidget);
    videoWidget->installEventFilter(this);

    timeSlider = new QSlider(Qt::Horizontal);
    name = new  QLabel();
    name->setStyleSheet("font-weight: bold;font: 15pt Arial Bold");


    playButton = new playerBtn("pause");
    connect(playButton, SIGNAL (released()), this, SLOT (playClicked()));

    ffButton = new playerBtn("ff");
    connect(ffButton, SIGNAL (released()), this, SLOT (ffClicked()));

    rewindButton = new playerBtn("rewind");;
    connect(rewindButton, SIGNAL (released()), this, SLOT (rewindClicked()));

    nextButton = new playerBtn("next");;
    connect(nextButton, SIGNAL (released()), this, SLOT (nextClicked()));

    restartButton = new playerBtn("restart");;
    connect(restartButton, SIGNAL (released()), this, SLOT (restartClicked()));

    listBtn = new playerBtn("hidden");
    listBtn->setCheckable(true);
    connect(listBtn,SIGNAL(clicked(bool)),this,SIGNAL(sigOpenList(bool)));

    volumeBtn = new playerBtn("volume");
    volumeBtn->setCheckable(true);
    connect(volumeBtn,SIGNAL(clicked(bool)),this,SLOT(sltMute(bool)));

    fullScreenBtn = new playerBtn("full");
    fullScreenBtn->setCheckable(true);
    connect(fullScreenBtn,&QPushButton::clicked,[=](bool){videoWidget->setFullScreen(true);});

    volumeSlider = new QSlider(Qt::Horizontal);
    volumeSlider->setRange(0,100);
    //set up the layout
    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->addWidget(name,1,Qt::AlignCenter);
    mainLayout->addWidget(videoWidget);
    mainLayout->addWidget(timeSlider);
    QHBoxLayout *controlsLayout = new QHBoxLayout();

    controlsLayout->addWidget(listBtn);
    controlsLayout->addWidget(fullScreenBtn);
    controlsLayout->addWidget(restartButton);
    controlsLayout->addWidget(rewindButton);
    controlsLayout->addWidget(playButton);
    controlsLayout->addWidget(ffButton);
    controlsLayout->addWidget(nextButton);
    controlsLayout->addWidget(volumeBtn);
    controlsLayout->addWidget(volumeSlider);
    mainLayout->addLayout(controlsLayout);


    display = new QWidget;
    display->setLayout(mainLayout);

    connect(timeSlider,SIGNAL(sliderPressed()),this,SLOT(sltPress()));
    connect(timeSlider,SIGNAL(sliderReleased()),this,SLOT(sltRelease()));

    connect(this,SIGNAL(positionChanged(qint64)),this,SLOT(sltPosition(qint64)));
    connect(this,SIGNAL(durationChanged(qint64)),this,SLOT(sltDuration(qint64)));
    connect(volumeSlider,SIGNAL(valueChanged(int)),this,SLOT(sltVolumeChanged(int)));
}

// all buttons have been setup, store pointers here
void ThePlayer::setContent(std::vector<TheButton*>* b, std::vector<TheButtonInfo>* i) {
    buttons = b;
    infos = i;
    currentInfo = buttons->at(0)->info;
    setMedia(* currentInfo->url);
    play();

    QString data =currentInfo->url->toString();
    QFileInfo file(data);
    data = file.baseName();
    name->setText(data);
    //buttons->at(0)->init(&infos->at(infos->size()-1));
}


bool ThePlayer::eventFilter(QObject *watched, QEvent *event)
{
    if(watched == videoWidget)
    {
        if(event->type() == QEvent::MouseButtonDblClick)
        {
            if(videoWidget->isFullScreen())
            {
                videoWidget->setFullScreen(false);
            }
            else
            {
                videoWidget->setFullScreen(true);
            }
        }
    }

    return QMediaPlayer::eventFilter(watched,event);
}


void ThePlayer::sltMute(bool flag)
{
    if(flag)
    {
        this->setMuted(true);
    }
    else
    {
        this->setMuted(false);
    }
}

void ThePlayer::sltVolumeChanged(int val)
{
    this->setVolume(val);
}

void ThePlayer::sltPosition(qint64 time)
{
    if(!isPress)
        timeSlider->setValue(time);
}

void ThePlayer::sltDuration(qint64 time)
{
    timeSlider->setRange(0,time);
}

void ThePlayer::sltPress()
{
    isPress = true;
}

void ThePlayer::sltRelease()
{
    isPress = false;
    this->setPosition(timeSlider->value());
}

void ThePlayer::playClicked() {

    if (playbackRate() != 1) {
        setPlaybackRate(1);
        playButton->setIcon(QIcon((":/playback_images/pause.png")));
        return;
    }

    switch (this->state()) {
    case QMediaPlayer::State::StoppedState:
        play();
        playButton->name = "pause";
        playButton->setIcon(QIcon((":/playback_images/pause.png")));
        break;
    case QMediaPlayer::State::PausedState:
        play();
        playButton->name = "pause";
        playButton->setIcon(QIcon((":/playback_images/pause.png")));
        break;
    case QMediaPlayer::State::PlayingState:
        pause();
        playButton->name = "play";
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

    currentInfo = buttons->at(1)->info;
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
    if(currentInfo==button)
            return;

    for (unsigned long long i=0; i < buttons->size(); i++) {
        if (buttons->at(i)->info == button) {
            for (unsigned long long j=i; j <buttons->size()-1; j++)
                buttons->at(j)->init(buttons->at(j+1)->info);
            break;
        }
    }
    buttons->at(buttons->size()-1)->init(currentInfo);

    currentInfo = button;
    buttons->at(0)->init(currentInfo);
    setMedia(* currentInfo->url);
    setPlaybackRate(1);
    play();
    playButton->name = "pause";
    playButton->setIcon(QIcon((":/playback_images/pause.png")));
    QString data =button->url->toString();
    QFileInfo file(data);
    data = file.baseName();
    name->setText(data);
}

