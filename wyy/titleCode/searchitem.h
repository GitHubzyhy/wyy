#ifndef SEARCHITEM_H
#define SEARCHITEM_H

#include <QWidget>

namespace Ui
{
    class SearchItem;
}

class SearchItem : public QWidget
{
    Q_OBJECT

public:
    explicit SearchItem(QWidget *parent = nullptr);
    ~SearchItem();

    void setLabNumColor(bool bo); //排行号样式设置

    void setLabNumText(QString str);//排行号文本设置
    void setLabNameText(QString str);

    void setLabrankingText(QString str);//其他
    void setLabMessageText(QString str);//歌曲介绍

signals:
    void musicText(QString str);

private:
    Ui::SearchItem *ui;

    // QWidget interface
protected:
    virtual void mouseDoubleClickEvent(QMouseEvent *event) override;
};

#endif // SEARCHITEM_H
