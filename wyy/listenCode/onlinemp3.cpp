#include "onlinemp3.h"
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QSlider>
#include <QLabel>
#include <QMessageBox>
#include <QFileInfo>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonArray>
#include <QEventLoop>
#include <QDir>
#include "musicform.h"
#include "songdatabase.h"

OnlineMp3::OnlineMp3(MusicForm * &musicForm,
                     SongDatabase *&songDatabase, QObject *parent)
    : QObject{parent}
{
    this->musicForm = musicForm;
    this->songDatabase = songDatabase;

    //这个内容未开发暂时放到这里
    connect(this, &OnlineMp3::lyricShow,
            this, &OnlineMp3::lyricTextShow);
}
//初始化音乐
void OnlineMp3::setMusicList(QSlider * &slider, QLabel * &startTime, QLabel * &endTime)
{
    player = new QMediaPlayer(this);
    playlist = new QMediaPlaylist(this);
    playlist->setPlaybackMode(QMediaPlaylist::Loop); //设置为循环播放模式
    player->setPlaylist(playlist);
    player->setVolume(50);
    //1. 播放条进度变量，界面也要跟着更新
    connect(player, &QMediaPlayer::positionChanged, [ = ](qint64 duration)
    {
        if (slider->isSliderDown())//正处于 手动调整状态，不处理
            return ;

        slider->blockSignals(true);
        slider->setSliderPosition(int(duration));
        slider->blockSignals(false);

        int secs = int(duration) / 1000;
        int min = secs / 60; //取整
        secs = secs % 60; //剩余秒
        QString positionTime = QString("%1:%2").arg(QString::number(min), 2, '0')
                               .arg(QString::number(secs), 2, '0');
        startTime->setText(positionTime);
    });
    //2. 一般在切换播放文件
    connect(player, &QMediaPlayer::durationChanged, [ = ](qint64 duration)
    {
        slider->setMaximum(int(duration));
        int secs = int(duration) / 1000;
        int min = secs / 60; //取整
        secs = secs % 60; //剩余秒
        QString durationTime = QString("%1:%2").arg(QString::number(min), 2, '0')
                               .arg(QString::number(secs), 2, '0');
        endTime->setText(durationTime);
    });
    //加载音乐
    playlist->addMedia(QUrl::fromLocalFile("../../music/music.mp3"));//添加文件
    playlist->setCurrentIndex(0);//设置播放列表的位置
}
//音量
void OnlineMp3::setVolume(int value)
{
    player->setVolume(value);
}
//静音
void OnlineMp3::setMuted(bool isMuted)
{
    player->setMuted(isMuted);
}
//0:开始播放 1: 暂停 2: 停止
void OnlineMp3::setPlayerState(int flag)
{
    switch (flag)
    {
        case 0:
            player->play();
            break;
        case 1:
            player->pause();
            break;
        case 2:
            player->stop();
            break;
    }
}
//设置播放进度
void OnlineMp3::setPosition(int value)
{
    player->blockSignals(true);
    player->setPosition(value);
    player->blockSignals(false);
}

//访问http网页
void OnlineMp3::httpAccess(QString url)
{
    request = new QNetworkRequest;//实例化网络请求操作事项
    request->setUrl(url);//将url网页地址存入request请求中

    manager = new QNetworkAccessManager; //实例化网络管理（访问）
    manager->get(*request); //通过get,上传具体的请求
    //当网页回复消息，出发finished信号，读取数据
    connect(manager, &QNetworkAccessManager::finished,
            this, &OnlineMp3::netReply);
}
//读取网络数据
void OnlineMp3::netReply(QNetworkReply *reply)
{
    // 响应的状态码为200, 表示请求成功
    int status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute)
                      .toInt();

    qDebug() << "请求方式:" << reply->operation(); // 请求方式
    qDebug() << "响应状态码:" << status_code; // 状态码
    qDebug() << "url:" << reply->url(); // 请求的url
    qDebug() << "响应头:" << reply->rawHeaderList() << "\n\n"; // header

    if ( reply->error() != QNetworkReply::NoError || status_code != 200 )
        QMessageBox::warning(NULL, "提示", "请求数据失败！\r\n1. 检查网络是否连接\r\n2. 关闭VPN", QMessageBox::Ok);
    else
    {
        // 如果没有发生网络错误，则读取响应数据
        QByteArray data = reply->readAll();
        // 发射自定义的 finish 信号，将响应数据传递给槽函数
        emit finish(data);
    }

    reply->deleteLater();
}
//获取搜索的url
QString OnlineMp3::getSearchUrl(QString musicName)
{
    QDateTime time = QDateTime::currentDateTime();
    // 将当前时间转换为自纪元以来的秒数，并将其转换为字符串
    QString currentTimeString = QString::number(time.toSecsSinceEpoch() * 1000);

    // 构建签名列表
    QStringList signature_list;
    signature_list << "NVPh5oo715z5DIWAeQlhMDsWXXQV4hwt"
                   << "appid=1014"
                   << "bitrate=0"
                   << "callback=callback123"
                   << "clienttime=" + currentTimeString
                   << "clientver=1000"
                   << "dfid=08wNhe1SuXFi3mgjH54fVBZD"
                   << "filter=10"
                   << "inputtype=0"
                   << "iscorrection=1"
                   << "isfuzzy=0"
                   << "keyword=" + musicName
                   << "mid=2059bfd9bdb82110cdc4426ae85da0fd"
                   << "page=1"
                   << "pagesize=30"
                   << "platform=WebFilter"
                   << "privilege_filter=0"
                   << "srcappid=2919"
                   << "token="
                   << "userid=0"
                   << "uuid=2059bfd9bdb82110cdc4426ae85da0fd"
                   << "NVPh5oo715z5DIWAeQlhMDsWXXQV4hwt";

    // 将签名列表中的元素连接成一个字符串
    QString string = signature_list.join("");
    //qDebug()<< string;
    //生成 MD5 哈希
    QByteArray hashedData = QCryptographicHash::hash(string.toUtf8(), QCryptographicHash::Md5);
    QString signaturecode = hashedData.toHex();

    // 根据用户输入的 MP3 名称发起操作请求
    QString url = kugouSearchApi + QString("callback=callback123"
                                           "&srcappid=2919"
                                           "&clientver=1000"
                                           "&clienttime=%1"
                                           "&mid=2059bfd9bdb82110cdc4426ae85da0fd"
                                           "&uuid=2059bfd9bdb82110cdc4426ae85da0fd"
                                           "&dfid=08wNhe1SuXFi3mgjH54fVBZD"
                                           "&keyword=%2"
                                           "&page=1"
                                           "&pagesize=30"
                                           "&bitrate=0"
                                           "&isfuzzy=0"
                                           "&inputtype=0"
                                           "&platform=WebFilter"
                                           "&userid=0"
                                           "&iscorrection=1"
                                           "&privilege_filter=0"
                                           "&filter=10"
                                           "&token="
                                           "&appid=1014"
                                           "&signature=%3"
                                          )
                  .arg(currentTimeString)
                  .arg(musicName)
                  .arg(signaturecode);
    return url;
}
//搜索音乐
QList<SongInfo *> OnlineMp3::searchPlayer(QString musicName)
{
    QString searchUrl = getSearchUrl(musicName);
    qDebug() << "--------------------- 搜索歌曲的http请求---------------------";
    // 发起 HTTP 请求
    httpAccess(searchUrl);

    QByteArray JsonData;
    QEventLoop loop;//事件循环类

    // 等待 HTTP 请求完成并获取数据
    auto c = connect(this, &OnlineMp3::finish, this, [&](const QByteArray &data)
    {
        JsonData = data;
        loop.exit(1);
    });

    loop.exec();
    disconnect(c);

    return hashJsonAnalysis(JsonData);
}
//解析json数据
QList<SongInfo *> OnlineMp3::hashJsonAnalysis(QByteArray JsonData)
{
    QList<SongInfo *> songInfoList;

    musicUrl = "";//置空防止解析错误json时放上一首歌曲
    //移除callback123()
    // 找到第一个左括号 "(" 的位置
    if (!isdoubleClick) //不是双击播放歌曲
    {
        int leftBracketIndex = JsonData.indexOf('(');
        if (leftBracketIndex != -1)
        {
            // 找到最后一个右括号 ")" 的位置
            int rightBracketIndex = JsonData.lastIndexOf(')');
            if (rightBracketIndex != -1)
            {
                // 提取 JSON 数据，只要包裹的部分
                JsonData = JsonData.mid(leftBracketIndex + 1, rightBracketIndex - leftBracketIndex - 1);
            }
        }
    }
    // qDebug() << "解析后的" << JsonData;
    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(JsonData, &err);
    if (err.error != QJsonParseError::NoError)
    {
        qDebug() << "json格式错误\n" << JsonData.data();
        return {};
    }
    if (doc.isObject()) // 如果解析后的对象是一个 JSON 对象
    {
        QJsonObject data = doc.object(); // 获取 JSON 对象中的"data"字段
        if (data.contains("err_code"))
        {
            if (data.value("err_code").toInt() == 30020)
            {
                QMessageBox::critical(nullptr, "官方的url可能改变了", JsonData);
                return songInfoList;
            }
        }
        if (data.contains("data")) // 如果"data"字段存在
        {
            QJsonObject objectInfo = data.value("data").toObject(); // 获取"data"字段中的对象
            //1--------------------------------------------------------------------------
            // 如果包含歌词，发送歌词显示信号
            if (objectInfo.contains("lyrics"))
                emit lyricShow(objectInfo.value("lyrics").toString());
            // 返回音乐播放 URL
            if (objectInfo.contains("play_url"))
            {
                musicUrl = objectInfo.value("play_url").toString();
                qDebug() << "音乐播放的url" << objectInfo.value("play_url").toString();
            }
            if (objectInfo.contains("img"))
            {
                curPicPath = objectInfo.value("img").toString();
                qDebug() << "当前播放音乐的图片下载路径" << curPicPath;
            }
            //2--------------------------------------------------------------------------
            if (objectInfo.contains("lists")) // 如果"lists"字段存在
            {
                QJsonArray objectHash = objectInfo.value("lists").toArray(); // 获取"lists"字段中的数组
                for (int i = 0; i < objectHash.count(); i++) // 遍历数组中的每个元素
                {
                    QJsonObject album = objectHash.at(i).toObject(); // 获取数组元素中的对象
                    // 从对象中获取歌曲名、歌手、歌曲时长、语言、哈希值
                    QString musicName = album.value("FileName").toString();
                    QString singer = album.value("SingerName").toString();
                    int duration = album.value("Duration").toInt();

                    int language = 0;
                    QString strLang = album.value("trans_param").toObject()
                                      .value("language").toString();

                    if ( strLang == "国语")
                        language = 1;
                    else if (strLang == "粤语")
                        language = 2;
                    else if (strLang == "日语")
                        language = 3;
                    else if (strLang == "英语")
                        language = 4;
                    else if (strLang == "其他")
                        language = 5;
                    QString EMixSongID = album.value("EMixSongID").toString();

                    // 将解析出的信息插入数据库
                    SongInfo *songInfo = new SongInfo(i + 1, musicName, singer, QString::number(duration), language, EMixSongID);
                    songDatabase->insertTable("songlist", *songInfo);
                    if (!songDatabase->getIsAccess())
                        QMessageBox::critical(nullptr, "解析json数据这里插入songlist数据表错误", songDatabase->getErrMsg());
                    songInfoList << songInfo;
                    // updateUi(songInfo);
                }
            }
        }
    }
    return songInfoList;
}
//获取播放的url
QString OnlineMp3::getDownloadUrl(QString encode_album_audio_id)
{
    //signature---------------------------------
    QDateTime time = QDateTime::currentDateTime();
    // 将当前时间转换为自纪元以来的秒数，并将其转换为字符串
    QString currentTimeString = QString::number(time.toSecsSinceEpoch() * 1000);
    // currentTimeString = "1713782920612";
    // QString encode_album_audio_id = "j5yn384";
    // 构建签名列表
    QStringList signature_list;
    signature_list << "NVPh5oo715z5DIWAeQlhMDsWXXQV4hwt"
                   << "appid=1014"
                   << "clienttime=" + currentTimeString
                   << "clientver=20000"
                   << "dfid=08wNhe1SuXFi3mgjH54fVBZD"
                   << "encode_album_audio_id=" + encode_album_audio_id
                   << "mid=2059bfd9bdb82110cdc4426ae85da0fd"
                   << "platid=4"
                   << "srcappid=2919"
                   << "token="
                   << "userid=0"
                   << "uuid=2059bfd9bdb82110cdc4426ae85da0fd"
                   << "NVPh5oo715z5DIWAeQlhMDsWXXQV4hwt";

    // 将签名列表中的元素连接成一个字符串
    QString string = signature_list.join("");
    //qDebug()<< string;
    //生成 MD5 哈希
    QByteArray hashedData = QCryptographicHash::hash(string.toUtf8(), QCryptographicHash::Md5);

    //开始构建url---------------------------------
    QString signaturecode = hashedData.toHex();// 将哈希数据转换为十六进制字符串
    QString url = kugouDownldadApi + QString("srcappid=2919"
                  "&clientver=20000"
                  "&clienttime=%1"
                  "&mid=2059bfd9bdb82110cdc4426ae85da0fd"
                  "&uuid=2059bfd9bdb82110cdc4426ae85da0fd"
                  "&dfid=08wNhe1SuXFi3mgjH54fVBZD"
                  "&appid=1014"
                  "&platid=4"
                  "&encode_album_audio_id=%2"
                  "&token="
                  "&userid=0"
                  "&signature=%3")
                  .arg(currentTimeString)
                  .arg(encode_album_audio_id)
                  .arg(signaturecode);
    return url;
}
//播放选中的音乐
void OnlineMp3::downloadPlayer(QString encode_album_audio_id)
{
    QString downloadUrl = getDownloadUrl(encode_album_audio_id);
    qDebug() << "------------------ 播放歌曲的http请求  ------------------";
    // 发起 HTTP 请求获取歌曲数据
    httpAccess(downloadUrl);
    QByteArray jsonData;
    QEventLoop loop;

    //等待 HTTP 请求完成并获取数据
    auto d = connect(this, & OnlineMp3::finish, this, [&](const QByteArray &data)
    {
        jsonData = data;
        loop.exit(1);
    });
    loop.exec();
    disconnect(d);

    isdoubleClick = true;
    // 解析获取的 JSON 数据
    hashJsonAnalysis(jsonData);
    isdoubleClick = false;

    // 设置媒体并播放音乐
    player->setMedia(QUrl(musicUrl));

    setMusicPic();//从网络中下载音乐封面，显示到界面上
}

//显示歌词 未开发
void OnlineMp3::lyricTextShow(QString str)
{
    // qDebug() << str;
}
//设置正在播放的音乐的图片
void OnlineMp3::setMusicPic()
{
    QUrl newUrl = QUrl::fromUserInput(curPicPath);//URL地址
    if (!newUrl.isValid())
    {
        qDebug() << "无效的url" << curPicPath;
        QMessageBox::information(NULL, "下载音乐图片错误",
                                 QString("无效URL: %1 \n错误信息: %2")
                                 .arg(curPicPath, newUrl.errorString()));
        return;
    }
    QDir dir("./cachePic");
    // 检查目录是否存在
    if (!dir.exists())
        dir.mkpath(dir.absolutePath());

    QString path = dir.absolutePath() + QDir::separator() + newUrl.fileName();
    path = QDir::cleanPath(path);

    QFile *file = new QFile(path);
    if (file->exists())
    {
        QFileInfo fileInfo(*file);
        qDebug() << "路径： " << fileInfo.absoluteFilePath();
        musicForm->setMusicPicture(fileInfo.absoluteFilePath());
        qDebug() << "直接从缓存中寻找图片!!!!!!";
        return;
    }

    //这里的open实际上就是创建一个文件
    if (!file->open(QIODevice::WriteOnly))
    {
        //打开错误
        QMessageBox::warning(NULL, tr("下载图片这里错误")
                             , tr("打开文件错误：") + file->errorString());
        return; //不处理文件
    }
    qDebug() << "-------------下载图片会manage产生finished信号-------------";
    downReply = manager->get(QNetworkRequest(newUrl));
    connect(downReply, &QNetworkReply::readyRead,//缓冲区有可读取数据
            this, [ = ]()
    {
        file->write(downReply->readAll());
    });

    connect(downReply, &QNetworkReply::finished,//下载结束后
            this, [ = ]
    {
        QFileInfo fileInfo(*file);
        musicForm->setMusicPicture(fileInfo.absoluteFilePath());
    });
}
