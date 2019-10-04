#include "ModelContato.h"

#include <QSqlRecord>
#include <QTextStream>

ModelContato::ModelContato(QObject *parent) : QSqlQueryModel (parent)
{

}

/*QHash<int, QByteArray> ModelContato::roleNames() const
{
    //Hash para retornar
    QHash<int, QByteArray> roles;

    //Pega as colunas da tabela
    for(int i = 0; i < record().count(); i ++)
    {
       roles.insert(Qt::UserRole + i + 1, record().fieldName(i).toUtf8());
    }

    return roles;
}

QVariant ModelContato::data(const QModelIndex &index, int role) const
{
    QVariant value;

    if(index.isValid())
    {
        if (role < Qt::UserRole)
        {
            value = QSqlQueryModel::data(index, role);
        }
        else
        {
            int columnIdx = role - Qt::UserRole - 1;
            QModelIndex modelIndex = this->index(index.row(), columnIdx);
            value = QSqlQueryModel::data(modelIndex, Qt::DisplayRole);
        }
    }

    return value;
}
*/
bool ModelContato::adicionar(Contato &obj)
{
    QSqlRecord rc = obj.converteParaRecord(record());

    rc.remove(rc.indexOf("id"));

    QSqlQuery q = Query::insert("contato").values(rc).make();

    q.exec();

    if(q.lastError().isValid())
    {
        qDebug() << q.lastQuery();
        return false;
    }

    return true;
}

bool ModelContato::atualizar(Contato &obj)
{
    QSqlRecord rc = obj.converteParaRecord(record());

    QSqlQuery q = Query::update("contato").values(rc).make();

    q.exec();

    if(q.lastError().isValid())
    {
        return false;
    }

    return true;
}

bool ModelContato::remover(unsigned id)
{
    QSqlQuery q = Query::remove("contato").where("id = ?", id).make();

    q.exec();

    if(q.lastError().isValid())
    {
        return false;
    }

    return true;
}

bool ModelContato::listar(QString filtroNome, QString filtroEmail, QString filtroTelefone)
{
    QSqlQuery q;

    if(filtroNome.isEmpty() && filtroEmail.isEmpty() && filtroTelefone.isEmpty())
    {
        q = Query::select("contato").make();
    }
    else
    {
        QString sql;
        QTextStream txtstr(&sql);
        QVariantList listaArgs;

        if(!filtroNome.isEmpty())
        {
            txtstr << "nome like '%?%'";

            listaArgs << filtroNome;
        }

        if(!filtroEmail.isEmpty())
        {
            if(!sql.isEmpty())
                txtstr << " AND ";

            txtstr << "email like '%?%'";

            listaArgs << filtroEmail;
        }

        if(!filtroTelefone.isEmpty())
        {
            if(!sql.isEmpty())
                txtstr << " AND ";

            txtstr << "telefone like '%?%'";

            listaArgs << filtroTelefone;
        }

        q = Query::select("contato").where(sql, listaArgs).make();

        qDebug() << sql;
    }

    q.exec();

    setQuery(q);

    if(lastError().isValid())
    {
        return false;
    }

    return true;
}
