#ifndef PICTUREITEM_H
#define PICTUREITEM_H

#include <QGraphicsObject>

class pictureItem : public QGraphicsObject
{
    Q_OBJECT
public:
    explicit pictureItem(const QPixmap &pixmap, QGraphicsItem *parent = nullptr);
    virtual ~pictureItem();

    void setType(int type);
    void setTransformationMode(Qt::TransformationMode mode); //设置模式

    // QGraphicsItem interface
    virtual QRectF boundingRect() const override;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    virtual int type() const override;

    //判断是否发出点击信号
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

signals:
    void clickedId(int);

private:
    QPixmap m_pixmap;
    int m_type; //方便管理和区分不同的图片
    Qt::TransformationMode mode; //渲染样式

    QPointF pressedSceneP; //按压的点
    bool isPressed; // 判断按压
};

#endif // PICTUREITEM_H
