#ifndef VOICE_H
#define VOICE_H

#include <QWidget>

namespace Ui
{
    class Voice;
}

class Voice : public QWidget
{
    Q_OBJECT

public:
    explicit Voice(QWidget *parent = nullptr);
    ~Voice();

signals:
    void voiceValue(int value);
    void isMute(bool flag);
    // QWidget interface
protected:
    virtual void leaveEvent(QEvent *event) override;
private slots:
    void on_verticalSlider_valueChanged(int value);
    void on_btnVoice_clicked();

private:
    Ui::Voice *ui;

    bool flag;//判断是否静音
};

#endif // VOICE_H
