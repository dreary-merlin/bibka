#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "bookdb.h"
#include <algorithm>
#include <QFileDialog>
#include <QStandardItemModel>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setCentralWidget(ui->tabWidget);
    ui->stackedWidget->setCurrentIndex(0);
    ui->stackedWidget_2->setCurrentIndex(0);
    ui->lineEdit_7->setValidator(new QIntValidator(0, 9999999, this));
    ui->lineEdit_10->setValidator(new QIntValidator(0, 9999999, this));
    ui->lineEdit_12->setValidator(new QIntValidator(0, 9999999, this));
    set_read_main_widgets(true);
    set_read_extra_widgets(true);
    change=false;
    on_load();
    connect(ui->listWidget, SIGNAL(currentRowChanged(int)), ui->stackedWidget, SLOT(setCurrentIndex(0)));
    connect(ui->pushButton_2, SIGNAL(clicked()), SLOT(on_book_remove()));
    connect(ui->pushButton, SIGNAL(clicked()), SLOT(on_book_add()));
    add_book = new AddBook(nullptr);
    add_book->hide();
    connect(add_book, SIGNAL(new_book(Book)), SLOT(on_book_added(Book)));
    connect(ui->comboBox,SIGNAL(currentTextChanged(const QString &)),SLOT(on_comboBox_textChanged()));
    connect(ui->comboBox_2,SIGNAL(currentTextChanged(const QString &)),SLOT(on_comboBox_textChanged()));
    connect(ui->comboBox_3,SIGNAL(currentTextChanged(const QString &)),SLOT(on_comboBox_textChanged()));
    connect(ui->comboBox_4,SIGNAL(currentTextChanged(const QString &)),SLOT(on_comboBox_textChanged()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_lineEdit_textChanged(const QString &arg1)
{
    QStringList new_list;
    for (auto i = booknames.cbegin(), end = booknames.cend(); i != end; ++i)
    new_list<<i.value();
    QRegExp reg(arg1,Qt::CaseInsensitive,QRegExp::Wildcard);
    new_list=new_list.filter(reg);
    ui->listWidget->clear();
    for(int i=0;i<new_list.count();i++)
    {
        QList<int> keys=booknames.keys(new_list[i]);
        for(int key:keys)
        {
            QListWidgetItem *item = new QListWidgetItem(new_list[i],ui->listWidget);
            item->setData(Qt::UserRole, key);
            item->setIcon(QIcon(bookimages[key]));
        }
    }
}

void MainWindow::on_listWidget_currentRowChanged(int currentRow)
{
    if(currentRow>=0){
        std::vector<Book> books = BOOK_DB.books();
        int id=ui->listWidget->item(currentRow)->data(Qt::UserRole).toInt();
        auto it = std::find_if(books.begin(), books.end(), [&id](const Book& book) {
            return book.id == id;
        });
        if(it!=books.end())
        {
            ui->label_3->setPixmap(bookimages[it->id].scaled(ui->label_3->width(), ui->label_3->height(), Qt::KeepAspectRatio));
            ui->lineEdit_2->setText(it->author);
            ui->lineEdit_3->setText(it->name);
            ui->lineEdit_4->setText(it->publishing_house);
            ui->lineEdit_5->setText(num(it->year));
            ui->lineEdit_6->setText(it->isbn);
            ui->lineEdit_7->setText(num(it->pages));
            ui->lineEdit_8->setText(it->genre);
            ui->lineEdit_9->setText(it->cycle);
            ui->textEdit->setText(it->synopsis);
            ui->lineEdit_10->setText(num(it->price));
            ui->lineEdit_11->setText(it->date_of_purchase);
            ui->lineEdit_12->setText(num(it->quantity));
            ui->checkBox->setChecked(it->unhaul);
            ui->comboBox->setCurrentIndex(ui->comboBox->findText(it->type));
            ui->comboBox_2->setCurrentIndex(ui->comboBox_2->findText(it->status));
            ui->comboBox_3->setCurrentIndex(ui->comboBox_3->findText(it->storage));
            ui->comboBox_4->setCurrentIndex(ui->comboBox_4->findText(it->shelf));
            ui->textEdit_2->setText(it->extra);
        }
    }
}

void MainWindow::on_pushButton_4_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::on_pushButton_6_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_load() {
  std::vector<Book> books = BOOK_DB.books();
  QStringList l1,l2,l3,l4;
  for (auto book : books) {
      QString bookname = book.author+" - "+book.name;
      QPixmap pixmap = QPixmap();
      pixmap.loadFromData(book.image);
      QListWidgetItem *item = new QListWidgetItem(bookname,ui->listWidget);
      item->setIcon(QIcon(pixmap));
      item->setData(Qt::UserRole, book.id);
      booknames[book.id]=bookname;
      bookimages[book.id]=pixmap;
      if(!book.type.isEmpty()) l1<<book.type;
      if(!book.status.isEmpty()) l2<<book.status;
      if(!book.storage.isEmpty()) l3<<book.storage;
      if(!book.shelf.isEmpty()) l4<<book.shelf;
  }
  l1.removeDuplicates();
  ui->comboBox->addItems(l1);
  ui->comboBox->setCurrentIndex(-1);

  l2.removeDuplicates();
  ui->comboBox_2->addItems(l2);
  ui->comboBox_2->setCurrentIndex(-1);

  l3.removeDuplicates();
  ui->comboBox_3->addItems(l3);
  ui->comboBox_3->setCurrentIndex(-1);
  ui->comboBox_5->addItems(l3);
  ui->comboBox_5->setCurrentIndex(-1);

  l4.removeDuplicates();
  ui->comboBox_4->addItems(l4);
  ui->comboBox_4->setCurrentIndex(-1);
}

void MainWindow::on_book_remove() {
    std::vector<Book> books = BOOK_DB.books();
    QList<QListWidgetItem*> items = ui->listWidget->selectedItems();
    foreach(QListWidgetItem * item, items)
    {
        int id=item->data(Qt::UserRole).toInt();
        BOOK_DB.remove_book(id);
        booknames.take(id);
        bookimages.take(id);
        delete ui->listWidget->takeItem(ui->listWidget->row(item));
    }
}

void MainWindow::on_pushButton_3_clicked()
{
    set_read_main_widgets(!ui->textEdit->isReadOnly());
    auto item=ui->listWidget->currentItem();
    if(item && (change || ui->lineEdit_2->isModified() || ui->lineEdit_3->isModified() || ui->lineEdit_4->isModified() || ui->lineEdit_5->isModified() || ui->lineEdit_6->isModified() || ui->lineEdit_7->isModified() || ui->lineEdit_8->isModified() || ui->lineEdit_9->isModified() || ui->lineEdit_13->isModified() || ui->textEdit->document()->isModified()))
    {
        std::vector<Book> books = BOOK_DB.books();
        int id=item->data(Qt::UserRole).toInt();
        auto it = std::find_if(books.begin(), books.end(), [&id](const Book& book) {
            return book.id == id;
          });
        if(it!=books.end())
        {
            QString str=ui->lineEdit_13->text();
            if(!str.isEmpty())
            {
                QFile file(str);
                if(file.open(QIODevice::ReadOnly | QIODevice::Text))
                {
                    QPixmap pixmap(str);
                    QBuffer buffer(&it->image);
                    buffer.open(QIODevice::WriteOnly);
                    QFileInfo fileInfo(str);
                    if(pixmap.save(&buffer,fileInfo.suffix().toStdString().c_str()))
                    {ui->label_3->setPixmap(pixmap.scaled(ui->label_3->width(), ui->label_3->height(), Qt::KeepAspectRatio));
                    item->setIcon(QIcon(pixmap));
                    bookimages[it->id]=pixmap;
                    }
                }
            }
            it->author = ui->lineEdit_2->text();
            it->name = ui->lineEdit_3->text();
            it->publishing_house = ui->lineEdit_4->text();
            it->year = ui->lineEdit_5->text().toInt();
            it->isbn = ui->lineEdit_6->text();
            it->pages = ui->lineEdit_7->text().toInt();
            it->genre = ui->lineEdit_8->text();
            it->cycle = ui->lineEdit_9->text();
            it->synopsis = ui->textEdit->toPlainText();
            BOOK_DB.change_book(*it);
            QString bookname = it->author + " - " + it->name;
            if(bookname!=booknames[it->id])
            {
                booknames[it->id]=bookname;
                item->setText(bookname);
            }
            change=false;
        }
    }
}

void MainWindow::set_read_main_widgets(bool read)
{
    ui->lineEdit_2->setReadOnly(read);
    ui->lineEdit_3->setReadOnly(read);
    ui->lineEdit_4->setReadOnly(read);
    ui->lineEdit_5->setReadOnly(read);
    ui->lineEdit_6->setReadOnly(read);
    ui->lineEdit_7->setReadOnly(read);
    ui->lineEdit_8->setReadOnly(read);
    ui->lineEdit_9->setReadOnly(read);
    ui->textEdit->setReadOnly(read);
    ui->lineEdit_13->setVisible(!read);
    ui->pushButton_7->setVisible(!read);
}

void MainWindow::set_read_extra_widgets(bool read)
{
    ui->lineEdit_10->setReadOnly(read);
    ui->lineEdit_11->setReadOnly(read);
    ui->lineEdit_12->setReadOnly(read);
    ui->textEdit_2->setReadOnly(read);
    ui->comboBox->setEnabled(!read);
    ui->comboBox_2->setEnabled(!read);
    ui->comboBox_3->setEnabled(!read);
    ui->comboBox_4->setEnabled(!read);
    ui->checkBox->setEnabled(!read);
}

void MainWindow::on_pushButton_5_clicked()
{
    set_read_extra_widgets(!ui->textEdit_2->isReadOnly());
    auto item=ui->listWidget->currentItem();
    if(item && (change || ui->lineEdit_10->isModified() || ui->lineEdit_11->isModified() || ui->lineEdit_12->isModified() || ui->textEdit_2->document()->isModified()))
    {
        std::vector<Book> books = BOOK_DB.books();
        int id=item->data(Qt::UserRole).toInt();
        auto it = std::find_if(books.begin(), books.end(), [&id](const Book& book) {
            return book.id == id;
          });
        if(it!=books.end())
        {
            it->price = ui->lineEdit_10->text().toInt();
            it->date_of_purchase = ui->lineEdit_11->text();
            it->quantity = ui->lineEdit_12->text().toInt();
            it->unhaul=ui->checkBox->isChecked();
            it->type=ui->comboBox->currentText();
            it->status=ui->comboBox_2->currentText();
            it->storage=ui->comboBox_3->currentText();
            it->shelf=ui->comboBox_4->currentText();
            it->extra = ui->textEdit_2->toPlainText();
            BOOK_DB.change_book(*it);
            if(!it->type.isEmpty() && ui->comboBox->findText(it->type)==-1) ui->comboBox->addItem(it->type);
            if(!it->status.isEmpty() && ui->comboBox_2->findText(it->status)==-1) ui->comboBox_2->addItem(it->status);
            if(!it->storage.isEmpty() && ui->comboBox_3->findText(it->storage)==-1) ui->comboBox_3->addItem(it->storage);
            if(!it->shelf.isEmpty() && ui->comboBox_4->findText(it->shelf)==-1) ui->comboBox_4->addItem(it->shelf);
            change=false;
        }
    }
}

QString MainWindow::num(int num){
    return (num!=0)?QString::number(num):QString("");
}

void MainWindow::on_comboBox_currentIndexChanged(int index)
{
    if(index!=-1) change=true;
}

void MainWindow::on_comboBox_3_currentIndexChanged(int index)
{
    if(index!=-1) change=true;
}

void MainWindow::on_comboBox_2_currentIndexChanged(int index)
{
    if(index!=-1) change=true;
}

void MainWindow::on_comboBox_4_currentIndexChanged(int index)
{
    if(index!=-1) change=true;
}

void MainWindow::on_checkBox_stateChanged()
{
    change=true;
}

void MainWindow::on_comboBox_textChanged(){
    change=true;
}

void MainWindow::on_book_add() {
  add_book->show();
}

void MainWindow::on_book_added(Book book) {
  if (false == BOOK_DB.book_by_isbn(book.author, book.name, book.isbn))
  {
      book.id = BOOK_DB.add_book(book);
      QString bookname = book.author+" - "+book.name;
      QPixmap pixmap = QPixmap();
      pixmap.loadFromData(book.image);
      QListWidgetItem *item = new QListWidgetItem(bookname,ui->listWidget);
      item->setIcon(QIcon(pixmap));
      item->setData(Qt::UserRole, book.id);
      booknames[book.id]=bookname;
      bookimages[book.id]=pixmap;
      if(!book.type.isEmpty() && ui->comboBox->findText(book.type)==-1) ui->comboBox->addItem(book.type);
      if(!book.status.isEmpty() && ui->comboBox_2->findText(book.status)==-1) ui->comboBox_2->addItem(book.status);
      if(!book.storage.isEmpty() && ui->comboBox_3->findText(book.storage)==-1) {ui->comboBox_3->addItem(book.storage);ui->comboBox_5->addItem(book.storage);}
      if(!book.shelf.isEmpty() && ui->comboBox_4->findText(book.shelf)==-1) ui->comboBox_4->addItem(book.shelf);
  }
}

void MainWindow::on_pushButton_7_clicked()
{
    QString filename=QFileDialog::getOpenFileName(this,"Открыть файл","С://","All files (*.*)");
    ui->lineEdit_13->setText(filename);
    change=true;
}

void MainWindow::on_listView_doubleClicked(const QModelIndex &index)
{
    std::vector<int> ids=BOOK_DB.books_on_shelf(index.data(Qt::DisplayRole).toString());
    QStandardItemModel* model = new QStandardItemModel(this);
    ui->listView_2->setModel(model);
    for(size_t i=0;i<ids.size();i++) model->appendRow(new QStandardItem(QIcon(bookimages[ids[i]]), booknames[ids[i]].replace(QString(" - "),QString("\n"))));
    ui->stackedWidget_2->setCurrentIndex(1);
}

void MainWindow::on_pushButton_8_clicked()
{
    ui->stackedWidget_2->setCurrentIndex(0);
}

void MainWindow::on_comboBox_5_currentIndexChanged(const QString &arg1)
{
    QStandardItemModel* model = new QStandardItemModel(this);
    ui->listView->setModel(model);
    QStringList shelves=BOOK_DB.shelves_in_storage(arg1);
    for(int i=0;i<shelves.size();i++) model->appendRow(new QStandardItem(QIcon(":/new/prefix1/544-5448563_transparent-background-book-icon-clipart.png"), shelves[i]));
}
