#ifndef MUSICITEM_H
#define MUSICITEM_H

#include <QWidget>

namespace Ui
{
    class MusicItem;
}

class MusicItem : public QWidget
{
    Q_OBJECT

public:
    explicit MusicItem(QWidget *parent = nullptr);
    ~MusicItem();

    void setNum(int id);
    void setMusicName(QString musicName);
    void setMusicAuthor(QString author);
    void setMusicTimer(QString strTimer);
    void setLanguage(int value);

    int getLanguage();

private:
    Ui::MusicItem *ui;

    int language;
};

#endif // MUSICITEM_H
