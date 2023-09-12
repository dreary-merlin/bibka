#include "mainwindow.h"
#include "book.h"
#include <QApplication>
#include "bookdb.h"
Q_DECLARE_METATYPE(Book);
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qRegisterMetaType<Book>();
    BOOK_DB.createTables();
    MainWindow w;
    w.setWindowState(Qt::WindowMaximized);
    w.show();

    return a.exec();
}
