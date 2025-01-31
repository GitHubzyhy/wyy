#include "musicitem.h"
#include "ui_musicitem.h"

MusicItem::MusicItem(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MusicItem)
{
    ui->setupUi(this);
}

MusicItem::~MusicItem()
{
    delete ui;
}
//设置id
void MusicItem::setNum(int id)
{
    if (id < 10 && id > 0)
        ui->lbNum->setText(QString("%1").arg(QString::number(id), 2, '0'));
    else
        ui->lbNum->setNum(id);

    if (id % 2 == 0)
        ui->lbLog->setStyleSheet("border-image: url(:/NewMusic/images/NewMusic/sq1.jpg);");
}
//歌曲名
void MusicItem::setMusicName(QString musicName)
{
    ui->lbMusicName->setText(musicName);
}
//歌手
void MusicItem::setMusicAuthor(QString author)
{
    ui->lbSinger->setText(author);
}
//歌曲时间或者上次播放时间
void MusicItem::setMusicTimer(QString strTimer)
{
    ui->lbTime->setText(strTimer);
}
//属于哪个语种
void MusicItem::setLanguage(int value)
{
    language = value;
}
//返回当前语种
int MusicItem::getLanguage()
{
    return language;
}
