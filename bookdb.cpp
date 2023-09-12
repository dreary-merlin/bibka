#include "bookdb.h"
BookDB::BookDB(QString databasename, QObject *parent): DBFacade(databasename, parent)
{

}

void BookDB::createTables()
{
    if (false == m_db.tables().contains("book")) {
      exec("CREATE TABLE book"
           "("
              "id INTEGER PRIMARY KEY AUTOINCREMENT, "
              "author TEXT NOT NULL, "
              "name TEXT NOT NULL, "
              "image BLOB, "
              "ISBN TEXT, "
              "publishing_house TEXT, "
              "year INTEGER, "
              "genre TEXT, "
              "price INTEGER, "
              "synopsis TEXT, "
              "quantity INTEGER, "
              "pages INTEGER, "
              "cycle TEXT, "
              "unhaul INTEGER,"
              "type TEXT, "
              "status TEXT, "
              "storage TEXT, "
              "shelf TEXT, "
              "date_of_purchase TEXT,"
              "extra TEXT"
           ");"
      );
    }
}

std::vector<Book> BookDB::books() {
  std::vector<Book> books;

  QString query = tr("SELECT id, author, name, image, ISBN, publishing_house, year, genre, price, synopsis, quantity, pages, cycle, unhaul, type, status, storage, shelf, date_of_purchase, extra FROM book ");
  exec(query);

  while (m_query->next()) {
    Book data;
    data.id = m_query->value(0).toInt();
    data.author = m_query->value(1).toString();
    data.name = m_query->value(2).toString();
    data.image = m_query->value(3).toByteArray();
    data.isbn = m_query->value(4).toString();
    data.publishing_house = m_query->value(5).toString();
    data.year = m_query->value(6).toInt();
    data.genre = m_query->value(7).toString();
    data.price = m_query->value(8).toInt();
    data.synopsis = m_query->value(9).toString();
    data.quantity = m_query->value(10).toInt();
    data.pages = m_query->value(11).toInt();
    data.cycle = m_query->value(12).toString();
    data.unhaul = m_query->value(13).toBool();
    data.type = m_query->value(14).toString();
    data.status = m_query->value(15).toString();
    data.storage = m_query->value(16).toString();
    data.shelf = m_query->value(17).toString();
    data.date_of_purchase = m_query->value(18).toString();
    data.extra = m_query->value(19).toString();
    books.push_back(data);
  }
  return books;
}

int BookDB::add_book(const Book &book) {
    m_query->prepare( "INSERT INTO book(author, name, image, ISBN, publishing_house, year, genre, price, synopsis, quantity, pages, cycle, unhaul, type, status, storage, shelf, date_of_purchase, extra) VALUES (:authorData, :nameData, :imageData, :ISBNData, :publishing_houseData, :yearData, :genreData, :priceData, :synopsisData, :quantityData, :pagesData, :cycleData, :unhaulData, :typeData, :statusData, :storageData, :shelfData, :date_of_purchaseData, :extraData)" );
    new_values(book);
    return m_query->lastInsertId().toInt();
}

void BookDB::remove_book(int id){
    QString query = tr("DELETE FROM book WHERE id = ") + QString::number(id);
    exec(query);
}

void BookDB::change_book(const Book &book){
    m_query->prepare( "UPDATE book SET author = :authorData, name = :nameData, image = :imageData, ISBN = :ISBNData, publishing_house = :publishing_houseData, year = :yearData, genre = :genreData, price = :priceData, synopsis = :synopsisData, quantity = :quantityData, pages = :pagesData, cycle = :cycleData, unhaul = :unhaulData, type = :typeData, status = :statusData, storage = :storageData, shelf = :shelfData, date_of_purchase = :date_of_purchaseData, extra = :extraData WHERE id = :idData");
    m_query->bindValue( ":idData", book.id );
    new_values(book);
}


void BookDB::new_values(const Book &book){
    m_query->bindValue( ":authorData", book.author );
    m_query->bindValue( ":nameData", book.name );
    m_query->bindValue( ":imageData", book.image );
    m_query->bindValue( ":ISBNData", book.isbn.isEmpty()?QVariant(QVariant::String):book.isbn );
    m_query->bindValue( ":publishing_houseData", book.publishing_house.isEmpty()?QVariant(QVariant::String):book.publishing_house );
    m_query->bindValue( ":yearData", book.year?book.year:QVariant(QVariant::Int) );
    m_query->bindValue( ":genreData", book.genre.isEmpty()?QVariant(QVariant::String):book.genre );
    m_query->bindValue( ":priceData", book.price?book.price:QVariant(QVariant::Int) );
    m_query->bindValue( ":synopsisData", book.synopsis.isEmpty()?QVariant(QVariant::String):book.synopsis );
    m_query->bindValue( ":quantityData", book.quantity?book.quantity:QVariant(QVariant::Int) );
    m_query->bindValue( ":pagesData", book.pages?book.pages:QVariant(QVariant::Int) );
    m_query->bindValue( ":cycleData", book.cycle.isEmpty()?QVariant(QVariant::String):book.cycle );
    m_query->bindValue( ":unhaulData", book.unhaul );
    m_query->bindValue( ":typeData", book.type.isEmpty()?QVariant(QVariant::String):book.type );
    m_query->bindValue( ":statusData", book.status.isEmpty()?QVariant(QVariant::String):book.status );
    m_query->bindValue( ":storageData", book.storage.isEmpty()?QVariant(QVariant::String):book.storage );
    m_query->bindValue( ":shelfData", book.shelf.isEmpty()?QVariant(QVariant::String):book.shelf );
    m_query->bindValue( ":date_of_purchaseData", book.date_of_purchase.isEmpty()?QVariant(QVariant::String):book.date_of_purchase );
    m_query->bindValue( ":extraData", book.extra.isEmpty()?QVariant(QVariant::String):book.extra );
    if( !m_query->exec() ) qDebug() << m_query->lastError();
}

bool BookDB::book_by_isbn(QString author, QString name, QString isbn) {
  QString query = tr("SELECT id FROM book WHERE author = ") + qs(author)+tr(" AND name = ")+qs(name)+tr(" AND isbn = ")+qs(isbn);
  exec(query);
  if (false == m_query->next())
    return false;
  return true;
}

std::vector<int> BookDB::books_on_shelf(QString shelf)
{
    std::vector<int> ids;

    QString query = tr("SELECT id FROM book WHERE shelf = ")+ qs(shelf);
    exec(query);

    while (m_query->next()) {
      ids.push_back(m_query->value(0).toInt());
    }
    return ids;
}

QStringList BookDB::shelves_in_storage(QString storage)
{
    QStringList shelves;

    QString query = tr("SELECT shelf FROM book WHERE storage = ")+ qs(storage);
    exec(query);

    while (m_query->next()) {
      shelves<<m_query->value(0).toString();
    }
    shelves.removeDuplicates();
    return shelves;
}
