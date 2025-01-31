#ifndef CLOSEMES_H
#define CLOSEMES_H

#include <QDialog>
#include <QWidget>

namespace Ui
{
    class CloseMes;
}

class CloseMes : public QDialog
{
    Q_OBJECT

public:
    explicit CloseMes(QWidget *parent = nullptr);
    ~CloseMes();

    bool getAns();//主窗口根据返回值做选择
private:
    Ui::CloseMes *ui;

    bool isExit;//是否直接退出还是可以通过系统托盘打开
};
#endif // CLOSEMES_H
