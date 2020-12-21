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
#include "classbtn.h"
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
#include<QLineEdit>
#include <QFileDialog>


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
            QFileInfo fi(f);
            QString text = fi.baseName() + ".txt";
            QString init = ":/initfile/" + fi.baseName() + ".txt";
            QFileInfo ff(text);
            QFileInfo fff(init);
            QString pos;
            QString time;
            QString dev;
            QString size;
            if(ff.exists()){
                QFile *file =new QFile(text);
                file->open(QIODevice::ReadOnly|QIODevice::Text);
                QTextStream stream( file );
                stream>>dev>>time>>pos>>size;
            }
            else if(fff.exists()){
                QFile *file =new QFile(init);
                file->open(QIODevice::ReadOnly|QIODevice::Text);
                QTextStream stream( file );
                stream>>dev>>time>>pos>>size;
            }
            else{
                QFile *file =new QFile(text);
                file->open(QIODevice::WriteOnly|QIODevice::Text);
                QTextStream stream( file );
                pos = "null";
                time = "null";
                dev = "null";
                size = "null";
                stream<<dev<<" "<<time<<" "<<pos<<" "<<size;
            }

            if (QFile(thumb).exists()) { // if a png thumbnail exists
                QImageReader *imageReader = new QImageReader(thumb);
                    QImage sprite = imageReader->read(); // read the thumbnail
                    if (!sprite.isNull()) {
                        QIcon* ico = new QIcon(QPixmap::fromImage(sprite)); // voodoo to create an icon for the button
                        QUrl* url = new QUrl(QUrl::fromLocalFile( f )); // convert the file location to a generic url
                        out . push_back(TheButtonInfo( url , ico , dev ,time , pos , size ) ); // add to the output list
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
                        out . push_back(TheButtonInfo( url , ico , dev ,time , pos , size ) );// add to the output list
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

    // the QMediaPlayer which controls the playback
    ThePlayer *player = new ThePlayer;

    // a row of buttons
    QWidget *buttonWidget = new QWidget();
    // a list of the buttons
    vector<TheButton*> buttons;
    // the buttons are arranged vertically
    QVBoxLayout *layout = new QVBoxLayout();
    buttonWidget->setLayout(layout);

    QWidget *classwidget = new QWidget();
    QVBoxLayout *classlayout = new QVBoxLayout();
    classwidget->setLayout(classlayout);
    classBtn *allBtn = new classBtn();
    classBtn *devBtn = new classBtn();
    classBtn *posBtn = new classBtn();
    QHBoxLayout *searchlayout = new QHBoxLayout();
    QPushButton *search = new QPushButton();
    search->setText("Search");
    QLineEdit *edit = new QLineEdit();
    edit->setPlaceholderText("Name,Device or Position");
    searchlayout->addWidget(edit);
    searchlayout->addWidget(search);

    QLabel *upNextTitle = new QLabel(buttonWidget);
    upNextTitle->setText(" Playlist:");
    upNextTitle->setStyleSheet("font-weight: bold; font: 30pt Arial Bold;background:#b5b5b5");
    classlayout->addWidget(upNextTitle);

    QScrollArea *scrollArea = new QScrollArea;
    scrollArea->setBackgroundRole(QPalette::Dark);
    scrollArea->setWidget(buttonWidget);
    scrollArea->setFixedWidth(295);
    scrollArea->setWidgetResizable(true);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    allBtn->setText("All");
    classlayout->addWidget(scrollArea);
    classlayout->addWidget(allBtn);
    classlayout->addWidget(devBtn);
    classlayout->addWidget(posBtn);
    classlayout->addLayout(searchlayout);


    // create the main window and layout
    QWidget window;
    QVBoxLayout *mainContainer = new QVBoxLayout();
    window.setLayout(mainContainer);
    window.setWindowTitle("Tomeo Editor");
    window.setMinimumSize(1000, 680);


    QWidget *navigationWidget = new QWidget();
    navigationWidget->setFixedSize(window.width(),40);
    QLabel *pageTitle = new QLabel(navigationWidget);
    pageTitle->setText("Tomeo Prototype");
    pageTitle->setGeometry(0,0,navigationWidget->width(),navigationWidget->height());
    pageTitle->setStyleSheet("font-weight: bold; font: 20pt Arial Bold");
    mainContainer->addWidget(navigationWidget);


    QWidget *playerDisplay = player->getDisplay();
    QHBoxLayout *videoPreviewContainer = new QHBoxLayout();
    // add the video and the buttons to the top level
    videoPreviewContainer->addWidget(playerDisplay);
    videoPreviewContainer->addWidget(classwidget);

    mainContainer->addLayout(videoPreviewContainer);
    // showtime!
    window.show();

    QObject::connect(player,&ThePlayer::sigOpenList,[=](bool flag){
        classwidget->setVisible(!flag);
    });

    // collect all the videos in the folder
    vector<TheButtonInfo> videos;

    if (argc == 2)
        videos = getInfoIn( string(argv[1]));

    if (videos.size() == 0) {
        const int result = QMessageBox::question(0,"Tomeo","There is no video in the folder, please select the folder manually");
        if(result == QMessageBox::Yes)
        {
            QString path = QFileDialog::getExistingDirectory(0);
            if(path != "")  videos = getInfoIn(path.toStdString());
            else            exit(-1);

        }
        else    exit(-1);
    }


    // create buttons for all videos
    for ( int i = 0; i < static_cast<int>(videos.size()); i++ ) {
        TheButton *button = new TheButton(buttonWidget);
        button->setStyleSheet(
                    "text-align:left;"
                    );
        button->connect(button, SIGNAL(jumpTo(TheButtonInfo* )), player, SLOT (jumpTo(TheButtonInfo* ))); // when clicked, tell the player to play.
        button->connect(button, SIGNAL(jumpTo(TheButtonInfo* )), devBtn, SLOT (devto(TheButtonInfo* )));
        button->connect(button, SIGNAL(jumpTo(TheButtonInfo* )), posBtn, SLOT (posto(TheButtonInfo* )));
        button->connect(button,&TheButton::right,[button,devBtn,posBtn](){
            QDialog *mainWindow = new QDialog;
            QGridLayout *gridLayout = new QGridLayout;
            gridLayout->setMargin(15);
            gridLayout->setColumnMinimumWidth(2, 15);
            QLabel *lbl1 = new QLabel(QWidget::tr("Device:"));
            QLineEdit *edit1 = new QLineEdit;
            edit1->setText(button->info->dev);
            QLabel *lbl2 = new QLabel(QWidget::tr("Position:"));
            QLineEdit *edit2 = new QLineEdit;
            edit2->setText(button->info->pos);
            QPushButton *okBtn = new QPushButton(QWidget::tr("OK"), mainWindow);
            QPushButton *calBtn = new QPushButton(QWidget::tr("Cancel"), mainWindow);
            gridLayout->addWidget(lbl1, 0, 0);
            gridLayout->addWidget(edit1, 0, 1);
            gridLayout->addWidget(lbl2, 2, 0);
            gridLayout->addWidget(edit2, 2, 1);
            gridLayout->addWidget(okBtn, 4, 0);
            gridLayout->addWidget(calBtn, 4, 2);
            mainWindow->setLayout(gridLayout);
            mainWindow->resize(400, 150);
            mainWindow->setWindowTitle(QWidget::tr("Update"));
            mainWindow->show();
            okBtn->connect(okBtn,&QPushButton::released,[button,edit1,edit2,mainWindow,devBtn,posBtn](){
                        button->info->dev = edit1->text();
                        button->info->pos = edit2->text();
                        QString f = button->info->url->toString();
                        QFileInfo fi(f);
                        f = fi.baseName() + ".txt";
                        QFile *file =new QFile(f);
                        file->open(QIODevice::WriteOnly|QIODevice::Text);
                        QTextStream stream( file );
                        stream<<button->info->dev<<" "<<button->info->time<<" "<<button->info->pos<<" "<<button->info->size;
                        file->close();
                        devBtn->devto(button->info);
                        posBtn->posto(button->info);
                        mainWindow->close();
                    });
           calBtn->connect(calBtn,&QPushButton::released,[mainWindow](){
                        mainWindow->close();
                    });
           });
        allBtn->connect(allBtn,&QPushButton::released,[button](){
                button->setVisible(true);
        });
        devBtn->connect(devBtn,&QPushButton::released,[devBtn,button](){
            if(button->info->dev == devBtn->text())
                button->setVisible(true);
            else
                button->setVisible(false);
        });
        posBtn->connect(posBtn,&QPushButton::released,[posBtn,button](){
            if(button->info->pos == posBtn->text())
                button->setVisible(true);
            else
                button->setVisible(false);
        });
        search->connect(search,&QPushButton::released,[edit,button](){
                    QString path = button->info->url->toString();
                    QFileInfo file(path);
                    QString name = file.baseName();
                    if(button->info->dev == edit->text() || button->info->pos == edit->text() || name == edit->text() || edit->text()=="" )
                        button->setVisible(true);
                    else
                        button->setVisible(false);
                });
        buttons.push_back(button);
        layout->addWidget(button);
        button->init(&videos.at(i));
    }
    // tell the player what buttons and videos are available
    player->setContent(&buttons, & videos);
    devBtn->setText(player->getInfo()->dev);
    posBtn->setText(player->getInfo()->pos);
    // wait for the app to terminate
    return app.exec();
}
