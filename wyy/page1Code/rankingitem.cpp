#include "rankingitem.h"
#include "ui_rankingitem.h"

RankingItem::RankingItem(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::RankingItem)
{
    ui->setupUi(this);
}

RankingItem::~RankingItem()
{
    delete ui;
}

//设置第一数据，false 为普通颜色, true为红色
void RankingItem::setNum(int num, bool b)
{
    ui->labNum->setText(QString::number(num));
    if (b == true)
        ui->labNum->setStyleSheet("QLabel{color:rgb(255,61,61);background:transparent;}");
    else
        ui->labNum->setStyleSheet("QLabel{color:rgb(222,222,222;background:transparent;)}");
}
//设置第二个数据
// 1. 第一种情况
void RankingItem::setStringOrPic(QString str)
{
    ui->labPicOrText->setText(str);
    ui->labPicOrText->setStyleSheet("QLabel{color:rgb(152,152,152);background:transparent;}");
}
// 2. 第二种情况
void RankingItem::setStringOrPic(int num)
{
    // 0 加载上  1加载下 2加载-
    switch (num)
    {
        case 0:
            ui->labPicOrText->setStyleSheet("QLabel{border-image:url(:/ranking/images/ranking/up.png);}");
            break;
        case 1:
            ui->labPicOrText->setStyleSheet("QLabel{border-image:url(:/ranking/images/ranking/down.png);}");
            break;
        case 2:
            ui->labPicOrText->setStyleSheet("QLabel{border-image:url(:/ranking/images/ranking/nor.png);}");
            break;
    }
}
//设置歌名
void RankingItem::setMusicName(QString strName)
{
    ui->labCon->setText(strName);
}
//设置作者
void RankingItem::setMusicAutor(QString name)
{
    ui->labEnd->setText(name);
}
