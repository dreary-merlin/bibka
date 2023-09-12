#include "addbook.h"
#include "ui_addbook.h"
#include <QFile>
#include <QMessageBox>
#include <QBuffer>
#include <QFileInfo>
#include <QFileDialog>
AddBook::AddBook(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AddBook)
{
    ui->setupUi(this);
    ui->lineEdit_5->setValidator(new QIntValidator(0, 9999999, this));
    ui->lineEdit_6->setValidator(new QIntValidator(0, 9999999, this));
    ui->lineEdit_11->setValidator(new QIntValidator(0, 9999999, this));
    connect(ui->pushButton, SIGNAL(clicked()), SLOT(on_add()));
    connect(ui->pushButton_2, SIGNAL(clicked()), SLOT(hide()));
}

AddBook::~AddBook()
{
    delete ui;
}

void AddBook::on_add() {
  if (ui->lineEdit->text().isEmpty() || ui->lineEdit_9->text().isEmpty()) return;
  QString str=ui->lineEdit_16->text();
  Book book;
  if(!str.isEmpty())
  {
      QFile file(str);
      if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
      {
          QMessageBox::information(this,"Error", "Некорректный путь к файлу");
          return;
      }
      QPixmap pixmap(str);
      QBuffer buffer(&book.image);
      buffer.open(QIODevice::WriteOnly);
      QFileInfo fileInfo(str);
      pixmap.save(&buffer,fileInfo.suffix().toStdString().c_str());
  }
  book.author = ui->lineEdit->text();
  book.name = ui->lineEdit_9->text();
  book.publishing_house = ui->lineEdit_2->text();
  book.year = ui->lineEdit_10->text().toInt();
  book.isbn = ui->lineEdit_3->text();
  book.pages = ui->lineEdit_11->text().toInt();
  book.genre = ui->lineEdit_4->text();
  book.cycle = ui->lineEdit_12->text();
  book.synopsis = ui->textEdit->toPlainText();
  book.price = ui->lineEdit_5->text().toInt();
  book.date_of_purchase = ui->lineEdit_13->text();
  book.quantity = ui->lineEdit_6->text().toInt();
  book.unhaul=ui->checkBox->isChecked();
  book.type=ui->lineEdit_7->text();
  book.status=ui->lineEdit_14->text();
  book.storage=ui->lineEdit_8->text();
  book.shelf=ui->lineEdit_15->text();
  book.extra = ui->textEdit_2->toPlainText();
  emit new_book(book);
  this->close();
}

void AddBook::on_pushButton_3_clicked()
{
    QString filename=QFileDialog::getOpenFileName(this,"Открыть файл","С://","All files (*.*)");
    ui->lineEdit_16->setText(filename);
}
