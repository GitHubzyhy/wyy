#include "messageitem.h"
#include "ui_messageitem.h"

MessageItem::MessageItem(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MessageItem)
{
    ui->setupUi(this);

    setAttribute(Qt::WA_StyledBackground);//渲染背景
    this->setStyleSheet("QWidget{background:transparent; border-radius:8px;}"
                        "QWidget:hover{background:transparent;}");
    ui->labNameMess->setStyleSheet("QLabel#labNameMess{color:rgb(91,133,180);}");
    ui->labTimer->setStyleSheet("QLabel#labTimer{color:rgb(217,217,217);}");
    ui->labInform->setStyleSheet("QLabel#labNameMess{color:rgb(49,49,49);}");
}

MessageItem::~MessageItem()
{
    delete ui;
}
//设置是否有红点flag true 为红 false 为false
void MessageItem::setRed(bool flag)
{
    if (flag)
        ui->labred->setPixmap(QPixmap(":/message/images/message/red.png"));
    else
        ui->labred->setPixmap(QPixmap(""));//空的QPixmap对象
}
//设置用户图片路径
void MessageItem::setStringPic(QString url)
{
    QString str = "QWidget{border-image:url(" + url + ");}";
    ui->widPicturePer->setStyleSheet(str);
}
//设置用户名字
void MessageItem::setName(QString str)
{
    ui->labNameMess->setText(str);
}
//设置时间
void MessageItem::setTime(QString str)
{
    ui->labTimer->setText(str);
}
//设置说明
void MessageItem::setInformMessage(QString str)
{
    ui->labInform->setText(str);
}
//设置类型
void MessageItem::setmType(int type)
{
    mType = type;
}
