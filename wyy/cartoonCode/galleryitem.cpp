#include "galleryitem.h"
#include "ui_galleryitem.h"

GalleryItem::GalleryItem(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::GalleryItem)
{
    ui->setupUi(this);

    //去除滚动条
    ui->textEdit->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->textEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    ui->itemPic->setAutoFillBackground(true); //用调色板需要用到
}

GalleryItem::~GalleryItem()
{
    delete ui;
}

void GalleryItem::setMessage(QString str)
{
    ui->textEdit->setText(str);
}

void GalleryItem::setPicture(QString url)
{
    QPalette palette = ui->itemPic->palette(); //获取该控件的调色板
    palette.setBrush(QPalette::Window, QBrush(QPixmap(url))); //设置背景
    ui->itemPic->setPalette(palette);
}
