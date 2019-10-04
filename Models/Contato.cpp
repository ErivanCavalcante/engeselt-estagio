#include "Contato.h"
#include <QVariant>
#include <QSqlField>

Contato::Contato()
{

}

QSqlRecord Contato::converteParaRecord(QSqlRecord base)
{
    if(!base.contains("id"))
        base.insert(0, QSqlField("id", QVariant::Int));
    if(!base.contains("nome"))
        base.insert(1, QSqlField("nome", QVariant::String));
    if(!base.contains("rua"))
        base.insert(1, QSqlField("rua", QVariant::String));
    if(!base.contains("numero"))
        base.insert(2, QSqlField("numero", QVariant::Int));
    if(!base.contains("bairro"))
        base.insert(3, QSqlField("bairro", QVariant::String));
    if(!base.contains("cidade"))
        base.insert(4, QSqlField("cidade", QVariant::String));
    if(!base.contains("estado"))
        base.insert(5, QSqlField("estado", QVariant::String));
    if(!base.contains("telefone"))
        base.insert(6, QSqlField("telefone", QVariant::String));
    if(!base.contains("email"))
        base.insert(7, QSqlField("email", QVariant::String));
    if(!base.contains("sexo"))
        base.insert(8, QSqlField("sexo", QVariant::Int));
    if(!base.contains("data_nascimento"))
        base.insert(9, QSqlField("data_nascimento", QVariant::Date));

    base.setValue("id", m_id);
    base.setValue("nome", m_nome);
    base.setValue("rua", m_rua);
    base.setValue("numero", m_numero);
    base.setValue("bairro", m_bairro);
    base.setValue("cidade", m_cidade);
    base.setValue("estado", m_estado);
    base.setValue("telefone", m_telefone);
    base.setValue("email", m_email);
    base.setValue("sexo", m_sexo);
    base.setValue("data_nascimento", m_dataNasc);

    return base;
}

void Contato::converteDeRecord(QSqlRecord &base)
{
    m_id = base.value("id").toUInt();
    m_nome = base.value("nome").toString();
    m_rua = base.value("rua").toString();
    m_numero = base.value("numero").toUInt();
    m_bairro = base.value("bairro").toString();
    m_cidade = base.value("cidade").toString();
    m_estado = base.value("estado").toString();
    m_telefone = base.value("telefone").toString();
    m_email = base.value("email").toString();
    m_sexo = (Sexo)base.value("sexo").toUInt();
    m_dataNasc = base.value("data_nascimento").toDate();
}
