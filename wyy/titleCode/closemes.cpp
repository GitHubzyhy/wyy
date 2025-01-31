#include "closemes.h"
#include "ui_closemes.h"

#include <QButtonGroup>

CloseMes::CloseMes(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::CloseMes)
{
    ui->setupUi(this);

    setWindowFlags(Qt::FramelessWindowHint);

    QButtonGroup *btnGroup = new QButtonGroup(this);
    btnGroup->addButton(ui->radioBtn1, 0);
    btnGroup->addButton(ui->radioBtn2, 1);
    btnGroup->button(1)->setChecked(true);
    isExit = true;//默认初始化为直接退出
    connect(btnGroup, &QButtonGroup::idClicked,
            this, [this](int id)
    {
        isExit = id == 0 ? false : true;
    });
}

CloseMes::~CloseMes()
{
    delete ui;
}

bool CloseMes::getAns()
{
    return isExit;
}
