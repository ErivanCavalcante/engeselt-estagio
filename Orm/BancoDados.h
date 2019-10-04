#ifndef BANCODADOS_H
#define BANCODADOS_H

#include <QSqlDatabase>
#include <QSqlError>
#include <QDebug>

class BancoDados
{
    public:
    BancoDados();

    bool inicarConexao();
    void removerConexao();

    QSqlDatabase pegarBancoDados() const {return db;}

    private:
    QSqlDatabase db;
};

#endif // BANCODADOS_H
