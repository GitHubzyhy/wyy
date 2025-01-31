#ifndef NEWCOURIERITEM_H
#define NEWCOURIERITEM_H

#include <QWidget>

namespace Ui
{
    class NewCourierItem;
}

class NewCourierItem : public QWidget
{
    Q_OBJECT

public:
    explicit NewCourierItem(QWidget *parent = nullptr);
    ~NewCourierItem();

    void setNum(int id);//设置id
    void setPicture(QString url);//设置图片
    void setMusicName(QString musicName);//设置歌名
    void setMusicAuthor(QString author);//设置作者
    void setMusicAlbum(QString name);//设置专辑
    void setMusicTimer(QString strTimer);//设置歌曲时长
private:
    Ui::NewCourierItem *ui;
};

#endif // NEWCOURIERITEM_H
