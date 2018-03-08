#ifndef PEZZO_H
#define PEZZO_H
#include <QMetaType>
#include <QtGlobal>
#include <QDebug>
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
    Pezzo();
    Pezzo(Tipo tp, Colore cl);
    Pezzo(const Pezzo&)=default;
};
Q_DECLARE_METATYPE(Pezzo)
uint qHash(Pezzo::Tipo key, uint seed = 0);
uint qHash(Pezzo::Colore key, uint seed = 0);
QDebug operator<<(QDebug stream, const Pezzo &pezzo);
#endif // PEZZO_H
