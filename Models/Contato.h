#ifndef CONTATO_H
#define CONTATO_H

#include <QString>
#include <QSqlRecord>
#include <QDate>

enum Sexo
{
    SEXO_NENHUM = 0,
    SEXO_FEMININO = 1,
    SEXO_MASCULINO = 2
};

class Contato
{
    public:
    Contato();

    unsigned m_id = 0;
    QString m_nome;
    QString m_rua;
    unsigned m_numero;
    QString m_bairro;
    QString m_cidade;
    QString m_estado;
    QString m_telefone;
    QString m_email;
    Sexo m_sexo;
    QDate m_dataNasc;

    QSqlRecord converteParaRecord(QSqlRecord base);
    void converteDeRecord(QSqlRecord &base);
};

#endif // CONTATO_H
