//
// Created by twak on 11/11/2019.
//

#include "the_player.h"
#include<QHelpEvent>
#include<QToolTip>
#include <QTime>

using namespace std;


ThePlayer::ThePlayer() : QMediaPlayer(NULL) {
    setVolume(80); // be slightly less annoying
    setNotifyInterval(1);
    volumeSlider->setRange(0,100);
    volumeSlider->setValue(80);

    videoWidget->setMinimumHeight(480);
    this->setVideoOutput(videoWidget);
    videoWidget->installEventFilter(this);

    timeLayout->addWidget(timeSlider);
    timelabel->setText("00:00/00:00");
    timeLayout->addWidget(timelabel);

    name->setStyleSheet("font-weight: bold;font: 15pt Arial Bold");

    mainLayout->addWidget(name,1,Qt::AlignCenter);
    mainLayout->addWidget(videoWidget);
    mainLayout->addLayout(timeLayout);
    mainLayout->addLayout(controlsLayout);

    controlsLayout->addWidget(listBtn);
    controlsLayout->addWidget(fullScreenBtn);
    controlsLayout->addWidget(restartButton);
    controlsLayout->addWidget(rewindButton);
    controlsLayout->addWidget(playButton);
    controlsLayout->addWidget(ffButton);
    controlsLayout->addWidget(nextButton);
    controlsLayout->addWidget(volumeBtn);
    controlsLayout->addWidget(volumeSlider);  

    setfunctions();
    display->setLayout(mainLayout);
}

void ThePlayer::setfunctions(){
    playButton->setCheckable(true);
    connect(playButton,SIGNAL(clicked(bool)),this,SLOT(sltPlay(bool)));
    connect(ffButton,SIGNAL(released()),this,SLOT(sltSender()));
    connect(rewindButton,SIGNAL(released()),this,SLOT(sltSender()));
    connect(nextButton, SIGNAL (released()), this, SLOT (nextClicked()));
    connect(restartButton, SIGNAL (released()), this, SLOT (restartClicked()));
    listBtn->setCheckable(true);
    connect(listBtn,SIGNAL(clicked(bool)),this,SIGNAL(sigOpenList(bool)));
    connect(volumeBtn,SIGNAL(released()),this,SLOT(sltMute()));
    fullScreenBtn->setCheckable(true);
    connect(fullScreenBtn,&QPushButton::clicked,[=](bool){videoWidget->setFullScreen(true);});
    connect(timeSlider,SIGNAL(sliderPressed()),this,SLOT(sltPress()));
    connect(timeSlider,SIGNAL(sliderReleased()),this,SLOT(sltRelease()));
    connect(this,SIGNAL(positionChanged(qint64)),this,SLOT(sltPosition(qint64)));
    connect(this,SIGNAL(durationChanged(qint64)),this,SLOT(sltDuration(qint64)));
    connect(volumeSlider,SIGNAL(valueChanged(int)),this,SLOT(sltVolumeChanged(int)));
    connect(this,SIGNAL(stateChanged(QMediaPlayer::State)),this,SLOT(sltState(QMediaPlayer::State)));
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
        else if(event->type() == QEvent::MouseButtonRelease){
            playClicked();
        }
        else if(event->type() == QEvent::KeyPress)
        {
            QKeyEvent* keyevent = (QKeyEvent*)event;
            if(keyevent->key() == Qt::Key_Escape && videoWidget->isFullScreen())
            {
                videoWidget->setFullScreen(false);
            }

        }
    }

    return QMediaPlayer::eventFilter(watched,event);
}


void ThePlayer::sltMute()
{
    if(this->volume()!=0)
    {
        this->setMuted(true);
        volumeSlider->setValue(0);
    }
    else
    {
        this->setMuted(false);
        volumeSlider->setValue(80);

    }
}

void ThePlayer::sltVolumeChanged(int val)
{
    this->setVolume(val);
    if(val==0)
        this->setMuted(true);
}

void ThePlayer::sltSender()
{
    if(this->state() == QMediaPlayer::StoppedState)
        return ;

    QPushButton* btn = (QPushButton*)sender();
    qint64 time = this->position();
    if(rewindButton == btn)
    {
        time -= 5000;
        if(time < 0)
        {
            this->stop();
            return ;
        }
    }
    else
    {
        time += 5000;

        if(time > this->duration())
        {
            this->stop();
            return ;
        }
    }

    this->setPosition(time);
}

void ThePlayer::sltState(QMediaPlayer::State newstate)
{

    switch (newstate) {
    case QMediaPlayer::State::StoppedState:
        playButton->setChecked(false);
        playButton->name = "play";
        playButton->setIcon(QIcon((":/playback_images/play.png")));
        timeSlider->setValue(0);
        break;
    case QMediaPlayer::State::PausedState:
        playButton->name = "play";
        playButton->setIcon(QIcon((":/playback_images/play.png")));
        playButton->setChecked(false);
        break;
    case QMediaPlayer::State::PlayingState:
        playButton->name = "pause";
        playButton->setIcon(QIcon((":/playback_images/pause.png")));
        playButton->setChecked(true);
        break;
    }

}

void ThePlayer::sltPlay(bool flag)
{
    if(flag)
    {
        play();
    }
    else
    {
        pause();
    }
}

void ThePlayer::sltPosition(qint64 time)
{
    if(!isPress)
        timeSlider->setValue(time);

    timelabel->setText(QString("%1/%2").arg(QTime::fromMSecsSinceStartOfDay(time).toString("mm:ss"))
                               .arg(QTime::fromMSecsSinceStartOfDay(timeSlider->maximum()).toString("mm:ss")));
}

void ThePlayer::sltDuration(qint64 time)
{
    timeSlider->setRange(0,time);

    timelabel->setText(QString("%1/%2").arg(QTime::fromMSecsSinceStartOfDay(0).toString("mm:ss"))
                               .arg(QTime::fromMSecsSinceStartOfDay(time).toString("mm:ss")));


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

