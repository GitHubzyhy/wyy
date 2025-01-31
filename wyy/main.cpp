#include "wyy.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    if (QT_VERSION >= QT_VERSION_CHECK(5, 6, 0))
        QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    // qputenv("QT_SCALE_FACTOR", QByteArray::number(0.9));
    QApplication a(argc, argv);
    Wyy w;
    w.show();
    return a.exec();
}
