#include "personform.h"
#include "ui_personform.h"

personForm::personForm(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::personForm)
{
    ui->setupUi(this);

    setAttribute(Qt::WA_StyledBackground); //设置样式表
}

personForm::~personForm()
{
    delete ui;
}

void personForm::leaveEvent(QEvent *event)
{
    Q_UNUSED(event);
    close();
}
