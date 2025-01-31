#include "gptthread.h"
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QMessageBox>

GptThread::GptThread(QWidget *parent)
    : QObject{parent}
{
}
//http请求
void GptThread::work(QString str)
{
    manager = new QNetworkAccessManager;
    QUrl url("https://spark-api-open.xf-yun.com/v1/chat/completions");
    QNetworkRequest request(url);
    //1. 构造请求体
    // 设置请求头
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Authorization", "Bearer PnkGpVcrxtrQchLNkVMJ:fnAlBPoCLgryYtItiUiV"); // 替换为实际的APIPassword

    // 创建请求体
    QJsonObject data;
    data["model"] = "generalv3.5";
    data["stream"] = true;

    // 添加消息
    QJsonArray messages;
    QJsonObject messageObject;
    messageObject["role"] = "user";
    messageObject["content"] = str;
    messages.append(messageObject);
    data["messages"] = messages;

    // 转换为 JSON 数据
    QJsonDocument doc(data);
    QByteArray jsonData = doc.toJson();
    //2. 发送 POST 请求
    QNetworkReply *reply = manager->post(request, jsonData);
    connect(reply, &QNetworkReply::readyRead,
            this, [reply, this]()
    {
        // 响应的状态码为200, 表示请求成功
        int status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute)
                          .toInt();
        if ( reply->error() != QNetworkReply::NoError || status_code != 200 )
            QMessageBox::warning(nullptr, "提示", "请求数据失败！", QMessageBox::Ok);
        //获取响应信息
        QByteArray reply_data = reply->readAll();
        qDebug() << "最原始数据" << reply_data;
        QString byteArray = QString::fromUtf8(reply_data);
        qDebug() << "-------------原始数据-------------\n" << byteArray;
        fromJson( reply_data);
    });

    connect(reply, &QNetworkReply::finished, [reply]()
    {
        if (reply->error() != QNetworkReply::NoError)
            qDebug() << "Error:" << reply->errorString();
        reply->deleteLater();
    });

}
//解析json并且发送信息
void GptThread::fromJson(QString str)
{
    QStringList strList = str.split("data: ");//strList[0]一定为""

    for (int i = 1; i < strList.count(); i++)
    {
        //1. 移除前缀后缀
        strList[i].remove("data: ");
        strList[i].remove("\n\n[DONE]\n\n");

        // 2. 打印转换后的 JSON 字符串，检查格式
        qDebug() << "-------------真正的json格式: -------------\n" << strList[i];
        QJsonParseError jsonError;
        QJsonDocument jsonDoc = QJsonDocument::fromJson( strList[i].toUtf8(), &jsonError);

        if (jsonError.error != QJsonParseError::NoError)
        {
            qDebug() << "JSON解析错误:" << jsonError.errorString();
            return;
        }

        // 确保JSON数据是对象
        if (!jsonDoc.isObject())
        {
            qDebug() << "JSON格式错误: 需要是对象类型";
            return;
        }
        // 解析顶层对象
        QJsonObject jsonObj = jsonDoc.object();

        // 获取字段 "choices"
        QJsonArray choicesArray = jsonObj.value("choices").toArray();
        for (int i = 0; i < choicesArray.size(); ++i)
        {
            QJsonObject choiceObj = choicesArray[i].toObject();
            QJsonObject deltaObj = choiceObj.value("delta").toObject();
            QString content = deltaObj.value("content").toString();
            qDebug() << "  content:" << content;
            emit sendAns(content);
        }
    }
}
