#ifndef BOOKDB_H
#define BOOKDB_H

#include "dbfacade.h"
#include "singleton.h"
#include <QObject>
#include "book.h"
#include <vector>


class BookDB:public DBFacade {
    Q_OBJECT
public:
    explicit BookDB(QString databasename="books.db", QObject *parent = nullptr);
    void createTables();
    std::vector<Book> books();
    int add_book(const Book &book);
    void remove_book(int id);
    void change_book(const Book &book);
    void new_values(const Book &book);
    bool book_by_isbn(QString author, QString name, QString isbn);
    std::vector<int> books_on_shelf(QString shelf);
    QStringList shelves_in_storage(QString storage);
};
#define BOOK_DB Singleton<BookDB>::instance()
#endif // BOOKDB_H
