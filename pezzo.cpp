#include "pezzo.h"
#include <QDataStream>

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
QDataStream& operator<<(QDataStream& stream, const Pezzo& item){
    return stream
        << static_cast<quint8>(item.tipo)
        << static_cast<quint8>(item.colore)
        << item.primaMossa
    ;
}
QDataStream& operator>>(QDataStream& stream, Pezzo& item){
    quint8 tempInt8;
    stream >> tempInt8;
    item.tipo=static_cast<Pezzo::Tipo>(tempInt8);
    stream >> tempInt8;
    item.colore=static_cast<Pezzo::Colore>(tempInt8);
    return stream >> item.primaMossa;
}
QDataStream& operator<<(QDataStream& stream, const Mossa& item){
    return stream
        << item.muoviDa
        << item.muoviA
        << item.mangiato
        << item.mangiatoEnPassant
        << item.primaMossa
        << static_cast<quint8>(item.pedoneTrasformato)
        << item.arrocco
    ;
}
QDataStream& operator>>(QDataStream& stream, Mossa& item){
    quint8 tempInt8;
    stream
        >> item.muoviDa
        >> item.muoviA
        >> item.mangiato
        >> item.mangiatoEnPassant
        >> item.primaMossa
        >> tempInt8
    ;
    item.pedoneTrasformato = static_cast<Pezzo::Tipo>(tempInt8);
    return stream>> item.arrocco;
}
Mossa::Mossa()
    : mangiatoEnPassant(false)
    , primaMossa(false)
    , arrocco(false)
    , pedoneTrasformato(Pezzo::Tipo::Nessuno)
{}
