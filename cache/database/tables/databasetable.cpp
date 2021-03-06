#include "databasetable.h"

DatabaseTable::DatabaseTable(const QString &name, QObject *parent) : QObject(parent), _name(name)
{

}

const QString &DatabaseTable::name() const
{
    return this->_name;
}

const QString &DatabaseTable::defaultField() const
{
    return this->_defaultfield;
}

void DatabaseTable::insert(TelegramObject *telegramobject)
{
    CreateQuery(queryobj);
    this->prepareInsert(queryobj);
    this->insertQuery(queryobj, telegramobject);
}

void DatabaseTable::parseFields(const QString& fields)
{
    QStringList fieldlist = fields.split(",", QString::SkipEmptyParts);

    foreach(QString field, fieldlist)
    {
        QStringList fieldelements = field.split(" ", QString::SkipEmptyParts);
        this->_fields << fieldelements.first();
        this->_values << ":" + fieldelements.first();
    }
}

bool DatabaseTable::prepare(QSqlQuery &queryobj, const QString &query) const
{
    if(!queryobj.prepare(query))
    {
        qWarning() << Q_FUNC_INFO << "failed to prepare query";
        qWarning() << queryobj.lastError();
        qWarning() << queryobj.lastQuery();

        return false;
    }

    return true;
}

bool DatabaseTable::execute(QSqlQuery &queryobj) const
{
    if(!queryobj.exec())
    {
        qWarning() << Q_FUNC_INFO << "failed to execute query";
        qWarning() << queryobj.lastError();
        qWarning() << queryobj.lastQuery();

        return false;
    }

    return true;
}

void DatabaseTable::createTable(const QString& fields, const QString& defaultfield, const QString &extradata)
{
    this->_defaultfield = defaultfield;
    this->parseFields(fields);

    QString params = extradata.isEmpty() ? QString() : (", " + extradata);
    this->query("CREATE TABLE IF NOT EXISTS " + this->_name + "(" + fields + params + ")");
}

void DatabaseTable::prepareInsert(QSqlQuery &insertquery)
{
    this->prepare(insertquery, "INSERT OR REPLACE INTO " + this->_name +
                               " (" + this->_fields.join(",") + ") " +
                               "VALUES (" + this->_values.join(",") + ")");
}

bool DatabaseTable::query(QSqlQuery &queryobj, const QString &query) const
{
    if(!this->prepare(queryobj, query))
        return false;

    return this->execute(queryobj);
}

bool DatabaseTable::query(const QString &query)
{
    CreateQuery(queryobj);
    return this->query(queryobj, query);
}
