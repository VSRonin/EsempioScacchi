#include "pezzo.h"
#include <type_traits>
#include <QHash>
uint qHash(Pezzo::Tipo key, uint seed){
    return qHash(static_cast<std::underlying_type<Pezzo::Tipo>::type>(key),seed);
}
uint qHash(Pezzo::Colore key, uint seed){
    return qHash(static_cast<std::underlying_type<Pezzo::Colore>::type>(key),seed);
}
Pezzo::Pezzo()
    : tipo(Pezzo::Tipo::Nessuno)
    , colore(Pezzo::Colore::Nessuno)
{}

Pezzo::Pezzo(Pezzo::Tipo tp, Pezzo::Colore cl)
    : tipo(tp)
    , colore(cl)
{}

QDebug operator<<(QDebug stream, const Pezzo &pezzo)
{
    if(pezzo.colore==Pezzo::Colore::Nessuno)
        return stream << "Invalido";
    switch (pezzo.tipo) {
    case Pezzo::Tipo::Pedone:
        return stream << (pezzo.colore == Pezzo::Colore::Bianco ? QChar(0x2659) : QChar(0x265F));
    case Pezzo::Tipo::Torre:
       return stream << (pezzo.colore == Pezzo::Colore::Bianco ? QChar(0x2656) : QChar(0x265C));
    case Pezzo::Tipo::Cavallo:
        return stream << (pezzo.colore == Pezzo::Colore::Bianco ? QChar(0x2658) : QChar(0x265E));
    case Pezzo::Tipo::Alfiere:
        return stream << (pezzo.colore == Pezzo::Colore::Bianco ? QChar(0x2657) : QChar(0x265D));
    case Pezzo::Tipo::Regina:
        return stream << (pezzo.colore == Pezzo::Colore::Bianco ? QChar(0x2655) : QChar(0x265B));
    case Pezzo::Tipo::Re:
         return stream << (pezzo.colore == Pezzo::Colore::Bianco ? QChar(0x2654) : QChar(0x265A));
    default:
        return stream << "Invalido";
    }
}
