#include "pictureitem.h"
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QPainterPath>

pictureItem::pictureItem(const QPixmap &pixmap, QGraphicsItem *parent)
    : QGraphicsObject(parent)
{
    m_pixmap = pixmap;
    m_type = 0;
    mode = Qt::SmoothTransformation; //图片无锯齿
    isPressed = false;
}

pictureItem::~pictureItem() {}

int pictureItem::type() const
{
    return m_type;
}
//方便管理和区分不同的图形项
void pictureItem::setType(int type)
{
    m_type = type;
}
//设置模式
void pictureItem::setTransformationMode(Qt::TransformationMode mode)
{
    if (mode != this->mode)
    {
        this->mode = mode;
        update();
    }
}

QRectF pictureItem::boundingRect() const
{
    if (m_pixmap.isNull())
        return QRectF();
    else
        //确定了矩形的宽度和高度，确保图像在适当的尺寸下显示。
        return QRectF(QPointF(0, 0), m_pixmap.size() / m_pixmap.devicePixelRatio());
}

void pictureItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);
    Q_UNUSED(option);

    //1. 画笔
    QPen pen;
    pen.setColor(QColor(0, 0, 0, 0));
    painter->setPen(pen);
    //2. 画刷
    QBrush brush;
    brush.setTexture(m_pixmap);
    painter->setBrush(brush);
    //3. 画圆角框框
    QPainterPath path;
    path.addRoundedRect(m_pixmap.rect(), 10, 10); // 设置圆角半径为10

    painter->setRenderHint(QPainter::SmoothPixmapTransform,
                           (this->mode == Qt::SmoothTransformation)); //设置 图片无锯齿样式
    // 绘制圆角矩形路径
    painter->drawPath(path);
}

void pictureItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    //只响应鼠标左键
    if (event->buttons() == Qt::LeftButton)
    {
        pressedSceneP = event->pos();
        isPressed = true;
    }
}

void pictureItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        if (isPressed
            && boundingRect().contains(pressedSceneP)
            && boundingRect().contains(event->pos()))
        {
            //确保释放的位置和按下的位置都在 boundingRect() 边界内
            isPressed = false;
            emit clickedId(type());//发送是哪个图片被点击了
        }
    }
}
