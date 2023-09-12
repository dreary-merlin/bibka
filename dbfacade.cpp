#include "dbfacade.h"

DBFacade::DBFacade(QString databasename, QObject *parent) : QObject(parent)
{
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName("C:\\bibka\\"+databasename);
    if (false == m_db.open())
        throw "can't open/create DB";

    m_query = new QSqlQuery(m_db);
}

DBFacade::~DBFacade() {
    delete m_query;
}

QString DBFacade::qs(QString str) {
    return "'" + str + "'";
}

QString DBFacade::qs(std::string str) {
    return qs(QString::fromStdString(str));
}

void DBFacade::exec(QString str) {
    if (false == m_query->exec(str))
        throw tr("DBFacade Error: can't exec : ") + str;
}
