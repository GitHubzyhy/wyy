#include "messageform.h"
#include "ui_messageform.h"
#include <QScrollBar>
#include <QButtonGroup>
#include "messageitem.h"
#include <QDebug>

MessageForm::MessageForm(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MessageForm)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_StyledBackground); //设置样式表

    QScrollBar *sv = ui->listMessage->verticalScrollBar();
    sv->setStyleSheet("QScrollBar:vertical{background:transparent; width:9px;}"
                      "QScrollBar::handle:vertical{min-height:10px;max-height:20px;background:rgb(224,224,224); width:6px; border-radius:3px;}"
                      "QScrollBar::up-arrow:vertical{border:none;}"
                      "QScrollBar::sub-line:vertical{background:transparent;}"
                      "QScrollBar::add-line:vertical{background:transparent;}");

    setButttonGroup();//设置按钮
    setListMessage();//设置列表消息
}

MessageForm::~MessageForm()
{
    delete ui;
}
//分组管理多个按钮
void MessageForm::setButttonGroup()
{
    QButtonGroup *btnGroup = new QButtonGroup(this);
    //将按钮添加到按钮组中并且指定ID
    btnGroup->addButton(ui->ptnPer, 0);
    btnGroup->addButton(ui->ptnComment, 1);
    btnGroup->addButton(ui->ptnMe, 2);
    btnGroup->addButton(ui->ptnInform, 3);

    btnGroup->setExclusive(true);//设置互斥
    btnGroup->button(0)->setChecked(true);//设置按钮被选中

    //因为这里信号有重载所以必须指定参数类型
    connect(btnGroup, &QButtonGroup::idClicked,
            this, [this](int id)
    {
        setMessageId(id);
    });
}
//处理按键数据
void MessageForm::setMessageId(int id)
{
    switch (id)
    {
        //功能未编写
        default:
            break;
    }
}
//设置列表消息
void MessageForm::setListMessage()
{

    //加载图片路径 名字 消息 时间
    QStringList pictureList;
    for (int i = 0; i < 11; ++i)
    {
        QString path = QString::asprintf(":/message/images/message/per%d.png", i);
        pictureList << path;
    }

    QVector<bool> flag = {true, true, false, true, false, false, true, false, true, false, false};
    QStringList perNameList = {"云音乐管家", "云贝小助手", "云音乐小秘书", "网易云音乐",
                               "云音乐福利", "吉法师", "ICY", "九月", "沈以诚",
                               "龚建波", "默片"
                              };
    QStringList timerList = {"2024年9月25日", "2024年9月15日", "2024年9月4日", "2024年6月3日",
                             "2024年4月24日", "2024年4月12日", "2024年3月27日", "2024年3月10日",
                             "2024年2月18日", "2024年2月5日", "2024年1月1日"
                            };

    QStringList messageInformList = {"通知：恭喜你获得1天网易云音乐黑椒VIP",
                                     "通知：中心完成了任务，送你更好的东西",
                                     "通知：你的账号已经在别的地方登录了，希望你快点解决",
                                     "通知：不开VIP还听啥歌，开了也听不到啥歌",
                                     "通知：恭喜你成功达到8级，继续加油",
                                     "通知：分享专辑《First Heartbreak》发布了",
                                     "通知：分享专辑《Amazing Grace》发布了",
                                     "通知：我的新歌《11》正式发布啦！",
                                     "通知：别不承认！这就是疫情期间宅家的你！",
                                     "通知：被封印在家，到底要怎样才能快乐起来？",
                                     "通知：特殊时期，我们也要拒绝丧气，元气抗疫！"
                                    };

    for (int i = 0; i < 11; i++)
    {
        QListWidgetItem *item = new QListWidgetItem(ui->listMessage);

        MessageItem *messItem = new MessageItem(this);

        messItem->setRed(flag[i]);
        messItem->setStringPic(pictureList[i]);
        messItem->setName(perNameList[i]);
        messItem->setTime(timerList[i]);
        messItem->setInformMessage(messageInformList[i]);

        ui->listMessage->setItemWidget(item, messItem);//指定显示子控件
        if (flag[i])//记录哪些是未读消息
            unread.append(i);
    }
}
//获取有多少条目为了主窗口设置消息数量
int MessageForm::getListItem()
{
    return ui->listMessage->count();
}

void MessageForm::leaveEvent(QEvent *event)
{
    Q_UNUSED(event);
    close();//鼠标离开this就关闭
}
//全部已读
void MessageForm::on_btnRead_clicked()
{
    if (unread.isEmpty()) return;
    for (int i = 0; i < unread.size(); ++i)
    {
        QListWidgetItem *item = ui->listMessage->item(unread[i]);
        MessageItem *unReadItem = static_cast<MessageItem *>(ui->listMessage->itemWidget(item));
        unReadItem->setRed(false);
    }
    unread.clear();//清空容器
    emit messageIsNull(true);
}
