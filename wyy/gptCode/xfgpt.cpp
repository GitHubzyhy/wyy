#include "xfgpt.h"
#include "qbuttongroup.h"
#include "ui_xfgpt.h"
#include <QBoxLayout>
#include <QScrollBar>
#include <QDebug>
#include <QLabel>
#include <QThread>

Xfgpt::Xfgpt(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Xfgpt)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_StyledBackground);//背景渲染

    //询问gpt
    QThread *thread = new QThread;
    gptThread.moveToThread(thread); //把GptThread的对象移动到线程中执行
    thread->start();//启动线程
    //绑定查询槽函数
    connect(this, &Xfgpt::query,
            &gptThread, &GptThread::work);

    //获取返回的信息
    connect(&gptThread, &GptThread::sendAns,
            this, [this](QString str)
    {
        appendEdit(str);
    });

    //滚动区域扩大后让滚动条滑到最下面
    connect(ui->scrollArea->verticalScrollBar(), &QScrollBar::rangeChanged, this, [this]()
    {
        ui->scrollArea->verticalScrollBar()->setValue(ui->scrollArea->verticalScrollBar()->maximum());
    });

    //按照提示内容查询
    QButtonGroup *btnGroup = new QButtonGroup(this);
    btnGroup->addButton(ui->btn1, 1);
    btnGroup->addButton(ui->btn2, 2);
    btnGroup->addButton(ui->btn3, 3);
    btnGroup->addButton(ui->btn4, 4);
    connect(btnGroup, &QButtonGroup::idClicked,
            this, &Xfgpt::tipQuery);

}

Xfgpt::~Xfgpt()
{
    delete ui;
}
//回复界面创建
void Xfgpt::answerEdit()
{
    oldHeight = 4;

    QWidget *widget = new QWidget(ui->scrollArea);
    widget->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));

    QVBoxLayout *vLayout = new QVBoxLayout;
    QLabel *headLabel = new QLabel(widget);
    headLabel->setFixedSize(60, 60);
    headLabel->setStyleSheet("border-image:url(:/gpt/images/gpt/gpt.png);"
                             "border-radius:30px;");

    vLayout->addWidget(headLabel);
    vLayout->addSpacerItem(new QSpacerItem(30, 30, QSizePolicy::Fixed, QSizePolicy::Expanding));
    vLayout->setContentsMargins(0, 0, 0, 0);

    editAns = new QPlainTextEdit(widget);
    editAns->setReadOnly(true);
    editAns->setStyleSheet("background-color: rgb(243, 245, 255);"
                           "border-radius: 14%;"
                           "font-weight:bold;"
                           "font-size: 15px;");

    QHBoxLayout *hLayout = new QHBoxLayout(widget);
    hLayout->addLayout(vLayout);
    hLayout->addWidget(editAns);

    ui->centLayout->addWidget(widget);

    editAns->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    deleteLater << widget;//后续销毁
}
//回复内容
void Xfgpt::appendEdit(QString str)
{
    QTextCursor cursor = editAns->textCursor();
    cursor.movePosition(QTextCursor::End); // 移动光标到文本末尾
    cursor.insertText(str); // 在光标处插入文本

    // 创建一个 QFont 对象并设置字体大小
    // QFont font = editAns->font(); // 假设你是在 ui->widget 中设置了 QSS
    // font.setPixelSize(15); // 确保与 QSS 中的 font-size: 15px 对应
    // font.setWeight(QFont::Bold);

    // // 使用该字体创建 QFontMetrics 对象
    // QFontMetrics fontMetrics(font);
    // qDebug() << fontMetrics.size(Qt::TextSingleLine, str);
    // qDebug() << fontMetrics.boundingRect(str);
    //适当调整edit的大小
    QSize documentSize = editAns->document()->size().toSize();
    int newHeight = documentSize.height();
    if (newHeight > oldHeight)
    {
        editAns->setFixedSize(editAns->width(), editAns->height() + 22);
        oldHeight = newHeight;
        qDebug() << "调整了";
    }
}
//设置歌曲名和歌手
void Xfgpt::setNaAu(QString Name, QString Author)
{
    name = Name;
    author = Author;
}
//按照提示查询
void Xfgpt::tipQuery(int id)
{
    answerEdit();//创建回复对话框
    QString str = "歌名：" + name + "歌手" + author;
    switch (id)
    {
        case 1:
            str = str + "音乐风格和元素";
            break;
        case 2:
            str = str + "个人感受与共鸣";
            break;
        case 3:
            str = str + "介绍下这首歌";
            break;
        case 4:
            str = str + "歌曲的创作背景";
            break;
    }
    emit query(str);
}
//发送提问
void Xfgpt::on_btnSend_clicked()
{
    QWidget *widget = new QWidget(ui->scrollAreaWidgetContents);

    QVBoxLayout *vLayout = new QVBoxLayout;
    QLabel *headLabel = new QLabel(widget);
    headLabel->setFixedSize(60, 60);
    headLabel->setStyleSheet("border-image:url(:/title/images/title/head.jpg);"
                             "border-radius:30px;");

    vLayout->addWidget(headLabel);
    vLayout->addSpacerItem(new QSpacerItem(30, 30, QSizePolicy::Fixed, QSizePolicy::Expanding));
    vLayout->setContentsMargins(0, 0, 0, 0);

    QPlainTextEdit *edit = new QPlainTextEdit(ui->plainTextEdit->toPlainText());
    edit->setReadOnly(true);
    edit->setStyleSheet("background-color: #0099ff;"
                        "color: white;"
                        "font-weight: bold;"
                        "font-family: \"宋体\";"
                        "border-radius: 14%;");
    edit->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    QHBoxLayout *hLayout = new QHBoxLayout(widget);
    hLayout->addSpacerItem(new QSpacerItem(30, 30, QSizePolicy::Expanding, QSizePolicy::Fixed));
    hLayout->addWidget(edit);
    hLayout->addLayout(vLayout);

    ui->centLayout->addWidget(widget);

    deleteLater << widget;//后续销毁
    emit query(ui->plainTextEdit->toPlainText());
    ui->plainTextEdit->clear();
    answerEdit();//创建回复对话框
}
//清空对话
void Xfgpt::on_btnClear_clicked()
{
    for (int i = 0; i < deleteLater.count(); ++i)
    {
        delete deleteLater[i];
        deleteLater[i] = nullptr;
    }
}

void Xfgpt::on_btnQuit_clicked()
{
    emit isClose();
    close();
}
