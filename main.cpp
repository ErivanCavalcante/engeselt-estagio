#include <QApplication>
#include <QScreen>
#include <QFile>

#include "Orm/BancoDados.h"
#include "Controllers/ControllerContato.h"
#include "Dpi.h"
#include "JanelaPrincipal.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QApplication app(argc, argv);

    //Pega a tela principal
    QScreen *tela = app.screens().at(0);

    //Calcula a dpi
    qreal ratio = app.devicePixelRatio();
    qreal virtualDpi = tela->logicalDotsPerInch();

    //Calcula o dpi
    //Dpi de referencia = 96
    Dpi::dpi = (virtualDpi / ratio) / 96;

    //Inicia o banco de dados
    BancoDados db;
    if(!db.inicarConexao())
    {
        qDebug() << "Erro ao inicar o banco de dados" << db.pegarBancoDados().lastError().databaseText();
        return -1;
    }

    //Carrega o estilo
    QFile qss(":/dark.qss");
    if(!qss.exists())
        qDebug() << "O arquivo de estilo nao existe";
    else
    {
        qss.open(QFile::ReadOnly | QFile::Text);
        QTextStream ts(&qss);
        app.setStyleSheet(ts.readAll());
    }

    //Cria a janela prinicpal e mostra ela
    JanelaPrincipal *pr = new JanelaPrincipal();
    pr->show();

    //Entra no loop ate terminar o programa
    int ret = app.exec();

    //Fim do programa
    delete pr;

    //Remove a conexao com o banco de dados
    db.removerConexao();

    return ret;
}
