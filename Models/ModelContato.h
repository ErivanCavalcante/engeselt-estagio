#ifndef MODELCONTATO_H
#define MODELCONTATO_H

#include <QSqlQueryModel>
#include "Contato.h"
#include "../Orm/query.h"

using namespace orm;

class ModelContato : public QSqlQueryModel
{
    public:
    ModelContato(QObject *parent);

    /*QHash<int, QByteArray> roleNames() const override;
    QVariant data(const QModelIndex &index, int role) const override;*/

    bool adicionar(Contato &obj);
    bool atualizar(Contato &obj);
    bool remover(unsigned id);
    bool listar(QString filtroNome, QString filtroEmail, QString filtroTelefone);
};

#endif // MODELCONTATO_H
