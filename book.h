#ifndef BOOK_H
#define BOOK_H

#include <QString>

class Book
{
public:
    int id;
    QString author;
    QString name;
    QByteArray image;
    QString isbn;
    QString publishing_house;
    int year;
    QString genre;
    int price;
    QString synopsis;
    int quantity;
    int pages;
    QString cycle;
    bool unhaul;
    QString type;
    QString status;
    QString storage;
    QString shelf;
    QString date_of_purchase;
    QString extra;
};

#endif // BOOK_H
