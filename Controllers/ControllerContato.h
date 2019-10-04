#ifndef CONTROLLERCONTATO_H
#define CONTROLLERCONTATO_H

#include <QObject>
#include "../Models/ModelContato.h"

class ControllerContato : public QObject
{
    Q_OBJECT

    public:
    ControllerContato(QObject *parent);

    ModelContato* pegarModelo();

    bool adicionarContato(Contato &c);
    bool removerContato(Contato &c);
    bool atualizarContato(Contato &c);
    bool listarContatos();
    bool listarContatos(QString filtroNome, QString filtroEmail, QString filtroTelefone);

    QString pegarUltimoErro();

    private:
    ModelContato m_mdlContato;

    QString m_erro;
};

#endif // CONTROLLERCONTATO_H
