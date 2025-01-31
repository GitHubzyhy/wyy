#ifndef CHANGECOLORFORM_H
#define CHANGECOLORFORM_H

#include <QWidget>

namespace Ui
{
    class ChangeColorForm;
}

class ChangeColorForm : public QWidget
{
    Q_OBJECT

public:
    explicit ChangeColorForm(QWidget *parent = nullptr);
    ~ChangeColorForm();

private slots:
    void on_ptnRed_clicked();
    void on_ptnBlue_clicked();
    void on_ptnPink_clicked();
    void on_ptnOrange_clicked();
    void on_ptnYellow_clicked();
    void on_ptnGreen_clicked();

signals:
    void colorId(QString color);

private:
    Ui::ChangeColorForm *ui;

    // QWidget interface
protected:
    virtual void leaveEvent(QEvent *event) override;
};

#endif // CHANGECOLORFORM_H
