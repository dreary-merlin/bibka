#ifndef ADDBOOK_H
#define ADDBOOK_H

#include <QWidget>
#include "book.h"
namespace Ui {
class AddBook;
}

class AddBook : public QWidget
{
    Q_OBJECT

public:
    explicit AddBook(QWidget *parent = nullptr);
    ~AddBook();
signals:
  void new_book(Book book);
public slots:
  void on_add();
private slots:
  void on_pushButton_3_clicked();

private:
    Ui::AddBook *ui;
};

#endif // ADDBOOK_H
