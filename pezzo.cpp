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
    , primaMossa(true)
{}

Pezzo::Pezzo(Pezzo::Tipo tp, Pezzo::Colore cl)
    : tipo(tp)
    , colore(cl)
    , primaMossa(true)
{}

QDebug operator<<(QDebug stream, const Pezzo &pezzo)
{
    if(pezzo.colore==Pezzo::Colore::Nessuno)
        return stream << "Invalido";
    switch (pezzo.tipo) {
    case Pezzo::Tipo::Pedone:
        return stream << (pezzo.colore == Pezzo::Colore::Bianco ? "Pedone Bianco" : "Pedone Nero");
    case Pezzo::Tipo::Torre:
       return stream << (pezzo.colore == Pezzo::Colore::Bianco ? "Torre Bianca" : "Torre Nera");
    case Pezzo::Tipo::Cavallo:
        return stream << (pezzo.colore == Pezzo::Colore::Bianco ? "Cavallo Bianco" : "Cavallo Nero");
    case Pezzo::Tipo::Alfiere:
        return stream << (pezzo.colore == Pezzo::Colore::Bianco ? "Alfiere Bianco" : "Alfiere Nero");
    case Pezzo::Tipo::Regina:
        return stream << (pezzo.colore == Pezzo::Colore::Bianco ? "Regina Bianca" : "Regina Nera");
    case Pezzo::Tipo::Re:
         return stream << (pezzo.colore == Pezzo::Colore::Bianco ? "Re Bianco" : "Re Nero");
    default:
        return stream << "Invalido";
    }
}
