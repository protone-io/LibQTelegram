#ifndef DATABASETABLE_H
#define DATABASETABLE_H

#include <functional>
#include <QObject>
#include <QtSql>
#include "../databasedata.h"
#include "../../../../types/telegramhelper.h"
#include "../../../../types/telegramobject.h"
#include "../../../../autogenerated/types/types.h"

class DatabaseTable : public QObject
{
    Q_OBJECT

    public:
        explicit DatabaseTable(const QString& name, QObject *parent = 0);
        const QString& name() const;
        const QString& defaultField() const;
        virtual void createSchema() = 0;

    public:
        template<typename T, typename U> T* get(U id, const char* type, bool ignoreerror, QObject* parent) const;
        void prepareInsert(QSqlQuery& insertquery);
        virtual void insertQuery(QSqlQuery& queryobj, TelegramObject* telegramobject) = 0;
        void insert(TelegramObject* telegramobject);
        void remove(TLInt id);

    private:
        void parseFields(const QString& fields);

    protected:
        void createTable(const QString &fields, const QString& defaultfield);
        bool query(QSqlQuery& queryobj, const QString& query) const;
        bool query(const QString& query);
        bool prepare(QSqlQuery &queryobj, const QString& query) const;
        bool execute(QSqlQuery &queryobj) const;

    private:
        QString _name;
        QString _defaultfield;
        QStringList _fields;
        QStringList _values;
};

template<typename T1, typename T2> T1* DatabaseTable::get(T2 id, const char* type, bool ignoreerror, QObject* parent) const
{
    CreateQuery(queryobj);

    this->prepare(queryobj, "SELECT * FROM " + this->name() + " WHERE id=:id");
    queryobj.bindValue(":id", id);
    this->execute(queryobj);

    if(!queryobj.first())
    {
        if(!ignoreerror)
            qWarning("Cannot get %s with id %" PRIdPTR, type, id);

        return NULL;
    }

    QByteArray data = queryobj.value(this->defaultField()).toByteArray();

    T1* t = new T1(parent);
    t->unserialize(data);
    return t;
}

#endif // DATABASETABLE_H
