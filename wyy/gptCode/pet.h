#ifndef PET_H
#define PET_H

#include <QWidget>
#include <QList>
#include <QMap>
#include <QUrl>
#include <QEvent>
#include <QMouseEvent>

namespace Act
{
    Q_NAMESPACE//标识命名空间

    enum RoleAct
    {
        Sleep,
        SayHello
    };
}

using namespace Act;

class Pet : public QWidget
{
    Q_OBJECT
public:
    explicit Pet(QWidget *parent = nullptr);

    void loadRoleActRes();//加载素材
    void showActAnimation(RoleAct k);//设置动作
    void ckAct();//切换动作
signals:
    void doubleClick(bool flag);
private:
    QMap<RoleAct, QList<QUrl> > action_map;//动作和图片路径对应
    QTimer *timer;
    RoleAct cur_role_act;//当前动作
    QUrl cur_role_pix;//当前动作对应的图片

    bool flag;
    // QWidget interface
protected:
    virtual void paintEvent(QPaintEvent *event) override;
    virtual void mouseDoubleClickEvent(QMouseEvent *event) override;
};

class DragFliter: public QObject
{
    // QObject interface
public:
    virtual bool eventFilter(QObject *watched, QEvent *event) override
    {
        auto w = dynamic_cast<QWidget *>(watched); //转换成窗口类型
        if (!w) return false;//返回false这个事件就会继续向下传递
        if (event->type() == QEvent::MouseButtonPress)
        {
            auto e = dynamic_cast<QMouseEvent *>(event);//父类向子类转换
            if (e) //转换成功
                pos = e->pos();
        }
        else if (event->type() == QEvent::MouseMove)
        {
            auto e = dynamic_cast<QMouseEvent *>(event);
            if (e->buttons()&Qt::LeftButton)
                w->move(e->globalPos() - pos); //实现拖动
        }
        return QObject::eventFilter(watched, event);//调用父类让他去处理其他事件
    }
private:
    QPoint pos;
};
#endif // PET_H
