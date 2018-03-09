#ifndef SCACCHIGLOBAL_H
#define SCACCHIGLOBAL_H
#include <QtGlobal>
enum {
    StatusCellRole = Qt::UserRole
    , ScaccoRole
};
enum CellStatus{
    Vuoto
    ,PezzoSelezionato
    ,CellaVuotaMuovibile
    ,CellaMangiabile
    ,CellaScacco
};
#endif // SCACCHIGLOBAL_H
