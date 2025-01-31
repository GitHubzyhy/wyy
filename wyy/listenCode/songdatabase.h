#ifndef SONGDATABASE_H
#define SONGDATABASE_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>

//数据类
struct SongInfo
{
    SongInfo(int num, QString MusicName, QString MusicAuthor, QString mTime
    , int language, QString EMixSongID)
    {
        this->num = num;
        this->MusicName = MusicName;
        this->MusicAuthor = MusicAuthor;
        this->mTime = mTime;
        this->language = language;
        this->EMixSongID = EMixSongID;
    }
    int num;//类似ID
    QString MusicName;//歌曲
    QString MusicAuthor;//歌手
    QString mTime;//时长或者历史时间
    int language;//语言，后续过滤需要
    QString EMixSongID;//用于拼接url
};

//歌曲的数据库操作
class SongDatabase : public QObject
{
    Q_OBJECT
public:
    SongDatabase(const SongDatabase &obj) = delete;
    SongDatabase &operator=(const SongDatabase &obj) = delete;
    ~SongDatabase();
    static SongDatabase *getInstance()
    {
        //饿汉单例模式
        static SongDatabase songDatabase;//分配内存和初始化只会执行一次
        return &songDatabase;
    }

    void initDataBase();//初始化数据库
    void execSql(const QString &sql);//执行sql语句 create/update/delete....
    void clearTable(const QString &tableName);//清空数据表
    void insertTable(const QString &tableName, const SongInfo &songInfo, bool flag = false); //将输入插入到指定表中
    int getCnt() const;//获取表中有多少个数据
    QList<SongInfo> queryInfo(const QString &tableName, const QString &filter = ""); //查询表中所有信息

    bool getIsAccess() const;//获取是否执行成功
    QString getErrMsg() const;//获取数据库执行的错误信息

private:
    SongDatabase() = default;

    QSqlDatabase db;
    QSqlQuery query;
    int cnt;//记录表中数据个数
    bool isAccess;//是否执行成功
    QString errMsg;//数据库执行的错误信息
};

#endif // SONGDATABASE_H
