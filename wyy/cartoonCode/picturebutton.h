#ifndef PICTUREBUTTON_H
#define PICTUREBUTTON_H

#include <QAbstractButton>
//轮播图下面的类似单选按钮的类
class pictureButton : public QAbstractButton
{
    Q_OBJECT
public:
    explicit pictureButton(QWidget *parent = nullptr);
    ~pictureButton();

signals:
    void entered();

protected:
    virtual void paintEvent(QPaintEvent *);
    virtual void enterEvent(QEvent *event);
};

#endif // PICTUREBUTTON_H
