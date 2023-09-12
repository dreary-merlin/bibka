#ifndef DBFACADE_H
#define DBFACADE_H

#include <QObject>
#include <QtSql/QtSql>

class DBFacade : public QObject
{
    Q_OBJECT
public:
    explicit DBFacade(QString databasename, QObject *parent = nullptr);
    ~DBFacade();
signals:
protected:
    void exec(QString);
    QString qs(QString);
    QString qs(std::string);
    QSqlDatabase m_db;
    QSqlQuery *m_query;
public slots:
};

#endif // DBFACADE_H
