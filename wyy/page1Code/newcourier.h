#ifndef NEWCOURIER_H
#define NEWCOURIER_H

#include <QWidget>

namespace Ui
{
    class NewCourier;
}

class NewCourier : public QWidget
{
    Q_OBJECT

public:
    explicit NewCourier(QWidget *parent = nullptr);
    ~NewCourier();

    void setItem(const int &num, const QString &picture,
                 const QString &name, const QString &author,
                 const QString &album, const QString &timer); //添加列表Item

    //设置对应数据
    void setInitData();//初始化界面
    void setChose(int id);//选择展示哪个页面
    void setChinese(); //设置中文
    void setEurope();//设置欧美
    void setKorea(); //设置韩国
    void setJapan(); //设置日文
private:
    Ui::NewCourier *ui;
};

#endif // NEWCOURIER_H
