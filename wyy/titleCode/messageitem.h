#ifndef MESSAGEITEM_H
#define MESSAGEITEM_H

#include <QWidget>

namespace Ui
{
    class MessageItem;
}

class MessageItem : public QWidget
{
    Q_OBJECT

public:
    explicit MessageItem(QWidget *parent = nullptr);
    ~MessageItem();

    void setRed(bool flag);//设置是否有红点flog true 为红 false 为false
    void setStringPic(QString url); //设置用户图片路径
    void setName(QString str);//设置用户名字
    void setTime(QString str);//设置时间
    void setInformMessage(QString str); //设置说明
    void setmType(int type);//设置类型

private:
    Ui::MessageItem *ui;
    //判断该消息是什么类型
    int mType;//0. 私信 1. 评论 2. @我 3. 通知
};

#endif // MESSAGEITEM_H
