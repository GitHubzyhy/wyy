#ifndef XFGPT_H
#define XFGPT_H

#include <QWidget>
#include "gptthread.h"

namespace Ui
{
    class Xfgpt;
}

class QPlainTextEdit;
class Xfgpt : public QWidget
{
    Q_OBJECT

public:
    explicit Xfgpt(QWidget *parent = nullptr);
    ~Xfgpt();

    //回复
    void answerEdit();
    void appendEdit(QString str);

    void tipQuery(int id);//按照提示查询
    void setNaAu(QString Name, QString Author);//设置歌名，作者

signals:
    void query(QString str);
    void isClose();

private slots:
    void on_btnSend_clicked();
    void on_btnClear_clicked();
    void on_btnQuit_clicked();

private:
    Ui::Xfgpt *ui;

    GptThread gptThread;
    //回复
    QPlainTextEdit *editAns;
    int oldHeight;
    //查询
    QString name, author;

    QList<QWidget *> deleteLater;//清空界面
};

#endif // XFGPT_H
