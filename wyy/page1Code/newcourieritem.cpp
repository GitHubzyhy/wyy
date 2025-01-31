#include "newcourieritem.h"
#include "ui_newcourieritem.h"

NewCourierItem::NewCourierItem(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::NewCourierItem)
{
    ui->setupUi(this);
}

NewCourierItem::~NewCourierItem()
{
    delete ui;
}
//设置id
void NewCourierItem::setNum(int id)
{
    ui->labMusicNum->setNum(id);
}
//设置图片
void NewCourierItem::setPicture(QString url)
{
    QString str = "QWidget{border-image:url(" + url + ");}";
    ui->widMusicPic->setStyleSheet(str);
}
//设置歌名
void NewCourierItem::setMusicName(QString musicName)
{
    ui->MusicName->setText(musicName);
}
//设置作者
void NewCourierItem::setMusicAuthor(QString author)
{
    ui->MusicAuthor->setText(author);
}
//设置专辑
void NewCourierItem::setMusicAlbum(QString name)
{
    ui->MusicAlbum->setText(name);
}
//设置歌曲时长
void NewCourierItem::setMusicTimer(QString strTimer)
{
    ui->MusicTimer->setText(strTimer);
}
