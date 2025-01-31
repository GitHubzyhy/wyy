#ifndef MUSICFORM_H
#define MUSICFORM_H

#include <QWidget>

namespace Ui
{
    class MusicForm;
}

class MusicForm : public QWidget
{
    Q_OBJECT

public:
    explicit MusicForm(QWidget *parent = nullptr);
    ~MusicForm();

    void setMusicName(QString musicName);
    void setMusicAuthor(QString musicAuthor);
    void setMusicPicture(QString url);

    QString getName()
    {
        return Name;
    }
    QString getAuthor()
    {
        return Author;
    }

private:
    Ui::MusicForm *ui;
    QString Name, Author;
};

#endif // MUSICFORM_H
