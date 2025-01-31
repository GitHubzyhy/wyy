#ifndef MESSAGEFORM_H
#define MESSAGEFORM_H

#include <QWidget>

namespace Ui
{
    class MessageForm;
}

class MessageForm : public QWidget
{
    Q_OBJECT

public:
    explicit MessageForm(QWidget *parent = nullptr);
    ~MessageForm();

    void setButttonGroup();//分组管理多个按钮
    void setMessageId(int id); //处理按键数据
    void setListMessage(); //设置列表消息

    int getListItem();//获取有多少条目为了主窗口设置消息数量

signals:
    void messageIsNull(bool flag);

private:
    Ui::MessageForm *ui;

    QList<int> unread;//记录未读条目序号
    // QWidget interface
protected:
    virtual void leaveEvent(QEvent *event) override;
private slots:
    void on_btnRead_clicked();
};

#endif // MESSAGEFORM_H
