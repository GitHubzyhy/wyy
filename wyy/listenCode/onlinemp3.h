#ifndef ONLINEMP3_H
#define ONLINEMP3_H

#include <QObject>

class QMediaPlayer;
class QMediaPlaylist;
class QNetworkRequest;
class QNetworkAccessManager;
class QNetworkReply;
class QSlider;
class QLabel;
class SongDatabase;
class MusicForm;
class SongInfo;
class OnlineMp3 : public QObject
{
    Q_OBJECT
public:
    explicit OnlineMp3(MusicForm * &musicForm,
                       SongDatabase *&songDatabase, QObject *parent = nullptr);

    void setMusicList(QSlider * &slider, QLabel * &startTime, QLabel * &endTime);//初始化音乐
    void setVolume(int value);//音量
    void setMuted(bool isMuted);//静音
    void setPlayerState(int flag);//0:开始播放 1: 暂停 2: 停止
    void setPosition(int value);//设置播放进度

    void httpAccess(QString url);//访问http网页
    void netReply(QNetworkReply *reply);//读取网络数据
    QString getSearchUrl(QString musicName);//获取搜索的url
    QList<SongInfo *> searchPlayer(QString musicName);//搜索音乐
    QList<SongInfo *> hashJsonAnalysis(QByteArray JsonData);//解析json数据

    QString getDownloadUrl(QString encode_album_audio_id);//获取播放的url
    void downloadPlayer(QString encode_album_audio_id);//播放选中的音乐
    void lyricTextShow(QString str);//显示歌词
    void setMusicPic();//设置正在播放的音乐的图片

signals:
    void finish(QByteArray Data);//请求完成信号
    void lyricShow(QString url);//显示歌词的信号

private:
    //播放音乐部分
    QMediaPlayer *player;//播放器
    QMediaPlaylist *playlist;//播放列表

    // 搜索引擎
    QString kugouSearchApi = "https://complexsearch.kugou.com/v2/search/song?";
    QString kugouDownldadApi = "https://wwwapi.kugou.com/play/songinfo?";
    QNetworkRequest *request;
    QNetworkAccessManager *manager;
    QNetworkReply *downReply;

    SongDatabase *songDatabase;
    QString musicUrl;//存储解析出来的url
    QString curPicPath;//当前播放的音乐中的图片下载路径
    /*是否是双击播放歌曲,因为要进行2次不同url的http请求*/
    bool isdoubleClick = false;

    MusicForm *musicForm;//page1的左下角模块
};

#endif // ONLINEMP3_H
