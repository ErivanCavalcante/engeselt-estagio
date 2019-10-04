#ifndef DIALOGOSOBRE_H
#define DIALOGOSOBRE_H

#include <QDialog>

namespace Ui {
    class DialogoSobre;
}

class DialogoSobre : public QDialog
{
    Q_OBJECT

    public:
    explicit DialogoSobre(QWidget *parent = nullptr);
    ~DialogoSobre();

    private:
    Ui::DialogoSobre *ui;
};

#endif // DIALOGOSOBRE_H
