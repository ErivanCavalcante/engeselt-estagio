#ifndef DIALOGOFILTROS_H
#define DIALOGOFILTROS_H

#include <QDialog>

namespace Ui {
    class DialogoFiltros;
}

class DialogoFiltros : public QDialog
{
    Q_OBJECT

    public:
    explicit DialogoFiltros(QWidget *parent = nullptr);
    ~DialogoFiltros();

    signals:
    void filtrar(QString nome, QString tel, QString email);

    private slots:
    void on_cbNome_stateChanged(int arg);

    void on_cbTel_stateChanged(int arg);

    void on_cbEmail_stateChanged(int arg);

    void on_btOk_clicked();

    private:
    Ui::DialogoFiltros *ui;
};

#endif // DIALOGOFILTROS_H
