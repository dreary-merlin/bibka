#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSplitter>
#include <QRegExp>
#include <QMap>
#include "addbook.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_lineEdit_textChanged(const QString &arg1);
    void on_listWidget_currentRowChanged(int currentRow);
    void on_pushButton_4_clicked();
    void on_pushButton_6_clicked();
    void on_load();
    void on_book_remove();
    void on_book_add();
    void on_book_added(Book book);
    void on_pushButton_3_clicked();
    void set_read_main_widgets(bool read);
    void set_read_extra_widgets(bool read);
    void on_pushButton_5_clicked();
    QString num(int num);
    void on_comboBox_currentIndexChanged(int index);
    void on_comboBox_3_currentIndexChanged(int index);
    void on_comboBox_2_currentIndexChanged(int index);
    void on_comboBox_4_currentIndexChanged(int index);
    void on_checkBox_stateChanged();
    void on_comboBox_textChanged();
    void on_pushButton_7_clicked();
    void on_listView_doubleClicked(const QModelIndex &index);
    void on_pushButton_8_clicked();

    void on_comboBox_5_currentIndexChanged(const QString &arg1);

private:
    Ui::MainWindow *ui;
    QMap<int,QString> booknames;
    QMap<int,QPixmap> bookimages;
    bool change;
    AddBook* add_book;
    QMap<QString,QStringList> bookshelves;
};

#endif // MAINWINDOW_H
