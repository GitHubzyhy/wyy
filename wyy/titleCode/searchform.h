#ifndef SEARCHFORM_H
#define SEARCHFORM_H

#include <QWidget>

namespace Ui
{
    class SearchForm;
}

class SearchForm : public QWidget
{
    Q_OBJECT

public:
    explicit SearchForm(QWidget *parent = nullptr);
    ~SearchForm();

    void setListItem();//设置QListWidget的条目
signals:
    void MuName(QString str);
private:
    Ui::SearchForm *ui;

};

#endif // SEARCHFORM_H
