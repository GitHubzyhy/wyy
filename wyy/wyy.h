#ifndef WYY_H
#define WYY_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui
{
    class Wyy;
}
QT_END_NAMESPACE

class SearchForm;
class ChangeColorForm;
class MessageForm;
class personForm;
class PictureWidget;
class GalleryPrivate;
class QButtonGroup;
class QListWidget;
class QStackedLayout;
class NewCourier;
class NewDisc;
class Voice;
class Xfgpt;
class MusicForm;
class QListWidgetItem;
class QStandardItemModel;
class MusicItem;
class SongDatabase;
class SongInfo;
class OnlineMp3;
class CloseMes;
class QSystemTrayIcon;
class Pet;
class Wyy : public QMainWindow
{
    Q_OBJECT

public:
    Wyy(QWidget *parent = nullptr);
    ~Wyy();

    void setMusicLeft(); //加载左下角模块
    void changeSkin(QString color);//切换皮肤对应的槽
    void setPictureWall(); //添加轮播图
    void setGallery(); //添加画廊部分

    void setButtonMus(); //设置歌单部分按钮
    void setSourceMus(int id); //通过按钮加载对应的数据

    /*排行榜模块*/
    void setItem(const QPair<int, bool> &pair, const QString &pic, const int &picc, //设置RankingItem
                 const QString &name, const QString &autor,
                 const QColor &color, QListWidget *&listWidget);
    void setRankData(); //设置排行榜数据
    void setRankNew(); //设置新歌榜
    /*歌手模块*/
    void setButtonGroup();//设置语言,分类，筛选的按钮组

    /*最新音乐模块*/
    void setNewMusicButton(); //设置新歌速递与新蝶上架模块
    void setNewMusicChooseButton(); //全部 华语 欧美 韩国 日本
    void setAddWidget(); //加载到堆栈布局里
    void changeChose(); //变化对应界面

    /*音乐播放部分*/
    QString convertSecondsToTime(int seconds);//秒转换成MM:SS 的时间格式
    void updateUi(SongInfo &songInfo);//更新界面
    void setFilter();//按语言过滤
    void setChose(int id);//选择给定语言种的歌曲
    void recentPlay();//最近播放

    void switchSong();//切换歌曲
    void initTrayIcon();//初始化系统托盘

private:
    Ui::Wyy *ui;

    //窗口移动定义
    QPoint movePoint;
    bool mousePress = false;
    MusicForm *musicForm;//左下角模块
    SearchForm *searchForm; //搜索框
    ChangeColorForm *changeColor;//换皮肤
    MessageForm *pMessageForm = nullptr;//个人消息
    personForm *p_person = nullptr; //个人中心
    PictureWidget *pictureWidget; //图片墙
    GalleryPrivate *gallerPrivate; //专属定制中的画廊
    QButtonGroup *pButtonMus; //音乐组

    /*歌手模块*/
    QButtonGroup *pButtonLaug; //语言
    QButtonGroup *pButtonClass; //歌手分类
    QButtonGroup *pButtonScreen; //筛选

    /*最新音乐模块*/
    QStackedLayout *stackedLayout;
    NewCourier *pCourier;
    NewDisc *pDisMusic;
    int currentFlag = 0; //0为新歌速递 1 新蝶上架
    int currentClass = 0; //0为 全部 1为中文 2为欧美 3为韩国 4为日本

    //gpt部分
    Xfgpt *gpt;
    Pet *pet;

    //播放音乐部分
    SongDatabase *songDatabase;
    OnlineMp3 *onlineMp3;
    Voice *voice;

    // 搜索/最近播放界面相关
    QStandardItemModel *model;
    bool flag = false;//0=最近播放 1=搜索
    int curRow;//第几首歌曲

    //系统托盘相关
    CloseMes *closeMes;
    QSystemTrayIcon *trayIcon;
    QMenu *trayMenu;
    QAction *showMessageAction;
    QAction *exitAction;
    // QObject interface
public:
    virtual bool eventFilter(QObject *watched, QEvent *event) override;
private slots:
    void on_pushButton_max_clicked(bool checked);
    void on_pushButton_close_clicked();
    void on_pushButton_min_clicked();
    void on_pushButton_cloth_clicked();//切换皮肤界面
    void on_pushButton_message_clicked();//个人消息
    void on_pushButton_name_clicked();//个人中心
    //个性定制中的按钮
    void on_btnPerL_clicked();
    void on_btnPerR_clicked();

    //音乐播放相关
    void on_horizontalSlider_music_valueChanged(int value);
    void on_pushButton_stop_clicked(bool checked);
    void on_pushButton_voice2_clicked();//音量

    void on_listT2_itemClicked(QListWidgetItem *item);
    void on_pushButton_search_clicked();
    void on_btnClear_clicked();
    void on_listView_doubleClicked(const QModelIndex &index);
};
#endif // WYY_H
