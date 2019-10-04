#include "Dpi.h"

double Dpi::dpi = 0.00f;
double Dpi::textScale = 1.00f;

Dpi::Dpi()
{

}

double Dpi::dp(double valor)
{
    return Dpi::dpi * valor;
}

double Dpi::sp(double valor)
{
    return Dpi::dpi * valor * Dpi::textScale;
}
