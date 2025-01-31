#include "changecolorform.h"
#include "ui_changecolorform.h"

ChangeColorForm::ChangeColorForm(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ChangeColorForm)
{
    ui->setupUi(this);

    setAttribute(Qt::WA_StyledBackground); //设置样式表
}

ChangeColorForm::~ChangeColorForm()
{
    delete ui;
}

void ChangeColorForm::on_ptnRed_clicked()
{
    emit colorId("Red");
}

void ChangeColorForm::on_ptnBlue_clicked()
{
    emit colorId("Blue");
}

void ChangeColorForm::on_ptnPink_clicked()
{
    emit colorId("Pink");
}

void ChangeColorForm::on_ptnOrange_clicked()
{
    emit colorId("Orange");
}

void ChangeColorForm::on_ptnYellow_clicked()
{
    emit colorId("Yellow");
}

void ChangeColorForm::on_ptnGreen_clicked()
{
    emit colorId("Green");
}

void ChangeColorForm::leaveEvent(QEvent *event)
{
    Q_UNUSED(event);
    close();
}
