#include "songdatabase.h"
#include <QMessageBox>
#include <QSqlDatabase>
#include <QDebug>
#include <QSqlError>
#include <QSqlRecord>

SongDatabase::~SongDatabase()
{
    if (db.isOpen())
        db.close();
}
//初始化数据库
void SongDatabase::initDataBase()
{
    // 1. 添加驱动连接对应数据库
    db = QSqlDatabase::addDatabase("QSQLITE"); // 如果不存在，则添加一个SQLite数据库连接
    db.setDatabaseName("song.db"); // 设置数据库文件名为"song.db"
    // 2、打开数据库，读取数据表
    if (!db.open()) // 尝试打开数据库
    {
        // 打开数据库失败，显示错误信息
        QMessageBox::critical(nullptr, "错误", db.lastError().text());
        return;
    }
    else
    {
        // 3、执行数据表创建操作
        QString sql = "create table if not exists songlist(id integer , MusicName text, MusicAuthor text, Duration varchar(11) ,language integer, EMixSongID text);"; // 创建歌曲列表表格的SQL语句
        execSql(sql); // 执行SQL语句

        // 创建歌曲记录表格
        sql = "create table if not exists songhistory(id integer primary key autoincrement, MusicName text, MusicAuthor text, HistoryTime varchar(11) ,language integer, EMixSongID text);";
        execSql(sql);
    }
}
//执行sql语句 create/update/delete....
void SongDatabase::execSql(const QString &sql)
{
    isAccess = true;

    QSqlQuery query; // 执行sql语句对象
    if (!query.exec(sql))
    {
        // 执行失败，显示错误信息
        errMsg = query.lastError().text();
        isAccess = false;
    }
}

//清空数据表
void SongDatabase::clearTable(const QString &tableName)
{
    QSqlQuery query;
    query.exec(QString("DELETE FROM  %1")
               .arg(tableName));//songhistory"
    query.exec(QString("DELETE FROM  sqlite_sequence WHERE name =\'%1\'")
               .arg(tableName));
}
//向数据表插入数据
void SongDatabase::insertTable(const QString &tableName, const SongInfo &songInfo, bool flag)
{
    isAccess = true;

    QSqlQuery query;
    QString sql;
    if (flag)
        sql = QString("insert into %1 values(NULL, '%2', '%3', '%4' ,%5 ,'%6');")
              .arg(tableName)
              .arg(songInfo.MusicName)//歌曲名
              .arg(songInfo.MusicAuthor)///歌手
              .arg(songInfo.mTime)//时长
              .arg(songInfo.language)//语言
              .arg(songInfo.EMixSongID);//哈希
    else
        sql = QString("insert into %1 values(%2, '%3', '%4', '%5' ,%6 ,'%7');")
              .arg(tableName)
              .arg(songInfo.num) //ID
              .arg(songInfo.MusicName)//歌曲名
              .arg(songInfo.MusicAuthor)///歌手
              .arg(songInfo.mTime)//时长
              .arg(songInfo.language)//语言
              .arg(songInfo.EMixSongID);//哈希

    if (!query.exec(sql))
    {
        // 插入失败，显示错误信息
        errMsg = query.lastError().text();
        isAccess = false;
    }
}
//获取表中有多少个数据
int SongDatabase::getCnt() const
{
    return cnt;
}
//查询表中所有信息
QList<SongInfo> SongDatabase::queryInfo(const QString &tableName, const QString &filter)
{
    isAccess = true;

    QList<SongInfo> songInfoList;
    cnt = 0; //每次执行都要清0

    QSqlQuery query;
    QString sql;
    if (filter.isEmpty())
        sql = QString("select * from %1").arg(tableName);
    else
        sql = QString("select * from %1 where %2")
              .arg(tableName)
              .arg(filter);

    if (!query.exec(sql)) // 执行查询操作
    {
        // 查询失败，显示错误信息
        errMsg = query.lastError().text();
        isAccess = false;

        return songInfoList;
    }
    while (query.next()) // 遍历查询结果
    {
        QSqlRecord rec = query.record(); // 获取查询结果的记录

        int num = rec.value("id").toInt();
        QString MusicName = rec.value("MusicName").toString();
        QString MusicAuthor = rec.value("MusicAuthor").toString();
        QString mTime;//时长或者历史时间
        if ("songlist" == tableName)
            mTime = rec.value("Duration").toString();
        else
            mTime = rec.value("HistoryTime").toString();

        int language = rec.value("language").toInt();
        QString EMixSongID = rec.value("EMixSongID").toString();

        cnt++;
        songInfoList << SongInfo{num, MusicName, MusicAuthor, mTime, language, EMixSongID};
    }
    return songInfoList;
}
//获取是否执行成功
bool SongDatabase::getIsAccess() const
{
    return isAccess;
}
//获取数据库执行的错误信息
QString SongDatabase::getErrMsg() const
{
    return errMsg;
}
