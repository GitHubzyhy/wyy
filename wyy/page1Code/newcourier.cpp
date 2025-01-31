#include "newcourier.h"
#include "ui_newcourier.h"
#include "newcourieritem.h"

NewCourier::NewCourier(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::NewCourier)
{
    ui->setupUi(this);
    setInitData();
}

NewCourier::~NewCourier()
{
    delete ui;
}

//添加列表Item
void NewCourier::setItem(const int &num, const QString &picture,
                         const QString &name, const QString &author,
                         const QString &album, const QString &timer)
{
    NewCourierItem *nitem5 = new NewCourierItem(this);
    nitem5->setNum(num); //设置编号
    nitem5->setPicture(picture);
    nitem5->setMusicName(name);
    nitem5->setMusicAuthor(author);
    nitem5->setMusicAlbum(album);
    nitem5->setMusicTimer(timer);

    QListWidgetItem *item = new QListWidgetItem(ui->listAll);
    ui->listAll->setItemWidget(item, nitem5);
}

//选择展示哪个页面
void NewCourier::setChose(int id)
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

void NewCourier::setInitData()
{
    ui->listAll->clear();
    setItem(01, ":/NewMusic/images/NewMusic/01.png", "一起向未来", "易烊千玺", "一起向未来", "03:19");
    setItem(02, ":/NewMusic/images/NewMusic/02.png", "灯牌", "刘也", "朝圣者", "05:20");
    setItem(03, ":/NewMusic/images/NewMusic/03.png", "漠河舞厅2020", "柳爽", "漠河舞厅2020", "05:04");
    setItem(04, ":/NewMusic/images/NewMusic/04.png", "B级鲨鱼", "裴德", "B级鲨鱼", "03:51");
    setItem(05, ":/NewMusic/images/NewMusic/05.png", "春天花会开", "张杰", "时光音乐会", "04:14");
}

void NewCourier::setChinese()
{
    ui->listAll->clear();

    setItem(01, ":/NewMusic/images/NewMusic/06.png", "宣言", "赵磊", "进击之月", "04:28");
    setItem(02, ":/NewMusic/images/NewMusic/01.png", "一起向未来", "易烊千玺", "一起向未来", "03:19");
    setItem(03, ":/NewMusic/images/NewMusic/03.png", "漠河舞厅2020", "柳爽", "漠河舞厅2020", "05:04");
    setItem(04, ":/NewMusic/images/NewMusic/04.png", "B级鲨鱼", "裴德", "B级鲨鱼", "03:51");
    setItem(05, ":/NewMusic/images/NewMusic/05.png", "春天花会开", "张杰", "时光音乐会", "04:14");
}

void NewCourier::setEurope()
{
    ui->listAll->clear();

    setItem(01, ":/NewMusic/images/NewMusic/07.png", "Fay As Me", "Bruno Mars", "An Evening With Silk Sonic", "03:39");
    setItem(02, ":/NewMusic/images/NewMusic/08.png", "Already Dead", "Juice WRLD", "Already Dead", "03:51");
    setItem(03, ":/NewMusic/images/NewMusic/09.png", "Be Alive", "Beyonce", "Be Alive", "03:40");
    setItem(04, ":/NewMusic/images/NewMusic/10.png", "Bite Me", "Avril Lavigne", "Bite Me", "02:39");
    setItem(05, ":/NewMusic/images/NewMusic/11.png", "World We Used To Know", "Alan", "World We Used To Know", "04:14");

}

void NewCourier::setKorea()
{
    ui->listAll->clear();

    setItem(01, ":/NewMusic/images/NewMusic/12.png", "거대한 말", "B1A4", "거대한 말", "03:56");
    setItem(02, ":/NewMusic/images/NewMusic/13.png", "SNAPSHOT", "jessica", "제시카 & 크리스탈 - US로드", "03:10");
    setItem(03, ":/NewMusic/images/NewMusic/14.png", "DAYDREAM", "Jnbsics", "크리스탈", "05:04");
    setItem(04, ":/NewMusic/images/NewMusic/15.png", "같은 곳을 향해가", "유지현 김상지", "같은 곳을 향해가", "03:51");
    setItem(05, ":/NewMusic/images/NewMusic/16.png", "9号线少女", "Yourbeagle", "9号线少女", "03:00");
}

void NewCourier::setJapan()
{
    ui->listAll->clear();

    setItem(01, ":/NewMusic/images/NewMusic/17.png", "MAKAFUKA", "RADWIMPS", "FOREVER DAZE", "05:31");
    setItem(02, ":/NewMusic/images/NewMusic/18.png", "Chopstick", "Niziu", "Chopstick", "02:57");
    setItem(03, ":/NewMusic/images/NewMusic/19.png", "ラストシーン", "菅田将暉", "ラストシーン", "04:29");
    setItem(04, ":/NewMusic/images/NewMusic/20.png", "starting over", "三代目", "BEST BROTHERS", "04:20");
    setItem(05, ":/NewMusic/images/NewMusic/21.png", "Ito", "TOMORROW X", "Chaotic", "04:30");
}
