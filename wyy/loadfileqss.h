#ifndef LOADFILEQSS_H
#define LOADFILEQSS_H

#include <QFile>
#include <QDebug>

class LoadFileQss
{
public:
    static QByteArray setStyle(const QString &fileName)
    {
        QFile fileQss(fileName);
        if (!fileQss.open(QFile::ReadOnly))
        {
            //打开错误
            qDebug() << "打开文件错误" << fileQss.errorString();
            return ""; //不处理文件
        }
        return fileQss.readAll();
    }
};

#endif // LOADFILEQSS_H
