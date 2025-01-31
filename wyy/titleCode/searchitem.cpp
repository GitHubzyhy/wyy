#include "searchitem.h"
#include "ui_searchitem.h"

SearchItem::SearchItem(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SearchItem)
{
    ui->setupUi(this);

    this->setStyleSheet("QLabel{background:transparent;}");
    ui->lab_name->setStyleSheet("QLabel{color:rgb(51,51,51);}");
    ui->lab_ranking->setStyleSheet("QLabel{color:rgb(204,204,204);}");
    ui->lab_message->setStyleSheet("QLabel{color:rgb(153,153,153);}");
}

SearchItem::~SearchItem()
{
    delete ui;
}
//排行号样式设置
void SearchItem::setLabNumColor(bool bo)
{
    if (!bo)
        ui->lab_num->setStyleSheet("QLabel{color:rgb(255,61,61)}");
    else
        ui->lab_num->setStyleSheet("QLabel{color:rgb(222,222,222)}");
}
//排行号文本设置
void SearchItem::setLabNumText(QString str)
{
    ui->lab_num->setText(str);
}
//热榜歌曲名
void SearchItem::setLabNameText(QString str)
{
    ui->lab_name->setText(str);
}
//其他
void SearchItem::setLabrankingText(QString str)
{
    ui->lab_ranking->setText(str);
}
//歌曲介绍
void SearchItem::setLabMessageText(QString str)
{
    ui->lab_message->setText(str);
}

void SearchItem::mouseDoubleClickEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    emit musicText(ui->lab_name->text());
}
