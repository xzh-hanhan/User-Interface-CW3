/*
 *
 *    ______
 *   /_  __/___  ____ ___  ___  ____
 *    / / / __ \/ __ `__ \/ _ \/ __ \
 *   / / / /_/ / / / / / /  __/ /_/ /
 *  /_/  \____/_/ /_/ /_/\___/\____/
 *              video for sports enthusiasts...
 *
 *  2811 cw3 : twak
 */

#include <iostream>
#include <QApplication>
#include <QtMultimediaWidgets/QVideoWidget>
#include <QMediaPlaylist>
#include <string>
#include <vector>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QLabel>

#include <QtWidgets/QHBoxLayout>
#include <QtCore/QFileInfo>
#include <QtWidgets/QFileIconProvider>
#include <QDesktopServices>
#include <QImageReader>
#include <QMessageBox>
#include <QtCore/QDir>
#include <QtCore/QDirIterator>
#include "the_player.h"
#include "the_button.h"
#include <QScrollArea>
#include <QPalette>


using namespace std;

// read in videos and thumbnails to this directory
vector<TheButtonInfo> getInfoIn (string loc) {

    vector<TheButtonInfo> out =  vector<TheButtonInfo>();
    QDir dir(QString::fromStdString(loc) );
    QDirIterator it(dir);

    while (it.hasNext()) { // for all files

        QString f = it.next();

            if (f.contains("."))

#if defined(_WIN32)
            if (f.contains(".wmv"))  { // windows
#else
            if (f.contains(".mp4") || f.contains("MOV"))  { // mac/linux
#endif

            QString thumb = f.left( f .length() - 4) +".png";
            if (QFile(thumb).exists()) { // if a png thumbnail exists
                QImageReader *imageReader = new QImageReader(thumb);
                    QImage sprite = imageReader->read(); // read the thumbnail
                    if (!sprite.isNull()) {
                        QIcon* ico = new QIcon(QPixmap::fromImage(sprite)); // voodoo to create an icon for the button
                        QUrl* url = new QUrl(QUrl::fromLocalFile( f )); // convert the file location to a generic url
                        out . push_back(TheButtonInfo( url , ico  ) ); // add to the output list
                    }
                    else
                        qDebug() << "1 warning: skipping video because I couldn't process thumbnail " << thumb << endl;
            }
            else{
                QString missingThumb = f.left( f.lastIndexOf( QChar('/') ) ) +"/missing.png";

                QImageReader *missingThumbReader = new QImageReader(missingThumb);
                    QImage missingThumbSprite = missingThumbReader->read(); // read the thumbnail
                    if (!missingThumbSprite.isNull()) {
                        QIcon* ico = new QIcon(QPixmap::fromImage(missingThumbSprite)); // voodoo to create an icon for the button
                        QUrl* url = new QUrl(QUrl::fromLocalFile( f )); // convert the file location to a generic url
                        out . push_back(TheButtonInfo( url , ico  ) ); // add to the output list
                    }
                    else
                        qDebug() << "2 warning: skipping video because I couldn't process thumbnail " << missingThumb << endl;
            }
        }
    }

    return out;
}


int main(int argc, char *argv[]) {

    // let's just check that Qt is operational first
    qDebug() << "Qt version: " << QT_VERSION_STR << endl;

    // create the Qt Application
    QApplication app(argc, argv);

    // collect all the videos in the folder
    vector<TheButtonInfo> videos;

    if (argc == 2)
        videos = getInfoIn( string(argv[1]) );

    if (videos.size() == 0) {

        const int result = QMessageBox::question(
                    NULL,
                    QString("Tomeo"),
                    QString("no videos found! download, unzip, and add command line argument to \"quoted\" file location. Download videos from Tom's OneDrive?"),
                    QMessageBox::Yes |
                    QMessageBox::No );

        switch( result )
        {
        case QMessageBox::Yes:
          QDesktopServices::openUrl(QUrl("https://leeds365-my.sharepoint.com/:u:/g/personal/scstke_leeds_ac_uk/EcGntcL-K3JOiaZF4T_uaA4BHn6USbq2E55kF_BTfdpPag?e=n1qfuN"));
          break;
        default:
            break;
        }
        exit(-1);
    }





    // the widget that will show the video (created in the play now)
//    QVideoWidget *videoWidget = new QVideoWidget;

    // the QMediaPlayer which controls the playback
    ThePlayer *player = new ThePlayer;

//    player->setVideoOutput(videoWidget);
//    videoWidget->layout()->setAlignment(Qt::AlignTop);
//    player->play();

    // a row of buttons
    QWidget *buttonWidget = new QWidget();
    // a list of the buttons
    vector<TheButton*> buttons;
    // the buttons are arranged horizontally
    //QHBoxLayout *layout = new QHBoxLayout();
    QVBoxLayout *layout = new QVBoxLayout();
    buttonWidget->setLayout(layout);


    //QLabel *upNextLabel = new QLabel(buttonWidget);
    //printf("number of vidoes &d", videos.size());


    QLabel *upNextTitle = new QLabel(buttonWidget);
    upNextTitle->setText("Playlist:");
    upNextTitle->setGeometry(0,0,20,20);
    upNextTitle->setStyleSheet("font-weight: bold; font: 30pt Arial Bold");
    layout->addWidget(upNextTitle);

    // create buttons for all videos
    for ( unsigned long long i = 0; i < videos.size()-1; i++ ) {
        TheButton *button = new TheButton(buttonWidget);
        button->setStyleSheet(
                    "text-align:left;"
                    );
        button->connect(button, SIGNAL(jumpTo(TheButtonInfo* )), player, SLOT (jumpTo(TheButtonInfo* ))); // when clicked, tell the player to play.
        buttons.push_back(button);
        layout->addWidget(button);
        button->init(&videos.at(i));
    }


    // tell the player what buttons and videos are available
    player->setContent(&buttons, & videos);

    // create the main window and layout
    QWidget window;
    QVBoxLayout *mainContainer = new QVBoxLayout();
    window.setLayout(mainContainer);
    window.setWindowTitle("Tomeo Editor");
    window.setMinimumSize(800, 680);

    QWidget *navigationWidget = new QWidget();
    //navigationWidget->setGeometry(0,0,window.width(),20);
    navigationWidget->setFixedSize(window.width(),50);
    QLabel *pageTitle = new QLabel(navigationWidget);
    pageTitle->setText("Tomeo Prototype");
    pageTitle->setGeometry(0,0,navigationWidget->width(),navigationWidget->height());
    pageTitle->setStyleSheet("font-weight: bold; font: 30pt Arial Bold");

    mainContainer->addWidget(navigationWidget);

    QWidget *playerDisplay = player->getDisplay();
    QHBoxLayout *videoPreviewContainer = new QHBoxLayout();
    // add the video and the buttons to the top level
    //videoPreviewContainer->addWidget(videoWidget);
    videoPreviewContainer->addWidget(playerDisplay);

    QScrollArea *scrollArea = new QScrollArea;
    scrollArea->setBackgroundRole(QPalette::Dark);
    scrollArea->setWidget(buttonWidget);
    scrollArea->setFixedWidth(295);
    videoPreviewContainer->addWidget(scrollArea);

    mainContainer->addLayout(videoPreviewContainer);
    // showtime!
    window.show();

    // wait for the app to terminate
    return app.exec();
}
