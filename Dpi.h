#ifndef DPI_H
#define DPI_H

class Dpi
{
    public:
    Dpi();

    static double dpi;
    static double textScale;

    static double dp(double valor);
    static double sp(double valor);
};

#endif // DPI_H
