#include "newdisc.h"
#include "ui_newdisc.h"
#include "loadfileqss.h"

NewDisc::NewDisc(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::NewDisc)
{
    ui->setupUi(this);

    setInitData();
}

NewDisc::~NewDisc()
{
    delete ui;
}
//初始化
void NewDisc::setInitData()
{
    this->setStyleSheet(LoadFileQss::setStyle(":/qss/qss/disc/init.css"));

    ui->labName01->setText("进击之月");
    ui->labDiscAu01->setText("赵磊");
    ui->labName02->setText("一起向未来");
    ui->labDiscAu02->setText("易烊千玺");
    ui->labName03->setText("All Too Well");
    ui->labDiscAu03->setText("Switf");
    ui->labName04->setText("B级鲨鱼");
    ui->labDiscAu04->setText("裴德");
}
//选择展示哪个页面
void NewDisc::setChose(int id)
{
    switch (id)
    {
        case 0:
            setInitData();
            break;
        case 1:
            setChinese();
            break;
        case 2:
            setEurope();
            break;
        case 3:
            setKorea();
            break;
        case 4:
            setJapan();
            break;
    }
}
//设置中文
void NewDisc::setChinese()
{
    this->setStyleSheet(LoadFileQss::setStyle(":/qss/qss/disc/chinese.css"));

    ui->labName01->setText("进击之月");
    ui->labDiscAu01->setText("赵磊");
    ui->labName02->setText("一起向未来");
    ui->labDiscAu02->setText("易烊千玺");
    ui->labName03->setText("B级鲨鱼");
    ui->labDiscAu03->setText("裴德");
    ui->labName04->setText("云");
    ui->labDiscAu04->setText("林鹿言");
}
//设置欧美
void NewDisc::setEurope()
{
    this->setStyleSheet(LoadFileQss::setStyle(":/qss/qss/disc/Europe.css"));

    ui->labName01->setText("Fay As Me");
    ui->labDiscAu01->setText("Bruno Mars");
    ui->labName02->setText("Already Dead");
    ui->labDiscAu02->setText("Juice WRLD");
    ui->labName03->setText("All Too Well");
    ui->labDiscAu03->setText("Switf");
    ui->labName04->setText("Bite Me");
    ui->labDiscAu04->setText("Avril Lavigne");
}
//设置韩国
void NewDisc::setKorea()
{
    this->setStyleSheet(LoadFileQss::setStyle(":/qss/qss/disc/Korea.css"));

    ui->labName01->setText("거대한 말");
    ui->labDiscAu01->setText("B1A4");
    ui->labName02->setText("SNAPSHOT");
    ui->labDiscAu02->setText("jessica");
    ui->labName03->setText("DAYDREAM");
    ui->labDiscAu03->setText("jessica");
    ui->labName04->setText("같은 곳을 향해가");
    ui->labDiscAu04->setText("유지현 김상지");
}
//设置日文
void NewDisc::setJapan()
{
    this->setStyleSheet(LoadFileQss::setStyle(":/qss/qss/disc/Japan.css"));

    ui->labName01->setText("MAKAFUKA");
    ui->labDiscAu01->setText("RADWIMPS");
    ui->labName02->setText("Chopstick");
    ui->labDiscAu02->setText("Niziu");
    ui->labName03->setText("ラストシーン");
    ui->labDiscAu03->setText("菅田将暉");
    ui->labName04->setText("starting over");
    ui->labDiscAu04->setText("三代目");
}
