#ifndef PEZZO_H
#define PEZZO_H
#include <QMetaType>
#include <QtGlobal>
#include <QPoint>
class QDataStream;
class Pezzo{
public:
    enum class Tipo : quint8{
        Nessuno
        ,Pedone
        ,Torre
        ,Cavallo
        ,Alfiere
        ,Regina
        ,Re
    };
    enum class Colore : quint8{
        Nessuno
        ,Nero
        ,Bianco
    };
    Tipo tipo;
    Colore colore;
    bool primaMossa;
    Pezzo();
    Pezzo(Tipo tp, Colore cl);
    Pezzo(const Pezzo& other)=default;
    Pezzo& operator=(const Pezzo& other)=default;
    bool valido() const;
};
class Mossa{
public:
    Mossa(const Mossa& other) = default;
    Mossa& operator=(const Mossa& other) = default;
    Mossa();
    QPoint muoviDa;
    QPoint muoviA;
    Pezzo mangiato;
    bool mangiatoEnPassant;
    bool primaMossa;
    Pezzo::Tipo pedoneTrasformato;
    bool arrocco;
};
QDataStream& operator<<(QDataStream& stream, const Pezzo& item);
QDataStream& operator>>(QDataStream& stream, Pezzo& item);
QDataStream& operator<<(QDataStream& stream, const Mossa& item);
QDataStream& operator>>(QDataStream& stream, Mossa& item);
Q_DECLARE_METATYPE(Pezzo)
Q_DECLARE_METATYPE(Mossa)
#endif // PEZZO_H
