#ifndef GALLERYPRIVATE_H
#define GALLERYPRIVATE_H

#include"galleryitem.h"

class QWidget;
class QPropertyAnimation;
class GalleryPrivate
{
public:
    GalleryPrivate();
    //创建画廊
    void setItemRes(int itemWidth, int itemHeight, const QList<GalleryItem *> &itemData );

    int getPreviousItemIndex();//容器向左移动
    int getNextItemIndex();//容器向右移动
    void showImage(int index);//显示对应位置

    QWidget *getItemViewport() const;//获取view 的指针
private:
    //对象的宽, 对象的高, 对象的个数
    int itemWidth, itemHeght, itemCount;
    const int Padding = 12;
    QWidget *itemViewport; //能够看到对象的区域
    QWidget *itemContainer; //对象的全部区域

    int currentItem;//当前的对象的索引
    QPropertyAnimation *animation;//动画设置
};

#endif // GALLERYPRIVATE_H
