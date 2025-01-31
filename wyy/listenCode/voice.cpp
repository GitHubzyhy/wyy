#include "voice.h"
#include "ui_voice.h"
#include <QDebug>

Voice::Voice(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Voice)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_StyledBackground);

    flag = true;
}

Voice::~Voice()
{
    delete ui;
}

void Voice::leaveEvent(QEvent *event)
{
    Q_UNUSED(event);
    close();
}
//调节音量
void Voice::on_verticalSlider_valueChanged(int value)
{
    emit voiceValue(value);
}
//是否静音
void Voice::on_btnVoice_clicked()
{
    if (!flag)
    {
        ui->btnVoice->setStyleSheet("QPushButton#btnVoice {"
                                    "border-top: 1px solid rgb(205, 205, 205);"
                                    "border-image:url(:/bottom/images/bottom/voice4.png);"
                                    "}"
                                    "QPushButton#btnVoice:hover {"
                                    "border-image:url(:/bottom/images/bottom/voice5.png);"
                                    "}"
                                   );
        emit isMute(false);
    }
    else
    {
        ui->btnVoice->setStyleSheet("QPushButton#btnVoice {"
                                    "border-top: 1px solid rgb(205, 205, 205);"
                                    "border-image:url(:/bottom/images/bottom/voice6.png);"
                                    "}"
                                    "QPushButton#btnVoice:hover {"
                                    "border-image:url(:/bottom/images/bottom/voice7.png);"
                                    "}"
                                   );

        emit isMute(true);
    }
    flag ^= 1;
}
