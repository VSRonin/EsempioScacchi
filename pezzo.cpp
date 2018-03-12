#include "pezzo.h"

Pezzo::Pezzo()
    : Pezzo(Pezzo::Tipo::Nessuno,Pezzo::Colore::Nessuno)
{}

Pezzo::Pezzo(Pezzo::Tipo tp, Pezzo::Colore cl)
    : tipo(tp)
    , colore(cl)
    , primaMossa(true)
{}

bool Pezzo::valido() const
{
    return tipo!=Pezzo::Tipo::Nessuno && colore!=Pezzo::Colore::Nessuno;
}
