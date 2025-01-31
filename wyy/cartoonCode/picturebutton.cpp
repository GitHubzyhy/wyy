#include "picturebutton.h"
#include <QPainter>
#include <QPainterPath>//绘图操作的集合

pictureButton::pictureButton(QWidget *parent)
    : QAbstractButton(parent)
{
    setCheckable(true);//设置二态按钮
    setFixedSize(8, 8);//设置自定义按钮大小
}

pictureButton::~pictureButton() {}

void pictureButton::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);//抗锯齿

    QPen drawPen;
    drawPen.setWidth(1);
    //选中为红,未选中为灰
    drawPen.setColor(Qt::gray);
    painter.setPen(drawPen);

    QPainterPath drawPath;
    QRectF drawRect = this->rect();
    drawPath.addRoundedRect(drawRect, 4, 4); //带有圆角的给定矩形 rect 添加到路径
    painter.setClipPath(drawPath);//路径外的部分不会被渲染

    if (isChecked())
    {
        painter.fillRect(drawRect, QColor(236, 65, 65)); //变成红色
    }
    else
        painter.fillRect(drawRect, QColor(128, 128, 128, 128));//正常是灰色

}
//鼠标进入
void pictureButton::enterEvent(QEvent *event)
{
    if (!isChecked())
        setChecked(true);
    emit entered();

    return QAbstractButton::enterEvent(event);
}
