#ifndef PERSONFORM_H
#define PERSONFORM_H

#include <QWidget>

namespace Ui {
class personForm;
}

class personForm : public QWidget
{
    Q_OBJECT

public:
    explicit personForm(QWidget *parent = nullptr);
    ~personForm();

private:
    Ui::personForm *ui;

    // QWidget interface
protected:
    virtual void leaveEvent(QEvent *event) override;
};

#endif // PERSONFORM_H
