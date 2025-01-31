#ifndef PICTUREWIDGET_H
#define PICTUREWIDGET_H

#include <QWidget>
#include <QMap>

namespace Ui
{
    class PictureWidget;
}

class pictureItem;
class QTimeLine;
class QGraphicsScene;
class QButtonGroup;

#define RAW_VIEW_SIZE QSize(750,198)

//图形项的位于直线的位置比例
#define P1 (qreal)0.00
#define P2 (qreal)0.15
#define P3 (qreal)0.44
#define otherP2 P2

static QList<pictureItem *> itemList;//存储图形项
static QMap<int, pictureItem *> mapLink; //按钮id 与 图片资源的映射
static QList<QPointF> pointA;//记录图形项摆放坐标

class PictureWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PictureWidget(QWidget *parent = nullptr);
    ~PictureWidget();

    void setButtonGroup(); //设置button组，用于控制图片切换
    void setInitList(); //初始化图片位置、优先级和缩放因子列表
    void setPictureScreen(); //图形项初始化操作
    void setTimerAndConnect(); //图形项信号槽连接

public slots:
    void timerOutFunc(int startNum);//更新图片滚动的状态
    void clickedItemRoll(int type);//图形项点击对应的槽

    void on_btnL_clicked();//向左滚动
    void on_btnR_clicked();//向右滚动

private:
    Ui::PictureWidget *ui;

    QButtonGroup *m_BtnGroup; //按钮盒子

    QList<qreal> m_PointList; //各个图形项的位置比例
    QList<qreal> m_ZValueList; //图形项显示优先级列表
    QList<qreal> m_PixmapScaleList; //图形项位置信息伸缩因子列表

    QGraphicsScene *m_scene;
    QList<QPixmap> m_PixmapList; //图片列表

    int lastType;//防止重复滚动相同结果

    QTimeLine *timeline;//动画相关的时间线
    QTimer *m_newT; //定时器
};

#endif // PICTUREWIDGET_H
