#include "galleryprivate.h"
#include <QPropertyAnimation>
#include <QWidget>
GalleryPrivate::GalleryPrivate()
{
    itemWidth = itemHeght = itemCount = currentItem = 0;
}
//创建画廊
void GalleryPrivate::setItemRes(int itemWidth, int itemHeight, const QList<GalleryItem *> &itemData)
{

    this->itemWidth = itemWidth;//143px
    this->itemHeght = itemHeight;//226px
    this->itemCount = itemData.size();//8个

    //1. viewport 的固定大小 显示5个item 与中间4个间隔 每个固定15
    itemViewport = new QWidget;
    itemViewport->setFixedSize(this->itemWidth * 5 + Padding * 4, this->itemHeght);
    //2. itemContainer 的大小为itemCount个对象+itemCount-1个空隙
    itemContainer = new QWidget(itemViewport);//让视图作为容器的父亲是因为多余的部分可以隐藏
    itemContainer->resize(itemCount *this->itemWidth + (itemCount - 1) * Padding, this->itemHeght);
    itemContainer->move(0, 0);

    //让对象添加到 对应的位置。
    for (int index = 0; index < itemCount; index++)
    {
        itemData[index]->setParent(itemContainer);
        itemData[index]->move(index *this->itemWidth + (index) *Padding, 0);
    }

    //动画设置
    currentItem = 0;//当前对象索引
    animation = new QPropertyAnimation(itemContainer, "pos"); //按点移动的动画（移动）
    animation->setDuration(200);//持续时间
}

QWidget *GalleryPrivate::getItemViewport() const
{
    return itemViewport;
}

//容器向左移动
int GalleryPrivate::getPreviousItemIndex()
{
    if (currentItem)
        currentItem--;
    return currentItem;
}
//容器向右移动
int GalleryPrivate::getNextItemIndex()
{
    if (currentItem < itemCount - 5)
        currentItem++;
    return currentItem;
}
//动画设置
void GalleryPrivate::showImage(int index)
{
    if (index >= 0 && index < itemCount - 4)//最多移动3次
    {
        if (index == 0)
            animation->setEndValue(QPoint(0, 0));
        else
            animation->setEndValue(QPoint(-(index *this->itemWidth + (index)*Padding), 0));

        animation->start();//动画开始
    }
}
