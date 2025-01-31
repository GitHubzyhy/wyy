#include "wyy.h"
#include "ui_wyy.h"
#include "musicform.h"
#include <QMouseEvent>
#include "searchform.h"
#include <QButtonGroup>
#include <QDebug>
#include <QStackedLayout>
#include <QStandardItemModel>
#include "changecolorform.h"
#include "loadfileqss.h"
#include "messageform.h"
#include "personform.h"
#include "picturewidget.h"
#include "galleryprivate.h"
#include "rankingitem.h"
#include "newcourier.h"
#include "newdisc.h"
#include "voice.h"
#include "xfgpt.h"
#include "pet.h"
#include "musicitem.h"
#include <QStandardItemModel>
#include <QMessageBox>
#include "closemes.h"
#include "SongDatabase.h"
#include "onlinemp3.h"
#include <QSystemTrayIcon>
#include <QMenu>
#include <QTime>

Wyy::Wyy(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Wyy)
{
    ui->setupUi(this);
    //首先去除自带的边框
    this->setWindowFlag(Qt::FramelessWindowHint);
    installEventFilter(this);//安装事件过滤器

    //初始化左下角模块
    setMusicLeft();
    //加入过滤器
    ui->line_Search->installEventFilter(this);
    searchForm = new SearchForm(this); //创建一个新的搜索框
    searchForm->installEventFilter(this);
    searchForm->setMouseTracking(true);
    searchForm->hide();
    connect(searchForm, &SearchForm::MuName, this, [this](QString str)
    {
        ui->line_Search->setText(str);
        on_pushButton_search_clicked();
    });

    //切换皮肤
    changeColor = new ChangeColorForm(this);
    changeColor->hide();
    connect(changeColor, &ChangeColorForm::colorId,
            this, &Wyy::changeSkin);
    //个人消息
    pMessageForm = new MessageForm(this);
    pMessageForm->hide();//隐藏

    int cnt = pMessageForm->getListItem();
    if (cnt >= 100)
        cnt = 99;
    ui->label_message_Tip->setText(QString::number(cnt));//设置有多少条消息

    connect(pMessageForm, &MessageForm::messageIsNull, this, [this](bool flag)
    {
        if (flag)
            ui->label_message_Tip->hide();
    });

    //个人中心
    p_person = new personForm(this);
    p_person->hide();

    setPictureWall();//轮播图
    setGallery(); //添加画廊部分
    setButtonMus(); //歌单部分
    setRankData();//设置排行榜数据
    setRankNew(); //设置新歌榜
    setButtonGroup();//歌手

    /*最新音乐模块*/
    setAddWidget();//加载到堆栈布局里
    setNewMusicButton();//设置新歌速递与新蝶上架模块
    setNewMusicChooseButton();//全部 华语 欧美 韩国 日本

    //接入gpt
    gpt = new Xfgpt(this);
    gpt->hide();
    pet = new Pet();
    pet->show();
    pet->move(width() / 2, height() / 2);
    connect(pet, &Pet::doubleClick, this, [this](bool flag)
    {
        if (flag)
        {
            int gptWidth = width() / 2;
            int gptHeight = height() - ui->title_Widget->height();
            gpt->setGeometry(gptWidth + 100, ui->title_Widget->height(), gptWidth - 100, gptHeight);

            gpt->setNaAu(musicForm->getName(), musicForm->getAuthor());
            gpt->raise();//提升层次
            gpt->show();

            this->raise();//让主窗口显示出来
        }
        else
            gpt->hide();
    });
    connect(gpt, &Xfgpt::isClose, this, [this]
    {
        pet->ckAct();//变成睡觉
    });

    //设置tabWidget和stackedWidget默认显示哪一页，和相关按钮交互
    ui->tabWidget->setCurrentIndex(0);
    QButtonGroup *btnGroup = new QButtonGroup(this);
    btnGroup->addButton(ui->pushButton_title_left, 0);
    btnGroup->addButton(ui->pushButton_title_right, 1);
    connect(btnGroup, &QButtonGroup::idClicked,
            ui->stackedWidget, &QStackedWidget::setCurrentIndex);
    ui->stackedWidget->setCurrentIndex(0);

    songDatabase = SongDatabase::getInstance(); //获取单例对象
    songDatabase->initDataBase();//初始化数据库

    model = new QStandardItemModel;
    ui->listView->setModel(model);
    ui->listView->setAlternatingRowColors(true);//设置交替颜色

    setFilter();//按语言过滤

    onlineMp3 = new OnlineMp3(musicForm, songDatabase, this); //音乐搜索引擎
    onlineMp3->setMusicList(ui->horizontalSlider_music, ui->label_time, ui->label_time_long);
    switchSong();//切换歌曲

    //音量设置
    voice = new Voice(this);
    voice->hide();
    connect(voice, &Voice::voiceValue, onlineMp3, [this](int value)
    {
        onlineMp3->setVolume(value);
    });
    connect(voice, &Voice::isMute, onlineMp3, [this](bool flag)
    {
        onlineMp3->setMuted(flag);
    });

    //系统托盘相关
    closeMes = new CloseMes(this);
    closeMes->hide();
    initTrayIcon();//初始化系统托盘
}

Wyy::~Wyy()
{
    delete pet;
    delete ui;
}

/*左下角模块*/
void Wyy::setMusicLeft()
{
    musicForm = new MusicForm();
    musicForm->setMusicName("把回忆拼好给你");
    musicForm->setMusicAuthor("王贰浪");
    musicForm->setMusicPicture(":/bottom/images/bottom/demo.jpg");
    ui->verticalLayout->addWidget(musicForm);
}

bool Wyy::eventFilter(QObject *watched, QEvent *event)
{
    //--------------------移动窗口-------------------//
    if (event->type() == QEvent::MouseButtonPress)
    {
        auto e = dynamic_cast<QMouseEvent *> (event);
        if (e) //转换成功
        {
            movePoint = e->globalPos() - pos();
            mousePress = true;
        }
    }
    else if (event->type() == QEvent::MouseMove)
    {
        auto e = dynamic_cast<QMouseEvent *>(event);
        if (mousePress)
            move(e->globalPos() - movePoint); //实现拖动
    }
    else if (event->type() == QEvent::MouseButtonRelease)
        mousePress = false;

    //----------------------当点击查询按钮----------------//
    //当点击查询按钮
    if (watched == ui->line_Search)
    {
        if (event->type() == QEvent::MouseButtonPress)
        {
            int x = ui->widgetSearch->x();
            int y = ui->widgetSearch->y();
            int hight = ui->widgetSearch->height();

            searchForm->setGeometry(x - 45, y + hight + 2, searchForm->width(), searchForm->height());
            searchForm->show();

            return true;
        }
        else if (event->type() == QEvent::KeyPress)
        {
            QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
            if (keyEvent->key() == Qt::Key_Enter//小键盘的Enter
                || keyEvent->key() == Qt::Key_Return)//大键盘(字母)的Enter
                on_pushButton_search_clicked();//搜索按钮对应的槽函数
        }
        else
            return false;

    }
    else if (watched == searchForm)
    {
        //当鼠标离开时候，界面隐藏
        if (event->type() == QEvent::Leave)
        {
            searchForm->hide();
            return true;
        }
        else
            return false;

    }

    return QObject::eventFilter(watched, event);//调用父类让他去处理其他事件
}

//最小化
void Wyy::on_pushButton_min_clicked()
{
    showMinimized(); //最小化
}
//放大
void Wyy::on_pushButton_max_clicked(bool checked)
{
    if (checked)
        showFullScreen();
    else
        showNormal();
}
//关闭
void Wyy::on_pushButton_close_clicked()
{
    closeMes->move(width() / 2 - closeMes->width() / 2, height() / 2 - closeMes->height());

    if (closeMes->exec() == QDialog::Accepted)
    {
        if (closeMes->getAns() == false)
        {
            hide();
            pet->hide();
        }
        else
        {
            pet->close();
            close();
        }
    }
}

//切换皮肤对应的槽
void Wyy::changeSkin(QString color)
{
    if ("Red" == color)
        ui->title_Widget->setStyleSheet(LoadFileQss::setStyle(":/qss/qss/skin/Red.css"));
    else if ("Blue" == color)
        ui->title_Widget->setStyleSheet(LoadFileQss::setStyle(":/qss/qss/skin/Blue.css"));
    else if ("Yellow" == color)
        ui->title_Widget->setStyleSheet(LoadFileQss::setStyle(":/qss/qss/skin/Yellow.css"));
    else if ("Pink" == color)
        ui->title_Widget->setStyleSheet(LoadFileQss::setStyle(":/qss/qss/skin/Pink.css"));
    else if ("Orange" == color)
        ui->title_Widget->setStyleSheet(LoadFileQss::setStyle(":/qss/qss/skin/Orange.css"));
    else if ("Green" == color)
        ui->title_Widget->setStyleSheet(LoadFileQss::setStyle(":/qss/qss/skin/Green.css"));
}
//切换皮肤界面
void Wyy::on_pushButton_cloth_clicked()
{
    QRect colorGeometry = changeColor->geometry();
    int colorX = ui->pushButton_cloth->x() - (colorGeometry.width() / 2);
    int colorY = ui->title_Widget->height();

    changeColor->setGeometry(colorX, colorY, colorGeometry.width(), colorGeometry.height());

    if (changeColor->isHidden())
    {
        changeColor->raise(); //提示显示层数
        changeColor->show();
    }
    else
        changeColor->hide();
}

//个人消息
void Wyy::on_pushButton_message_clicked()
{
    QRect messageGeometry = pMessageForm->geometry();
    int messageX = ui->message_Widget->x() - (messageGeometry.width() / 2);
    int messageY = ui->title_Widget->height();

    pMessageForm->setGeometry(messageX, messageY, messageGeometry.width(),
                              messageGeometry.height());

    if (pMessageForm->isHidden())
    {
        pMessageForm->raise(); //提示显示层数
        pMessageForm->show();
    }
    else
        pMessageForm->hide();
}

//显示个人中心
void Wyy::on_pushButton_name_clicked()
{
    QRect personGeometry = pMessageForm->geometry();
    int personX = ui->person_Widget->x() - (personGeometry.width() / 4);
    int personY = ui->title_Widget->height();

    p_person->setGeometry(personX + 20, personY, 290,
                          400);

    if (p_person->isHidden())
    {
        p_person->raise(); //提示显示层数
        p_person->show();
    }
    else
        p_person->hide();
}
//轮播图
void Wyy::setPictureWall()
{
    pictureWidget = new PictureWidget(this);
    ui->verLout_pic->addWidget(pictureWidget);
}
//画廊
void Wyy::setGallery()
{
    //初始话画廊
    gallerPrivate = new GalleryPrivate();

    QList<GalleryItem * > items;
    //添加8个数据
    GalleryItem *item = new GalleryItem();
    item->setPicture(":/Gallery/images/Gallery/1.jpg");
    item->setMessage("曲太硬 轻点踩油门");
    items.append(item);

    item = new GalleryItem();
    item->setPicture(":/Gallery/images/Gallery/2.jpg");
    item->setMessage("驾车听K-Pop|跟随K-Pop感受微风的轻抚");
    items.append(item);

    item = new GalleryItem();
    item->setPicture(":/Gallery/images/Gallery/3.jpg");
    item->setMessage("韩系表白墙|融化在耳畔的甜甜爱意");
    items.append(item);

    item = new GalleryItem();
    item->setPicture(":/Gallery/images/Gallery/4.jpg");
    item->setMessage("重返2009|苏州河等2009经典热曲等你来听");
    items.append(item);

    item = new GalleryItem();
    item->setPicture(":/Gallery/images/Gallery/5.jpg");
    item->setMessage("【压迫感】最初的恶魔猎人");
    items.append(item);

    item = new GalleryItem();
    item->setPicture(":/Gallery/images/Gallery/6.jpg");
    item->setMessage("从《爱的代价》开启宝藏音乐|时光雷达");
    items.append(item);

    item = new GalleryItem();
    item->setPicture(":/Gallery/images/Gallery/7.jpg");
    item->setMessage("驾车必备 请 勿 超 速!!!");
    items.append(item);

    item = new GalleryItem();
    item->setPicture(":/Gallery/images/Gallery/8.jpg");
    item->setMessage("《进化风暴|赛博朋克5D科幻极势感");
    items.append(item);

    //开始创建画廊
    gallerPrivate->setItemRes(item->width(), item->height(), items);
    ui->horizontalLayout_6->setContentsMargins(0, 0, 0, 0);
    ui->horizontalLayout_6->addWidget(gallerPrivate->getItemViewport());
}

/*专属定制中的按钮*/
void Wyy::on_btnPerL_clicked()
{
    int num = gallerPrivate->getPreviousItemIndex();
    gallerPrivate->showImage(num);
}

void Wyy::on_btnPerR_clicked()
{
    int num = gallerPrivate->getNextItemIndex();
    gallerPrivate->showImage(num);
}

void Wyy::setButtonMus()
{
    pButtonMus = new QButtonGroup(this);
    pButtonMus->addButton(ui->ptn0, 0);
    pButtonMus->addButton(ui->ptn1, 1);
    pButtonMus->addButton(ui->ptn2, 2);
    pButtonMus->addButton(ui->ptn3, 3);
    pButtonMus->addButton(ui->ptn4, 4);
    pButtonMus->addButton(ui->ptn5, 5);
    pButtonMus->addButton(ui->ptn6, 6);
    pButtonMus->addButton(ui->ptn7, 7);
    pButtonMus->addButton(ui->ptn8, 8);
    pButtonMus->addButton(ui->ptn9, 9);
    pButtonMus->setExclusive(true);
    pButtonMus->button(0)->setChecked(true);

    connect(pButtonMus, &QButtonGroup::idClicked,
            [ = ](int id)
    {
        setSourceMus(id);
    });
}

void Wyy::setSourceMus(int id)
{
    QString str = pButtonMus->button(id)->text();
    ui->ptnChange->setText(str + " >");
    switch (id)
    {
        //0 华语
        case 0:
        {
            QList<QString> arrHuayu;
            for (int i = 0; i < 5; i++)
                arrHuayu.append(QString::asprintf(":/pictureMusic/images/pictureMusic/huayu%d.png", i));
            QString style = "QWidget#picTitle{border-image:url(" + arrHuayu[0] + ");}";
            ui->picTitle->setStyleSheet(style);

            style = "QWidget#widMus1{border-image:url(" + arrHuayu[1] + ");}";
            ui->widMus1->setStyleSheet(style);

            style = "QWidget{border-image:url(" + arrHuayu[2] + ");}";
            ui->widMus2->setStyleSheet(style);

            style = "QWidget{border-image:url(" + arrHuayu[3] + ");}";
            ui->widMus3->setStyleSheet(style);

            style = "QWidget{border-image:url(" + arrHuayu[4] + ");}";
            ui->widMus4->setStyleSheet(style);
            break;
        }
        //流行
        case 1:
        {
            QList<QString> arrLiuxing;
            for (int i = 0; i < 5; i++)
                arrLiuxing.append(QString::asprintf(":/pictureMusic/images/pictureMusic/liuxing%d.png", i));
            QString style = "QWidget#picTitle{border-image:url(" + arrLiuxing[0] + ");}";
            ui->picTitle->setStyleSheet(style);
            style = "QWidget#widMus1{border-image:url(" + arrLiuxing[1] + ");}";
            ui->widMus1->setStyleSheet(style);
            style = "QWidget{border-image:url(" + arrLiuxing[2] + ");}";
            ui->widMus2->setStyleSheet(style);
            style = "QWidget{border-image:url(" + arrLiuxing[3] + ");}";
            ui->widMus3->setStyleSheet(style);
            style = "QWidget{border-image:url(" + arrLiuxing[4] + ");}";
            ui->widMus4->setStyleSheet(style);
            break;
        }
        //摇滚
        case 2:
        {
            QList<QString> arrYaogun;
            for (int i = 0; i < 5; i++)
                arrYaogun.append(QString::asprintf(":/pictureMusic/images/pictureMusic/yaogun%d.png", i));
            QString style = "QWidget#picTitle{border-image:url(" + arrYaogun[0] + ");}";
            ui->picTitle->setStyleSheet(style);
            style = "QWidget#widMus1{border-image:url(" + arrYaogun[1] + ");}";
            ui->widMus1->setStyleSheet(style);
            style = "QWidget{border-image:url(" + arrYaogun[2] + ");}";
            ui->widMus2->setStyleSheet(style);
            style = "QWidget{border-image:url(" + arrYaogun[3] + ");}";
            ui->widMus3->setStyleSheet(style);
            style = "QWidget{border-image:url(" + arrYaogun[4] + ");}";
            ui->widMus4->setStyleSheet(style);
            break;
        }
        //民谣
        case 3:
        {
            QList<QString> arrMinyao;
            for (int i = 0; i < 5; i++)
                arrMinyao.append(QString::asprintf(":/pictureMusic/images/pictureMusic/minyao%d.png", i));
            QString style = "QWidget#picTitle{border-image:url(" + arrMinyao[0] + ");}";
            ui->picTitle->setStyleSheet(style);
            style = "QWidget#widMus1{border-image:url(" + arrMinyao[1] + ");}";
            ui->widMus1->setStyleSheet(style);
            style = "QWidget{border-image:url(" + arrMinyao[2] + ");}";
            ui->widMus2->setStyleSheet(style);
            style = "QWidget{border-image:url(" + arrMinyao[3] + ");}";
            ui->widMus3->setStyleSheet(style);
            style = "QWidget{border-image:url(" + arrMinyao[4] + ");}";
            ui->widMus4->setStyleSheet(style);
            break;
        }
        //电子
        case 4:
        {
            QList<QString> arrDianzi;
            for (int i = 0; i < 5; i++)
                arrDianzi.append(QString::asprintf(":/pictureMusic/images/pictureMusic/dianzi%d.png", i));
            QString style = "QWidget#picTitle{border-image:url(" + arrDianzi[0] + ");}";
            ui->picTitle->setStyleSheet(style);
            style = "QWidget#widMus1{border-image:url(" + arrDianzi[1] + ");}";
            ui->widMus1->setStyleSheet(style);
            style = "QWidget{border-image:url(" + arrDianzi[2] + ");}";
            ui->widMus2->setStyleSheet(style);
            style = "QWidget{border-image:url(" + arrDianzi[3] + ");}";
            ui->widMus3->setStyleSheet(style);
            style = "QWidget{border-image:url(" + arrDianzi[4] + ");}";
            ui->widMus4->setStyleSheet(style);
            break;
        }
        //另类
        case 5:
        {
            QList<QString> arrLinglei;
            for (int i = 0; i < 5; i++)
                arrLinglei.append(QString::asprintf(":/pictureMusic/images/pictureMusic/linglei%d.png", i));
            QString style = "QWidget#picTitle{border-image:url(" + arrLinglei[0] + ");}";
            ui->picTitle->setStyleSheet(style);
            style = "QWidget#widMus1{border-image:url(" + arrLinglei[1] + ");}";
            ui->widMus1->setStyleSheet(style);
            style = "QWidget{border-image:url(" + arrLinglei[2] + ");}";
            ui->widMus2->setStyleSheet(style);
            style = "QWidget{border-image:url(" + arrLinglei[3] + ");}";
            ui->widMus3->setStyleSheet(style);
            style = "QWidget{border-image:url(" + arrLinglei[4] + ");}";
            ui->widMus4->setStyleSheet(style);
            break;
        }
        //轻音乐
        case 6:
        {
            QList<QString> arrQingyinyue;
            for (int i = 0; i < 5; i++)
                arrQingyinyue.append(QString::asprintf(":/pictureMusic/images/pictureMusic/qingyinyue%d.png", i));
            QString style = "QWidget#picTitle{border-image:url(" + arrQingyinyue[0] + ");}";
            ui->picTitle->setStyleSheet(style);
            style = "QWidget#widMus1{border-image:url(" + arrQingyinyue[1] + ");}";
            ui->widMus1->setStyleSheet(style);
            style = "QWidget{border-image:url(" + arrQingyinyue[2] + ");}";
            ui->widMus2->setStyleSheet(style);
            style = "QWidget{border-image:url(" + arrQingyinyue[3] + ");}";
            ui->widMus3->setStyleSheet(style);
            style = "QWidget{border-image:url(" + arrQingyinyue[4] + ");}";
            ui->widMus4->setStyleSheet(style);
            break;
        }
        //综艺
        case 7:
        {
            QList<QString> arrZongyi;
            for (int i = 0; i < 5; i++)
                arrZongyi.append(QString::asprintf(":/pictureMusic/images/pictureMusic/zongyi%d.png", i));
            QString style = "QWidget#picTitle{border-image:url(" + arrZongyi[0] + ");}";
            ui->picTitle->setStyleSheet(style);
            style = "QWidget#widMus1{border-image:url(" + arrZongyi[1] + ");}";
            ui->widMus1->setStyleSheet(style);
            style = "QWidget{border-image:url(" + arrZongyi[2] + ");}";
            ui->widMus2->setStyleSheet(style);
            style = "QWidget{border-image:url(" + arrZongyi[3] + ");}";
            ui->widMus3->setStyleSheet(style);
            style = "QWidget{border-image:url(" + arrZongyi[4] + ");}";
            ui->widMus4->setStyleSheet(style);
            break;
        }
        //原声
        case 8:
        {
            QList<QString> arrYuansheng;
            for (int i = 0; i < 5; i++)
                arrYuansheng.append(QString::asprintf(":/pictureMusic/images/pictureMusic/yuansheng%d.png", i));
            QString style = "QWidget#picTitle{border-image:url(" + arrYuansheng[0] + ");}";
            ui->picTitle->setStyleSheet(style);
            style = "QWidget#widMus1{border-image:url(" + arrYuansheng[1] + ");}";
            ui->widMus1->setStyleSheet(style);
            style = "QWidget{border-image:url(" + arrYuansheng[2] + ");}";
            ui->widMus2->setStyleSheet(style);
            style = "QWidget{border-image:url(" + arrYuansheng[3] + ");}";
            ui->widMus3->setStyleSheet(style);
            style = "QWidget{border-image:url(" + arrYuansheng[4] + ");}";
            ui->widMus4->setStyleSheet(style);
            break;
        }
        //ACG
        case 9:
        {
            QList<QString> arrAcg;
            for (int i = 0; i < 5; i++)
                arrAcg.append(QString::asprintf(":/pictureMusic/images/pictureMusic/Acg%d.png", i));
            QString style = "QWidget#picTitle{border-image:url(" + arrAcg[0] + ");}";
            ui->picTitle->setStyleSheet(style);
            style = "QWidget#widMus1{border-image:url(" + arrAcg[1] + ");}";
            ui->widMus1->setStyleSheet(style);
            style = "QWidget{border-image:url(" + arrAcg[2] + ");}";
            ui->widMus2->setStyleSheet(style);
            style = "QWidget{border-image:url(" + arrAcg[3] + ");}";
            ui->widMus3->setStyleSheet(style);
            style = "QWidget{border-image:url(" + arrAcg[4] + ");}";
            ui->widMus4->setStyleSheet(style);
            break;
        }
        default:
            break;
    }
}
//设置RankingItem
void Wyy::setItem(const QPair<int, bool> &pair, const QString &pic, const int &picc,
                  const QString &name, const QString &autor,
                  const QColor &color, QListWidget *&listWidget)
{
    //RankingIte属性设置
    RankingItem *item = new RankingItem(this);
    item->setNum(pair.first, pair.second);
    //通过picc判断调用哪个重载函数
    if (picc == -1)
        item->setStringOrPic(pic);
    else
        item->setStringOrPic(picc);

    item->setMusicName(name);
    item->setMusicAutor(autor);

    //ListWidgetItem相关属性设置
    QListWidgetItem *newWidget = nullptr;
    newWidget = new QListWidgetItem(listWidget);
    newWidget->setBackground(color);
    newWidget->setSizeHint(QSize(item->width(), item->height()));//条目显示建议尺寸
    listWidget->setItemWidget(newWidget, item);
}
//飙升榜单
void Wyy::setRankData()
{
    setItem({1, true}, "792%", -1, "删了吧", "烟(许佳豪)", QColor(249, 249, 249), ui->listCont);
    setItem({2, true}, "127%", -1, "时间轴", "余佳运", QColor(255, 255, 255), ui->listCont);
    setItem({3, true}, "113%", -1, "没你也能活下去", "Jony J", QColor(249, 249, 249), ui->listCont);
    setItem({4, false}, "111%", -1, "都不懂", "吉法师", QColor(255, 255, 255), ui->listCont);
    setItem({5, false}, "88%", -1, "透明轨迹", "ICY", QColor(249, 249, 249), ui->listCont);

    //设置第一个选中
    ui->listCont->setCurrentRow(0);
}
//新歌榜
void Wyy::setRankNew()
{
    setItem({1, true}, "", 0, "不需要挽留", "move on", QColor(249, 249, 249), ui->listCont2);
    setItem({2, true}, "", 1, "潮汐", "司南", QColor(255, 255, 255), ui->listCont2);
    setItem({3, true}, "", 2, "执子之手", "宝石", QColor(249, 249, 249), ui->listCont2);
    setItem({4, false}, "", 2, "一格格", "卫兰", QColor(255, 255, 255), ui->listCont2);
    setItem({5, false}, "", 2, "把回忆拼好给你", "ICY", QColor(249, 249, 249), ui->listCont2);

    //设置第一个选中
    ui->listCont2->setCurrentRow(0);
}
//设置语言,分类，筛选的按钮组
void Wyy::setButtonGroup()
{
    //1. 语言
    pButtonLaug = new QButtonGroup(this);
    pButtonLaug->addButton(ui->ptnAll1, 0);
    pButtonLaug->addButton(ui->ptnChinese, 1);
    pButtonLaug->addButton(ui->ptnEurope, 2);
    pButtonLaug->addButton(ui->ptnJapan, 3);
    pButtonLaug->addButton(ui->ptnKorea, 4);
    pButtonLaug->addButton(ui->ptnOther1, 5);
    pButtonLaug->setExclusive(true);
    pButtonLaug->button(0)->setChecked(true);

    connect(pButtonLaug, &QButtonGroup::idClicked,
            [ = ](int id)
    {
        qDebug() << "设置的语言ID: " << id;
    });

    //2. 歌手分类
    pButtonClass = new QButtonGroup(this);
    pButtonClass->addButton(ui->ptnAll2, 0);
    pButtonClass->addButton(ui->ptnMan, 1);
    pButtonClass->addButton(ui->ptnWomen, 2);
    pButtonClass->addButton(ui->ptnBank, 3);
    pButtonClass->setExclusive(true);
    pButtonClass->button(0)->setChecked(true);

    connect(pButtonClass, &QButtonGroup::idClicked,
            [ = ](int id)
    {
        qDebug() << "歌手分类ID: " << id;
    });

    //2. 歌手筛选
    pButtonScreen = new QButtonGroup(this);
    pButtonScreen->addButton(ui->ptnhot, 0);
    pButtonScreen->addButton(ui->ptnA, 1);
    pButtonScreen->addButton(ui->ptnB, 2);
    pButtonScreen->addButton(ui->ptnC, 3);
    pButtonScreen->addButton(ui->ptnD, 4);
    pButtonScreen->addButton(ui->ptnE, 5);
    pButtonScreen->addButton(ui->ptnF, 6);
    pButtonScreen->addButton(ui->ptnG, 7);
    pButtonScreen->addButton(ui->ptnH, 8);
    pButtonScreen->addButton(ui->ptnI, 9);
    pButtonScreen->addButton(ui->ptnJ, 10);
    pButtonScreen->addButton(ui->ptnK, 11);
    pButtonScreen->addButton(ui->ptnL, 12);
    pButtonScreen->addButton(ui->ptnM, 13);
    pButtonScreen->addButton(ui->ptnN, 14);
    pButtonScreen->addButton(ui->ptnO, 15);
    pButtonScreen->addButton(ui->ptnP, 16);
    pButtonScreen->addButton(ui->ptnQ, 17);
    pButtonScreen->addButton(ui->ptnR, 18);
    pButtonScreen->addButton(ui->ptnS, 19);
    pButtonScreen->addButton(ui->ptnT, 20);
    pButtonScreen->addButton(ui->ptnU, 21);
    pButtonScreen->addButton(ui->ptnV, 22);
    pButtonScreen->addButton(ui->ptnW, 23);
    pButtonScreen->addButton(ui->ptnX, 24);
    pButtonScreen->addButton(ui->ptnY, 25);
    pButtonScreen->addButton(ui->ptnZ, 26);
    pButtonScreen->addButton(ui->ptnOH, 27);
    pButtonScreen->setExclusive(true);
    pButtonScreen->button(0)->setChecked(true);

    connect(pButtonScreen, &QButtonGroup::idClicked,
            [ = ](int id)
    {
        qDebug() << "歌手筛选ID: " << id;
    });
}
//加载到堆栈布局里
void Wyy::setAddWidget()
{
    pCourier = new NewCourier(this);//新歌列表
    pDisMusic = new NewDisc(this);//新歌碟片

    stackedLayout = new QStackedLayout;
    stackedLayout->addWidget(pCourier);
    stackedLayout->addWidget(pDisMusic);
    stackedLayout->setCurrentIndex(0);

    ui->verAddWidget->addLayout(stackedLayout);
}
//设置新歌速递与新蝶上架模块
void Wyy::setNewMusicButton()
{
    QButtonGroup *btnGroup = new QButtonGroup(this);
    btnGroup->addButton(ui->ptnNew, 0);
    btnGroup->addButton(ui->ptnNew2, 1);
    btnGroup->setExclusive(true);//设置互斥
    btnGroup->button(0)->setChecked(true);

    //连接槽
    connect(btnGroup, &QButtonGroup::idClicked,
            [ = ](int id)
    {
        currentFlag = id;//0为新歌速递 1 新蝶上架
        changeChose();//变化对应界面
    });
}
//全部 华语 欧美 韩国 日本
void Wyy::setNewMusicChooseButton()
{
    QButtonGroup *btnGroup = new QButtonGroup(this);
    btnGroup->addButton(ui->ptnMusAll, 0);
    btnGroup->addButton(ui->ptnMusChinese, 1);
    btnGroup->addButton(ui->ptnMusEurope, 2);
    btnGroup->addButton(ui->ptnMusKorea, 3);
    btnGroup->addButton(ui->ptnMusJapan, 4);
    btnGroup->setExclusive(true);
    btnGroup->button(0)->setChecked(true);

    //连接槽
    connect(btnGroup, &QButtonGroup::idClicked,
            [ = ](int id)
    {
        currentClass = id;//0为 全部 1为中文 2为欧美 3为韩国 4为日本
        changeChose();//变化对应界面
    });
}
//变化对应界面
void Wyy::changeChose()
{
    if (currentFlag == 0)//新歌速递
    {
        stackedLayout->setCurrentIndex(0);
        pCourier->setChose(currentClass);
    }
    else if (currentFlag == 1)//新蝶上架
    {
        stackedLayout->setCurrentIndex(1);
        pDisMusic->setChose(currentClass);
    }
}

void Wyy::on_horizontalSlider_music_valueChanged(int value)
{
    QTime time(0, value / 60000, qRound((value % 60000) / 1000.0));
    ui->label_time->setText(time.toString("mm:ss"));
    onlineMp3->setPosition(value);
}
//播放暂停
void Wyy::on_pushButton_stop_clicked(bool checked)
{
    if (checked)
    {
        ui->pushButton_stop->setChecked(true);
        onlineMp3->setPlayerState(0);//play
        ui->pushButton_stop->setStyleSheet("border-image:url(:/bottom/images/bottom/pause.png)");
    }
    else
    {
        onlineMp3->setPlayerState(1);//pause
        ui->pushButton_stop->setStyleSheet("border-image:url(:/bottom/images/bottom/star.png)");
    }
}
//音量
void Wyy::on_pushButton_voice2_clicked()
{
    QRect voiceGeometry = voice->geometry();
    int voiceX = ui->bottomRight->x() + ui->pushButton_voice2->x() - (voiceGeometry.width() / 2);
    int voiceY = ui->tail_widget->y() - voice->height();

    voice->setGeometry(voiceX, voiceY, voiceGeometry.width(),
                       voiceGeometry.height());
    if (voice->isHidden())
    {
        voice->raise(); //提升显示层数
        voice->show();
    }
    else
        voice->hide();
}

//秒转换成MM:SS 的时间格式
QString Wyy::convertSecondsToTime(int seconds)
{
    int minutes = seconds / 60; // 计算分钟数
    int remainingSeconds = seconds % 60; // 计算剩余的秒数
    return QString("%1:%2")
           .arg(minutes, 2, 10, QChar('0')) // 保证分钟数是两位数
           .arg(remainingSeconds, 2, 10, QChar('0')); // 保证秒数是两位数
}

//更新界面
void Wyy::updateUi(SongInfo &songInfo)
{
    QStandardItem *item = new QStandardItem;
    model->appendRow(item);

    MusicItem *musicItem = new MusicItem(ui->listView);
    musicItem->setNum(songInfo.num);
    musicItem->setMusicName(songInfo.MusicName);
    musicItem->setMusicAuthor(songInfo.MusicAuthor);
    if (flag)
        songInfo.mTime = convertSecondsToTime(songInfo.mTime.toInt());
    else
        songInfo.mTime = songInfo.mTime;
    musicItem->setMusicTimer(songInfo.mTime);
    musicItem->setLanguage(songInfo.language);

    // 将 MusicItem 作为 QListView 中某项的 widget
    QModelIndex index = model->index(model->rowCount() - 1, 0);
    ui->listView->setIndexWidget(index, musicItem);// 为每个模型项设置自定义的 QWidget (MusicItem)
}

//按语言过滤
void Wyy::setFilter()
{
    QButtonGroup *btnGroup = new QButtonGroup(this);
    btnGroup->addButton(ui->btnL1, 0);
    btnGroup->addButton(ui->btnL2, 1);
    btnGroup->addButton(ui->btnL3, 2);
    btnGroup->addButton(ui->btnL4, 3);
    btnGroup->addButton(ui->btnL5, 4);
    btnGroup->addButton(ui->btnL6, 5);
    btnGroup->setExclusive(true);//设置互斥

    btnGroup->button(0)->setChecked(true);
    //连接槽
    connect(btnGroup, &QButtonGroup::idClicked,
            [ = ](int id)
    {
        setChose(id);
    });
}

//国语，粤语，日语，英语，其他
void Wyy::setChose(int id)
{
    model->removeRows(0, model->rowCount());
    QString database, sql;
    if (flag == false) database = "songhistory";
    else database = "songlist";

    if (id == 0)
    {
        QList<SongInfo> songInfoList = songDatabase->queryInfo(database);
        if (!songDatabase->getIsAccess())
        {
            // 查询失败，显示错误信息
            QMessageBox::critical(nullptr, "筛选歌曲这里查询错误", songDatabase->getErrMsg());
            return;
        }
        for (int i = 0; i < songInfoList.size(); ++i)
            updateUi(songInfoList[i]);
    }
    else
    {
        QList<SongInfo> songInfoList = songDatabase->queryInfo(database, QString("language = %2").arg(id));
        if (!songDatabase->getIsAccess())
        {
            // 查询失败，显示错误信息
            QMessageBox::critical(nullptr, "筛选歌曲这里查询错误", songDatabase->getErrMsg());
            return;
        }

        for (int i = 0; i < songInfoList.size(); ++i)
            updateUi(songInfoList[i]);
    }
}
//我的音乐的列表
void Wyy::on_listT2_itemClicked(QListWidgetItem *item)
{
    if (item->text() == "最近播放")
        recentPlay();
}
//最近播放
void Wyy::recentPlay()
{
    flag = false;
    model->removeRows(0, model->rowCount());

    ui->widget_Lang->hide();
    ui->btnClear->show();
    ui->lbTitle->setText("最近播放");
    ui->lbTime->setText("播放时间");

    // 查询歌曲历史记录表中的数据并显示
    QList<SongInfo> songInfoList = songDatabase->queryInfo("songhistory");
    if (!songDatabase->getIsAccess())
    {
        // 查询失败，显示错误信息
        QMessageBox::critical(nullptr, "最近播放这里查询错误", songDatabase->getErrMsg());
        return;
    }
    for (auto songInfo : songInfoList)
        updateUi(songInfo);

    ui->lbTotal->setText(QString("共%1首").arg(songDatabase->getCnt()));
    ui->stackedWidget->setCurrentIndex(1);
}
//清空最近播放的历史记录
void Wyy::on_btnClear_clicked()
{
    model->removeRows(0, model->rowCount());
    ui->lbTotal->setText("共0首");

    songDatabase->clearTable("songhistory");
}
//搜索按钮对应的槽函数
void Wyy::on_pushButton_search_clicked()
{
    //切换到搜索界面初始化操作
    QString musicName = ui->line_Search->text().trimmed();
    flag = true;
    ui->widget_Lang->show();
    ui->btnClear->hide();
    ui->lbTime->setText("时长");
    ui->line_Search->clear();
    ui->lbTitle->setText("搜索 " + musicName);
    model->removeRows(0, model->rowCount());//清空列表
    ui->stackedWidget->setCurrentIndex(1);
    searchForm->close();
    // 清理数据表中已经存储的 hash 等数据
    songDatabase->clearTable("songlist");

    QList<SongInfo *> songInfoList = onlineMp3->searchPlayer(musicName);
    for (auto songInfo : songInfoList)
        updateUi(*songInfo);

    ui->lbTotal->setText("共" + QString::number(songInfoList.size()) + "首");
    // 使用qDeleteAll来删除所有Widget对象
    qDeleteAll(songInfoList);
    songInfoList.clear(); // 清空列表，确保所有指针都被移除
}
//双击播放
void Wyy::on_listView_doubleClicked(const QModelIndex &index)
{
    // 获取双击的歌曲索引，即数据表的 ID 号
    curRow = index.row();
    QString tableName;
    if (flag == false) tableName = "songhistory";
    else tableName = "songlist";

    QList<SongInfo> songInfoList = songDatabase->queryInfo(tableName,
                                   QString("id = %1")
                                   .arg(curRow + 1));
    if (!songDatabase->getIsAccess())
    {
        QMessageBox::critical(nullptr, "双击播放这里查询指定id歌曲失败", songDatabase->getErrMsg());
        return;
    }

    QString EMixSongID;
    if (!songInfoList.isEmpty())
    {
        SongInfo songInfo = songInfoList[0];
        musicForm->setMusicName(songInfo.MusicName);
        musicForm->setMusicAuthor(songInfo.MusicAuthor);
        EMixSongID = songInfo.EMixSongID;

        // 查询历史数据表中是否已经存在该歌曲的记录
        QList<SongInfo> historyList = songDatabase->queryInfo("songhistory", QString("EMixSongID = '%1'").arg(EMixSongID));
        if (!songDatabase->getIsAccess())
        {
            QMessageBox::critical(nullptr, "双击播放这里查询指定id歌曲的哈希失败", songDatabase->getErrMsg());
            return;
        }
        if (historyList.isEmpty())// 如果不存在该记录，则将其存入历史数据表
        {
            QDateTime dataTime = QDateTime::currentDateTime();
            QString historyTime = dataTime.toString("yyyy-MM-dd");
            songDatabase->insertTable("songhistory",
            {0, songInfo.MusicName, songInfo.MusicAuthor, historyTime, songInfo.language, EMixSongID}
            , true);
            if (!songDatabase->getIsAccess())
            {
                QMessageBox::critical(nullptr, "双击播放这里插入失败", songDatabase->getErrMsg());
                return;
            }
        }
    }

    onlineMp3->downloadPlayer(EMixSongID);
    on_pushButton_stop_clicked(true);
}

//切换歌曲
void Wyy::switchSong()
{
    QButtonGroup *btnGroup = new QButtonGroup(this);
    btnGroup->addButton(ui->pushButton_life_music, 0);
    btnGroup->addButton(ui->pushButton_right_music, 1);
    connect(btnGroup, &QButtonGroup::idClicked,
            [ = ](int id)
    {
        if (id == 0)
        {
            // 当前行数减一
            curRow--;
            // 如果行数为负数，则将其设置为列表的最后一行
            if (curRow < 0)
                curRow = model->rowCount() - 1;
        }
        else
        {
            // 当前行数减一
            curRow++;

            // 如果行数为边界下面，则将其设置为列表的第一行
            if (curRow == model->rowCount())
                curRow = 0;
        }
        ui->listView->setCurrentIndex(model->index(curRow, 0));//选中当要播放的行
        on_listView_doubleClicked(model->index(curRow, 0));
    });
}

//初始化系统托盘
void Wyy::initTrayIcon()
{
    trayIcon = new QSystemTrayIcon(QIcon(":/ohter/images/ohter/logoRed.ico"), this);
    trayIcon->setToolTip("网易云音乐");

    showMessageAction = new QAction(QIcon(":/ohter/images/ohter/eye.png"), "显示网易云", trayIcon);
    exitAction = new QAction(QIcon(":/ohter/images/ohter/exit.png"), "退出网易云", trayIcon);

    // 设置托盘图标上下文菜单
    trayMenu = new QMenu(this);
    trayMenu->addAction(showMessageAction);
    trayMenu->addAction(exitAction);
    trayIcon->setContextMenu(trayMenu);//设置上下文菜单
    // 显示系统托盘图标
    trayIcon->show();

    connect(trayIcon, &QSystemTrayIcon::activated,
            this, [&](QSystemTrayIcon::ActivationReason reason)
    {
        switch (reason)
        {
            // 单击系统托盘图标触发的操作
            case QSystemTrayIcon::Trigger:
                // 如果窗口当前为隐藏状态，则显示窗口
                if (isHidden())
                {
                    show();
                    pet->show();
                }
                // 否则隐藏窗口
                else
                {
                    hide();
                    pet->hide();
                }
                break;
            default:
                break;
        }
    });

    connect(showMessageAction, &QAction::triggered, [&]()
    {
        setVisible(true);
        pet->setVisible(true);
    });
    connect(exitAction, &QAction::triggered, [&]()
    {
        pet->close();
        qApp->quit();
    });
}
