#include "ControllerContato.h"

ControllerContato::ControllerContato(QObject *parent)
    : QObject (parent),
      m_mdlContato(parent)
{

}

ModelContato *ControllerContato::pegarModelo()
{
    return &m_mdlContato;
}

QString ControllerContato::pegarUltimoErro()
{
    return m_erro;
}

bool ControllerContato::adicionarContato(Contato &c)
{
    if(!m_mdlContato.adicionar(c))
    {
        m_erro = m_mdlContato.lastError().databaseText();
        return false;
    }

    return true;
}

bool ControllerContato::removerContato(Contato &c)
{
    if(c.m_id == 0)
    {
        m_erro = "O id é invalido";
        return false;
    }

    if(!m_mdlContato.remover(c.m_id))
    {
        m_erro = m_mdlContato.lastError().databaseText();
        return false;
    }

    return true;
}

bool ControllerContato::atualizarContato(Contato &c)
{
    if(c.m_id == 0)
    {
        m_erro = "O id é invalido";
        return false;
    }

    if(!m_mdlContato.atualizar(c))
    {
        m_erro = m_mdlContato.lastError().databaseText();
        return false;
    }

    return true;
}

bool ControllerContato::listarContatos()
{
    if(!m_mdlContato.listar("", "", ""))
    {
        m_erro = m_mdlContato.lastError().databaseText();
        return false;
    }

    return true;
}

bool ControllerContato::listarContatos(QString filtroNome, QString filtroEmail, QString filtroTelefone)
{
    if(!m_mdlContato.listar(filtroNome, filtroEmail, filtroTelefone))
    {
        m_erro = m_mdlContato.lastError().databaseText();
        return false;
    }

    return true;
}
