#ifndef GPTTHREAD_H
#define GPTTHREAD_H

#include <QWidget>

class QNetworkAccessManager;
class GptThread : public QObject
{
    Q_OBJECT
public:
    explicit GptThread(QWidget *parent = nullptr);

    void work(QString str);//http请求
    void fromJson(QString str);//解析json并且发送信息
signals:
    void sendAns(QString str);

private:
    QNetworkAccessManager *manager;
};

#endif // GPTTHREAD_H
