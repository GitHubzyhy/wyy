#include "pet.h"
#include <QPainter>
#include <QTimer>
#include <QDebug>

#define PetSize QSize(209,238)

Pet::Pet(QWidget *parent)
    : QWidget{parent}
{
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);//去除窗口边框
    setAttribute(Qt::WA_TranslucentBackground, true);//将窗口的背景设置为半透明
    resize(PetSize.width(), PetSize.height());

    installEventFilter(new DragFliter);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [this]()
    {
        static int index = 0; //记录显示动作图片的当前索引(持久性)
        QList<QUrl> paths = action_map.value(cur_role_act);
        cur_role_pix = paths[index++ % paths.size()];

        update();
    });
    loadRoleActRes();//加载资源
    showActAnimation(RoleAct::Sleep);//显示动作

    flag = false;
}
//加载素材
void Pet::loadRoleActRes()
{
    auto addRes = [this](RoleAct k, QString path, int count) //动作,路径,图片个数
    {
        QList<QUrl> paths;
        char buf[260];//路径最大长度为260位
        for (int i = 0; i < count; ++i)
        {
            memset(buf, 0, sizeof(buf)); //初始化为0
            sprintf_s(buf, path.toStdString().c_str(), i);//将整数i插入到字符串path中，并将结果存储在字符数组buf中
            paths << QUrl::fromLocalFile(buf);
        }
        action_map.insert(k, paths); //完成动作和图片的映射关系
    };
    //开始加载图片
    addRes(RoleAct::SayHello, ":/gpt/images/gpt/sayHello/sayHello_%d.png", 28);
    addRes(RoleAct::Sleep, ":/gpt/images/gpt/sleep/sleep_%d.png", 25);
}
//设置动作
void Pet::showActAnimation(RoleAct k)
{
    timer->stop();
    this->cur_role_act = k;
    timer->start(90);
}

void Pet::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);//在当前窗口绘图

    QPixmap pix;
    pix.load(cur_role_pix.toLocalFile());//将url转换成本地路径

    painter.drawPixmap(0, 0, pix.scaled(PetSize,//将图片绘制在左上角
                                        Qt::KeepAspectRatio, Qt::SmoothTransformation));
}
//切换动作
void Pet::ckAct()
{
    flag = flag == false ? true : false;
    if (flag)
        showActAnimation(RoleAct::SayHello);//显示动作
    else
        showActAnimation(RoleAct::Sleep);//显示动作
}
void Pet::mouseDoubleClickEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    ckAct();

    emit doubleClick(flag);
}
