#ifndef GALLERYITEM_H
#define GALLERYITEM_H

#include <QWidget>

namespace Ui
{
    class GalleryItem;
}

class GalleryItem : public QWidget
{
    Q_OBJECT

public:
    explicit GalleryItem(QWidget *parent = nullptr);
    ~GalleryItem();

    void setMessage(QString str);//设置说明
    void setPicture(QString url);//设置图片

private:
    Ui::GalleryItem *ui;
};

#endif // GALLERYITEM_H
