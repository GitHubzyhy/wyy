#include "picturewidget.h"
#include "ui_picturewidget.h"
#include <QButtonGroup>
#include <QGraphicsItemAnimation>
#include <QTimeLine>
#include <QTimer>
#include "pictureitem.h"
#include <qDebug>

QVector<QGraphicsItemAnimation *> item(10);//用于为每个图片元素设置动画效果

PictureWidget::PictureWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PictureWidget)
{
    ui->setupUi(this);

    setButtonGroup(); //设置按钮组
    setInitList();// 初始化图片的位置信息和缩放因子

    m_scene = new QGraphicsScene(this);
    setPictureScreen();//图形项初始化操作
    setTimerAndConnect();// 切换图形项信号槽连接

    lastType = -1;

    timeline = new QTimeLine(200);// 创建一个 QTimeLine 对象，表示动画的时间轴，持续时间为 200 毫秒
    timeline->setLoopCount(1); // 设置动画循环次数为1，即只执行一次动画
    for (int i = 0; i < 10; i++)
        item[i] = new QGraphicsItemAnimation();// 为每个图片元素创建新的 QGraphicsItemAnimation 对象

    m_newT = new QTimer(this);
    m_newT->setInterval(3000);
    connect(m_newT, &QTimer::timeout, this, [this]()
    {
        on_btnR_clicked();
    });
    m_newT->start();
}

PictureWidget::~PictureWidget()
{
    m_newT->stop();
    delete ui;
}
//设置按钮组
void PictureWidget::setButtonGroup()
{
    m_BtnGroup = new QButtonGroup(this);
    m_BtnGroup->addButton(ui->btnPic0, 0);
    m_BtnGroup->addButton(ui->btnPic1, 1);
    m_BtnGroup->addButton(ui->btnPic2, 2);
    m_BtnGroup->addButton(ui->btnPic3, 3);
    m_BtnGroup->addButton(ui->btnPic4, 4);
    m_BtnGroup->addButton(ui->btnPic5, 5);
    m_BtnGroup->addButton(ui->btnPic6, 6);
    m_BtnGroup->addButton(ui->btnPic7, 7);
    m_BtnGroup->addButton(ui->btnPic8, 8);
    m_BtnGroup->addButton(ui->btnPic9, 9);
    m_BtnGroup->setExclusive(true);//互斥
    m_BtnGroup->button(1)->setChecked(true);//让第二个按钮选中
}
//初始化一些与图片显示相关的列表，包含位置、Z值（层级）和缩放比例。
void PictureWidget::setInitList()
{
    //保存图片在视图中的位置比例
    m_PointList << P1 << P2 << P3 << otherP2 << otherP2 << otherP2 << otherP2 << otherP2 << otherP2 << otherP2;
    //保存图片的Z值（优先级），控制显示层次
    m_ZValueList << 1 << 2 << 1 << 0 << 0 << 0 << 0 << 0 << 0 << 0;
    //中心图片的缩放比例为 1，其他图片为 0.8。
    m_PixmapScaleList << 0.8 << 1 << 0.8 << 0.8 << 0.8 << 0.8 << 0.8 << 0.8 << 0.8 << 0.8;//保存图片缩放比例
}
//图形项初始化操作
void PictureWidget::setPictureScreen()
{
    //1. 设置样式 透明
    ui->graphicsView->setStyleSheet("background: transparent; padding: 0px; border: 0px;");
    ui->graphicsView->setScene(m_scene);
    m_scene->setSceneRect(0, 0, RAW_VIEW_SIZE.width(), RAW_VIEW_SIZE.height());

    //2. 创建图形项并将其放入到场景中
    // 2.1 图形项图片
    for (int i = 1; i <= 10; ++i)
        m_PixmapList.append(QPixmap(QString(":/picturewall/images/picturewall/%1.jpg").arg(i)));
    // 2.2 图形项位置
    QLineF m_MidLine; //线条的作用是为了后面设置图形项位置做准备
    m_MidLine.setPoints(QPointF(0, 0),//设置线条的起始点和结束点
                        QPointF(RAW_VIEW_SIZE.width(), 0));
    for (int i = 0; i < 10; i++)
    {
        itemList.append(new pictureItem(m_PixmapList[i].scaled(RAW_VIEW_SIZE,//用的是加载像素图的构造函数
                                        Qt::KeepAspectRatio,//在保持长宽比的前提下，将尺寸缩放为给定矩形内尽可能大的矩形。
                                        Qt::SmoothTransformation) ));//平滑缩放

        //图形项的位置，显示优先级，缩放
        itemList[i]->setPos(m_MidLine.pointAt(m_PointList[i]));//多余的图形项都藏在中间
        itemList[i]->setZValue(m_ZValueList[i]);//设置显示优先级
        itemList[i]->setScale(m_PixmapScaleList[i]);

        itemList[i]->setType(i);//区分不同的图形项
        itemList[i]->setTransformationMode(Qt::SmoothTransformation);//设置缩放模式

        //除了索引为1的图片，其他图片会额外向下偏移，使它们的 Y 坐标是
        if (i != 1)
            itemList[i]->setPos(itemList[i]->x(), RAW_VIEW_SIZE.height() / 10); //即距离顶部有一定的距离
        pointA.append(itemList[i]->pos());//记录下图形项坐标

        m_scene->addItem(itemList[i]);
    }

    //4. 放入映射map中
    for (int id = 0; id < 10; id++)//以后可以通过按钮的ID来快速找到对应图形项。
        mapLink.insert(id, itemList[id]);
}
//图形项信号槽连接
void PictureWidget::setTimerAndConnect()
{
    for (int i = 0; i < 10; i++)
    {
        //1. 点击图形项
        connect(itemList[i], &pictureItem::clickedId,
                this, &PictureWidget::clickedItemRoll);

        //2. 鼠标进入按钮中
        void (pictureButton:: *funcPtr)() = &pictureButton::entered;
        connect(static_cast<pictureButton *>( m_BtnGroup->button(i) ), funcPtr, this, [this, i]
        {
            pictureItem *p = mapLink.value(i); //获取id 对应的图形项
            clickedItemRoll(p->type());//图形项点击对应的槽
            qDebug() << "图形项的类型ID：" << p->type();
        });
    }
}
//图形项点击对应的槽
void PictureWidget::clickedItemRoll(int type)
{
    if (lastType == type) return; //重复选中当前图形项/按钮

    lastType = type;
    m_BtnGroup->button(type)->setChecked(true);
    timerOutFunc(type);//图片滚动
}
//更新图片滚动的状态
void PictureWidget::timerOutFunc(int startNum)//参数是新的起始位置
{
    //防止越界
    if (startNum - 1 == -1)
        startNum = 10;
    startNum = startNum - 1; // 根据当前起始图片编号，获取第一个应该滚动显示的图片索引

    //重新设置图形项的相关属性
    for (int i = 0; i < 10; i++)
    {
        itemList[i] = mapLink.value(startNum % 10); // 从 mapLink 映射中获取图形项，保证索引在 0 到 9 的范围内
        startNum++;// 更新图片索引，指向下一张图片

        //重新更新某个图形项的缩放，优先级，缩放模式
        itemList[i]->setScale(m_PixmapScaleList[i]);
        itemList[i]->setZValue(m_ZValueList[i]);

        //动画相关设置

        item[i]->setItem(itemList[i]);// 将图片元素与动画对象绑定
        item[i]->setTimeLine(timeline);// 绑定时间轴，确保动画执行时按照指定的时间轴进行
        item[i]->setPosAt(1, pointA[i]);// 为每个图片元素在时间轴上的最后时刻 (1.0) 设定它应该移动到的位置 pointA[i]
    }
    timeline->start(); // 启动时间轴，开始执行动画
    // m_scene->invalidate(); // 通知场景重新绘制，确保动画执行时视觉上得到更新
}

//左边运动
void PictureWidget::on_btnL_clicked()
{
    int id = m_BtnGroup->checkedId();
    if (id - 1 < 0)
        id = 9;
    else
        id = id - 1;

    lastType = id;
    m_BtnGroup->button(id)->setChecked(true);
    timerOutFunc(id);//图片滚动
}
//右边运动
void PictureWidget::on_btnR_clicked()
{
    int id = m_BtnGroup->checkedId();
    id = (id + 1) % 10;

    lastType = id;
    m_BtnGroup->button(id)->setChecked(true);
    timerOutFunc(id);//图片滚动
}
