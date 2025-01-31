#include "musicform.h"
#include "ui_musicform.h"

MusicForm::MusicForm(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MusicForm)
{
    ui->setupUi(this);
}

MusicForm::~MusicForm()
{
    delete ui;
}

void MusicForm::setMusicName(QString musicName)
{
    ui->labMName->setText(musicName);
    Name = musicName;
}

void MusicForm::setMusicAuthor(QString musicAuthor)
{
    ui->labMuAuthor->setText(musicAuthor);
    Author = musicAuthor;
}

void MusicForm::setMusicPicture(QString url)
{
    QString str = "QPushButton{border-image:url(" + url + ");"
                  "border-radius:12px;}";
    ui->ptnGetMusic->setStyleSheet(str);
    ui->pushButton->setStyleSheet("QPushButton{border-image:url(:/bottom/images/bottom/like.png);}"
                                  "QPushButton:checked{border-image:url(:/bottom/images/bottom/like2.png);}");
}
