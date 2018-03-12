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
    bool primaMossa;
    bool doppioPasso;
    Pezzo();
    Pezzo(Tipo tp, Colore cl);
    Pezzo(const Pezzo&)=default;
    bool valido() const;
};
Q_DECLARE_METATYPE(Pezzo)
#endif // PEZZO_H
