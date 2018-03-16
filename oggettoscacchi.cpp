#include "oggettoscacchi.h"
#include "pezzo.h"
#include "scacchiglobal.h"
#include "scacchimodel.h"
QAbstractItemModel *OggettoScacchi::model() const {return m_model;}
const QStack<Mossa> &OggettoScacchi::registroMosse() const {return m_registroMosse;}
OggettoScacchi::OggettoScacchi(QObject *parent)
    : QObject(parent)
    , m_turnoDi(Pezzo::Colore::Bianco)
    , m_pezzoCorrente(-1,-1)
    , m_pedoneDaTrasformare(-1,-1)
{
    m_model= new ScacchiModel(this);
}
void OggettoScacchi::startGame(){
    m_turnoDi = Pezzo::Colore::Bianco;
    rimuoviTuttiStati(Pezzo::Colore::Nessuno,true);
    for(int i=0;i<8;++i){
        for(int j=0;j<8;++j){
            m_model->setData(m_model->index(i,j),QVariant());
        }
    }
    Pezzo tempPezzo(Pezzo::Tipo::Pedone,Pezzo::Colore::Nero);
    for(int i=0;i<8;++i){
        m_model->setData(m_model->index(1,i),QVariant::fromValue(tempPezzo));
    }
    tempPezzo.colore = Pezzo::Colore::Bianco;
    for(int i=0;i<8;++i){
        m_model->setData(m_model->index(6,i),QVariant::fromValue(tempPezzo));
    }
    tempPezzo.tipo=Pezzo::Tipo::Torre;
    m_model->setData(m_model->index(7,0),QVariant::fromValue(tempPezzo));
    m_model->setData(m_model->index(7,7),QVariant::fromValue(tempPezzo));
    tempPezzo.colore = Pezzo::Colore::Nero;
    m_model->setData(m_model->index(0,0),QVariant::fromValue(tempPezzo));
    m_model->setData(m_model->index(0,7),QVariant::fromValue(tempPezzo));
    tempPezzo.colore = Pezzo::Colore::Bianco;
    tempPezzo.tipo=Pezzo::Tipo::Cavallo;
    m_model->setData(m_model->index(7,1),QVariant::fromValue(tempPezzo));
    m_model->setData(m_model->index(7,6),QVariant::fromValue(tempPezzo));
    tempPezzo.colore = Pezzo::Colore::Nero;
    m_model->setData(m_model->index(0,1),QVariant::fromValue(tempPezzo));
    m_model->setData(m_model->index(0,6),QVariant::fromValue(tempPezzo));
    tempPezzo.colore = Pezzo::Colore::Bianco;
    tempPezzo.tipo=Pezzo::Tipo::Alfiere;
    m_model->setData(m_model->index(7,2),QVariant::fromValue(tempPezzo));
    m_model->setData(m_model->index(7,5),QVariant::fromValue(tempPezzo));
    tempPezzo.colore = Pezzo::Colore::Nero;
    m_model->setData(m_model->index(0,2),QVariant::fromValue(tempPezzo));
    m_model->setData(m_model->index(0,5),QVariant::fromValue(tempPezzo));
    tempPezzo.colore = Pezzo::Colore::Bianco;
    tempPezzo.tipo=Pezzo::Tipo::Re;
    m_model->setData(m_model->index(7,4),QVariant::fromValue(tempPezzo));
    tempPezzo.colore = Pezzo::Colore::Nero;
    m_model->setData(m_model->index(0,4),QVariant::fromValue(tempPezzo));
    tempPezzo.colore = Pezzo::Colore::Bianco;
    tempPezzo.tipo=Pezzo::Tipo::Regina;
    m_model->setData(m_model->index(7,3),QVariant::fromValue(tempPezzo));
    tempPezzo.colore = Pezzo::Colore::Nero;
    m_model->setData(m_model->index(0,3),QVariant::fromValue(tempPezzo));
    partitaIniziata();
    turnoDi(m_turnoDi);
}

bool OggettoScacchi::muovi(const QModelIndex &fromIdx, const QModelIndex &toIdx){
    if(usaPezzo(fromIdx))
        return muovi(toIdx);
    return false;
}

bool OggettoScacchi::muovi(const QPoint &fromPt, const QPoint &toPt){
    if(usaPezzo(fromPt))
        return muovi(toPt);
    return false;
}
bool OggettoScacchi::muovi(const QPoint &fromPt, const QPoint &toPt, Pezzo::Tipo typPedone, bool clearRedo)
{
    if(usaPezzo(fromPt))
        return muovi(indexForPoint(toPt),typPedone,clearRedo);
    return false;
}

bool OggettoScacchi::usaPezzo(const QPoint &fromPt){return usaPezzo(indexForPoint(fromPt));}

bool OggettoScacchi::muovi(const QPoint &toPt){return muovi(indexForPoint(toPt));}

bool OggettoScacchi::usaPezzo(const QModelIndex& fromIdx)
{
    if(!fromIdx.isValid())
        return false;
    if(m_pedoneDaTrasformare.y()>=0)
        return false;
    Q_ASSERT(fromIdx.model()==m_model);
    if(m_pezzoCorrente.y()<0){ //nessun pezzo selezionato
        const QVariant tempData = fromIdx.data();
        if(tempData.isValid()){ //la cella contiene un pezzo
            Pezzo tempPezzo = tempData.value<Pezzo>();
            if(tempPezzo.colore==m_turnoDi){ //il pezzo selezionato e' del giocatore che ha il turno
                m_pezzoCorrente.setY(fromIdx.row());
                m_pezzoCorrente.setX(fromIdx.column());
                m_model->setData(fromIdx,PezzoSelezionato,StatusCellRole);
                formattaMossePossibili();
                return true;
            }
        }
    }
    else if(m_pezzoCorrente.y() == fromIdx.row() && m_pezzoCorrente.x() == fromIdx.column()){ //deseleziona pezzo corrente
        m_pezzoCorrente.setX(-1);
        m_pezzoCorrente.setY(-1);
        rimuoviTuttiStati(false);
        return true;
    }
    return false;
}

bool OggettoScacchi::muovi(const QModelIndex &toIdx){return muovi(toIdx,Pezzo::Tipo::Nessuno);}

bool OggettoScacchi::muovi(const QModelIndex &toIdx, Pezzo::Tipo typPedone, bool clearRedo)
{
    if(!toIdx.isValid())
        return false;
    if(m_pedoneDaTrasformare.y()>=0 && (typPedone==Pezzo::Tipo::Nessuno || typPedone==Pezzo::Tipo::Re ||typPedone==Pezzo::Tipo::Pedone))
        return false;
    if(m_pedoneDaTrasformare.y()>=0 && toIdx!=indexForPoint(m_pedoneDaTrasformare))
        return false;
    Q_ASSERT(toIdx.model()==m_model);
    if(m_pezzoCorrente.y()<0) //nessun pezzo selezionato
        return false;
    if(m_pezzoCorrente.y() == toIdx.row() && m_pezzoCorrente.x() == toIdx.column()) // da e a sono la stessa cosa
        return false;
    switch(toIdx.data(StatusCellRole).toInt()){
    case CellaMangiabile:
    case CellaVuotaMuovibile:{
        Mossa tempMossa;
        Pezzo tempPezzo = m_model->index(m_pezzoCorrente.y(),m_pezzoCorrente.x()).data().value<Pezzo>();
        tempMossa.primaMossa = tempPezzo.primaMossa;
        tempPezzo.primaMossa = false;
        if(tempPezzo.tipo == Pezzo::Tipo::Re && qAbs(m_pezzoCorrente.x()-toIdx.column())>1){
            //Arrocco
            const QModelIndex idxTorre = m_model->index(m_pezzoCorrente.y(),toIdx.column()>m_pezzoCorrente.x() ? 7:0);
            Pezzo tempTorre = idxTorre.data().value<Pezzo>();
            Q_ASSERT(tempTorre.tipo == Pezzo::Tipo::Torre && tempTorre.colore == tempPezzo.colore);
            tempTorre.primaMossa = false;
            tempMossa.arrocco = true;
            m_model->setData(m_model->index(m_pezzoCorrente.y(),toIdx.column()>m_pezzoCorrente.x() ? 5:2),QVariant::fromValue(tempTorre));
            m_model->setData(idxTorre,QVariant());
        }
        else if(tempPezzo.tipo == Pezzo::Tipo::Pedone){
            if(m_pedoneDaTrasformare.y()>=0){
                tempPezzo.tipo = typPedone;
                tempMossa.pedoneTrasformato=typPedone;
            }
            else if((toIdx.row()==0 || toIdx.row()==7)){
                // pedone raggiunge la fine
                m_pedoneDaTrasformare = pointForIndex(toIdx);
                if(typPedone!=Pezzo::Tipo::Nessuno && typPedone!=Pezzo::Tipo::Re && typPedone!=Pezzo::Tipo::Pedone){
                    tempPezzo.tipo = typPedone;
                    tempMossa.pedoneTrasformato=typPedone;
                }
                else{
                    richiediTrasformazionePedone();
                    return false;
                }
            }
            m_pedoneDaTrasformare = QPoint(-1,-1);
            if(!toIdx.data().isValid() && m_pezzoCorrente.x()!=toIdx.column()){
                // un pedone si muove in diagonale su una cella vuota solo se mangia "en passant"
                const QModelIndex enPassantIdx = m_model->index(m_pezzoCorrente.y(),toIdx.column());
                Q_ASSERT(enPassantIdx.data(DoppioPassoRole).toBool());
                Q_ASSERT(enPassantIdx.data().isValid());
                const Pezzo pezzoMangiato =enPassantIdx.data().value<Pezzo>();
                tempMossa.mangiato= pezzoMangiato;
                tempMossa.mangiatoEnPassant = true;
                mangiato(pezzoMangiato);
                m_model->setData(enPassantIdx,QVariant());
            }
            if(qAbs(m_pezzoCorrente.y()-toIdx.row())==2)
                m_model->setData(toIdx,true,DoppioPassoRole);
        }
        if(toIdx.data().isValid()){
            const Pezzo pezzoMangiato =toIdx.data().value<Pezzo>();
            tempMossa.mangiato= pezzoMangiato;
            mangiato(pezzoMangiato);
        }
        tempMossa.muoviDa = m_pezzoCorrente;
        tempMossa.muoviA = pointForIndex(toIdx);
        if(clearRedo){
            m_registroMosse.push(tempMossa);
            if(m_registroMosse.size()==1)
                undoEnabled(true);
            if(!m_redoStack.isEmpty()){
                m_redoStack.clear();
                redoEnabled(false);
            }
        }
        m_model->setData(toIdx,QVariant::fromValue(tempPezzo));
        m_model->setData(indexForPoint(m_pezzoCorrente),QVariant());
        cambiaTurno();
        return true;
    }
    default: break;
    }
    return  false;
}



bool OggettoScacchi::trasformaPedone(Pezzo::Tipo typ)
{
    if(m_pedoneDaTrasformare.y()<0)
        return false;
    return muovi(indexForPoint(m_pedoneDaTrasformare),typ);
}

bool OggettoScacchi::undoMossa()
{
    if(m_registroMosse.isEmpty())
        return false;
    if(m_pezzoCorrente.y()>=0)
        usaPezzo(m_pezzoCorrente);
    const Mossa tempMossa = m_registroMosse.pop();
    Pezzo pezzoMosso = indexForPoint(tempMossa.muoviA).data().value<Pezzo>();
    pezzoMosso.primaMossa=tempMossa.primaMossa;
    if(tempMossa.pedoneTrasformato != Pezzo::Tipo::Nessuno){
        Q_ASSERT(pezzoMosso.tipo==Pezzo::Tipo::Regina || pezzoMosso.tipo==Pezzo::Tipo::Torre || pezzoMosso.tipo==Pezzo::Tipo::Alfiere || pezzoMosso.tipo==Pezzo::Tipo::Cavallo);
        pezzoMosso.tipo=Pezzo::Tipo::Pedone;
    }
    m_model->setData(indexForPoint(tempMossa.muoviDa),QVariant::fromValue(pezzoMosso));
    m_model->setData(indexForPoint(tempMossa.muoviA),QVariant());
    if(tempMossa.arrocco){
        Q_ASSERT(!tempMossa.mangiato.valido());
        Q_ASSERT(tempMossa.muoviDa.y()==tempMossa.muoviA.y());
        const QModelIndex torreIdx = indexForPoint(QPoint(tempMossa.muoviDa.x()>tempMossa.muoviA.x() ? 2:5,tempMossa.muoviA.y()));
        Pezzo tempTorre = torreIdx.data().value<Pezzo>();
        Q_ASSERT(tempTorre.tipo==Pezzo::Tipo::Torre);
        tempTorre.primaMossa = true;
        m_model->setData(
            indexForPoint(QPoint(tempMossa.muoviDa.x()>tempMossa.muoviA.x() ? 0:7,tempMossa.muoviA.y()))
            , QVariant::fromValue(tempTorre)
        );
        m_model->setData(torreIdx,QVariant());
    }
    else if(tempMossa.mangiatoEnPassant){
        Q_ASSERT(tempMossa.mangiato.valido());
        m_model->setData(m_model->index(tempMossa.muoviDa.y(),tempMossa.muoviA.x()),QVariant::fromValue(tempMossa.mangiato));
    }
    else if(tempMossa.mangiato.valido()){
        m_model->setData(indexForPoint(tempMossa.muoviA),QVariant::fromValue(tempMossa.mangiato));
    }
    m_redoStack.push(tempMossa);
    if(m_redoStack.size()==1)
        redoEnabled(true);
    if(m_registroMosse.isEmpty())
        undoEnabled(false);
    cambiaTurno();
    return true;
}
bool OggettoScacchi::muovi(const Mossa &mossa)
{
    return muovi(mossa,false);
}

bool OggettoScacchi::muovi(const Mossa &mossa, bool salvaRedo)
{
    return muovi(mossa.muoviDa,mossa.muoviA,mossa.pedoneTrasformato,!salvaRedo);
}
bool OggettoScacchi::redoMossa()
{
    if(m_redoStack.isEmpty())
        return false;
    if(m_pezzoCorrente.y()>=0)
        usaPezzo(m_pezzoCorrente);
    const Mossa tempMossa = m_redoStack.pop();
    Q_ASSUME(muovi(tempMossa,true));
    m_registroMosse.push(tempMossa);
    if(m_registroMosse.size()==1)
        undoEnabled(true);
    if(m_redoStack.isEmpty())
        redoEnabled(false);
    return true;
}



void OggettoScacchi::formattaMossePossibili()
{
    if(m_pezzoCorrente.y()<0) return;
    QList<QPoint> mosse = mossePossibili(m_pezzoCorrente,true);
    filtraScacco(mosse,m_pezzoCorrente);
    const Pezzo pezzoCorrente = indexForPoint(m_pezzoCorrente).data().value<Pezzo>();
    Q_ASSERT(pezzoCorrente.valido());
    for(QList<QPoint>::const_iterator i = mosse.constBegin();i!=mosse.constEnd();++i){
        const QModelIndex currIdx=indexForPoint(*i);
        if(pezzoCorrente.tipo == Pezzo::Tipo::Pedone && !currIdx.data().isValid() && currIdx.column() != m_pezzoCorrente.x()){
            // un pedone si muove in diagonale su una cella vuota solo se mangia "en passant"
            m_model->setData(currIdx,CellaMangiabile,StatusCellRole);
            continue;
        }
        m_model->setData(currIdx,currIdx.data().isValid() ? CellaMangiabile : CellaVuotaMuovibile,StatusCellRole);
    }
}

bool OggettoScacchi::controllaDiagonalePedone(int rigaPartenza, int rigaArrivo, int colDiag, const Pezzo& pedone) const{
    if(rigaArrivo<0 || rigaArrivo>=8 || rigaPartenza<0 || rigaPartenza>=8 || colDiag<0 || colDiag>=8 || pedone.tipo!=Pezzo::Tipo::Pedone)
        return false;
    QVariant cellaMangiabile = m_model->index(rigaArrivo,colDiag).data();
    if(cellaMangiabile.isValid()){ // c'e' un altro pezzo nella cella in diagonale
        const Pezzo tempPezzo = cellaMangiabile.value<Pezzo>();
        if(tempPezzo.colore != pedone.colore) // il pezzo e' dell'avversario
            return true;
    }
    cellaMangiabile = m_model->index(rigaPartenza,colDiag).data();
    if(cellaMangiabile.isValid()){
        const Pezzo tempPezzo = cellaMangiabile.value<Pezzo>();
            if(
                tempPezzo.colore != pedone.colore
                && tempPezzo.tipo == Pezzo::Tipo::Pedone
                && m_model->index(rigaPartenza,colDiag).data(DoppioPassoRole).toBool()
                && tempPezzo.colore != pedone.colore
            ){
                //en passant
                return true;
            }
    }
    return false;
}

bool OggettoScacchi::controllaArrocco(Pezzo::Colore colr, bool aSinistra) const
{
    const int actionRow = colr==Pezzo::Colore::Bianco ? 7:0;
    const int colonnaTorre = aSinistra ? 0:7;
    const QVariant dataRe = m_model->index(actionRow,4).data();
    const QVariant dataTorre = m_model->index(actionRow,colonnaTorre).data();
    if(!dataRe.isValid() || !dataTorre.isValid())
        return false;
    if(!dataRe.value<Pezzo>().primaMossa)
        return false;
    if(!dataTorre.value<Pezzo>().primaMossa)
        return false;
    if(m_model->index(actionRow,4).data(ScaccoRole).isValid())
        return false; // non si puo' arrocare se si e' sotto scacco
    Q_ASSERT(dataRe.value<Pezzo>().colore == dataTorre.value<Pezzo>().colore);
    Q_ASSERT(dataRe.value<Pezzo>().colore==colr);
    for(int i=qMin(colonnaTorre,4)+1;i<qMax(4,colonnaTorre);++i){
        if(m_model->index(actionRow,i).data().isValid()) // la via tra re e torre non e' libera
            return false;
    }
    for(int i=qMin(colonnaTorre,4)+1;i<qMax(4,colonnaTorre);++i){
        if(scacco(colr,QPoint(i,actionRow))) //il re, durante il movimento dell'arrocco, non deve attraversare caselle in cui si troverebbe sotto scacco.
            return false;
    }
    return true;
}

void OggettoScacchi::filtraScacco(QList<QPoint> &mosse,const QPoint &pedina) const
{
    Q_ASSERT(pedina.x()>=0 && pedina.y()>=0 && pedina.x()<8 && pedina.y()<8);
    Q_ASSERT(m_model->index(pedina.y(),pedina.x()).data().isValid());
    const QVariant pezzovariant =m_model->index(pedina.y(),pedina.x()).data();
    Pezzo pezzoDaMuovere = pezzovariant.value<Pezzo>();
    const QPoint posRe = posizioneRe(pezzoDaMuovere.colore);
    if(pezzoDaMuovere.tipo != Pezzo::Tipo::Re && m_model->index(posRe.y(),posRe.x()).data(ScaccoRole).toInt()!=CellaScacco)
        return;
    pezzoDaMuovere.primaMossa=false;
    for(QList<QPoint>::iterator i=mosse.begin();i!=mosse.end();){
        // simula la mossa
        const QVariant oldVal = m_model->index(i->y(),i->x()).data();
        m_model->setData(m_model->index(i->y(),i->x()),QVariant::fromValue(pezzoDaMuovere));
        m_model->setData(m_model->index(pedina.y(),pedina.x()),QVariant());
        bool ancoraScacco = scacco(pezzoDaMuovere.colore);
        m_model->setData(m_model->index(i->y(),i->x()),oldVal);
        m_model->setData(m_model->index(pedina.y(),pedina.x()),pezzovariant);
        if(ancoraScacco)
            i=mosse.erase(i);
        else
            ++i;
    }
}

QList<QPoint> OggettoScacchi::mossePossibili(const QPoint &pedina, bool cntrlArrocco) const
{
    QList<QPoint> result;
    if(pedina.x()>=8 || pedina.y() >=8 || pedina.x()<0 || pedina.y() <0)
        return result;
    const QVariant cella = m_model->index(pedina.y(),pedina.x()).data();
    if(!cella.isValid())
        return result;
    const Pezzo pezzoCorrente = cella.value<Pezzo>();
    switch(pezzoCorrente.tipo){
    case Pezzo::Tipo::Pedone:{
        int rigaAzione = pedina.y() + (pezzoCorrente.colore==Pezzo::Colore::Bianco ? -1:1);
        if(controllaDiagonalePedone(pedina.y(),rigaAzione,pedina.x()+1,pezzoCorrente))
            result.append(QPoint(pedina.x()+1,rigaAzione));
        if(controllaDiagonalePedone(pedina.y(),rigaAzione,pedina.x()-1,pezzoCorrente))
            result.append(QPoint(pedina.x()-1,rigaAzione));
        if(!m_model->index(rigaAzione,pedina.x()).data().isValid()){ // la cella di fronte e' libera
            result.append(QPoint(pedina.x(),rigaAzione));
            if(pezzoCorrente.primaMossa){
                rigaAzione += (pezzoCorrente.colore==Pezzo::Colore::Bianco ? -1:1);
                if(!m_model->index(rigaAzione,pedina.x()).data().isValid()) // la cella 2 posti avanti e' libera
                    result.append(QPoint(pedina.x(),rigaAzione));
            }
        }
    }break;
    case Pezzo::Tipo::Alfiere:{
        for(QPoint shift : {QPoint(1,-1),QPoint(1,1),QPoint(-1,-1),QPoint(-1,1)}){
            for(QPoint cursore = pedina+shift;;cursore+=shift){
                if(cursore.x()<0 || cursore.y()<0 || cursore.x()>=8 || cursore.y()>=8)
                    break;
                const QVariant dataCursore=m_model->index(cursore.y(),cursore.x()).data();
                if(dataCursore.isValid()){
                    if(dataCursore.value<Pezzo>().colore!=pezzoCorrente.colore)
                        result.append(cursore);
                    break;
                }
                result.append(cursore);
            }
        }
    }break;
    case Pezzo::Tipo::Torre:{
        for(QPoint shift : {QPoint(1,0),QPoint(-1,0),QPoint(0,-1),QPoint(0,1)}){
            for(QPoint cursore = pedina+shift;;cursore+=shift){
                if(cursore.x()<0 || cursore.y()<0 || cursore.x()>=8 || cursore.y()>=8)
                    break;
                const QVariant dataCursore=m_model->index(cursore.y(),cursore.x()).data();
                if(dataCursore.isValid()){
                    if(dataCursore.value<Pezzo>().colore!=pezzoCorrente.colore)
                        result.append(cursore);
                    break;
                }
                result.append(cursore);
            }
        }
    }break;
    case Pezzo::Tipo::Regina:{
        for(QPoint shift : {QPoint(1,0),QPoint(-1,0),QPoint(0,-1),QPoint(0,1),QPoint(1,-1),QPoint(1,1),QPoint(-1,-1),QPoint(-1,1)}){
            for(QPoint cursore = pedina+shift;;cursore+=shift){
                if(cursore.x()<0 || cursore.y()<0 || cursore.x()>=8 || cursore.y()>=8)
                    break;
                const QVariant dataCursore=m_model->index(cursore.y(),cursore.x()).data();
                if(dataCursore.isValid()){
                    if(dataCursore.value<Pezzo>().colore!=pezzoCorrente.colore)
                        result.append(cursore);
                    break;
                }
                result.append(cursore);
            }
        }
    }break;
    case Pezzo::Tipo::Cavallo:{
        for(QPoint shift : {QPoint(2,1),QPoint(2,-1),QPoint(-2,1),QPoint(-2,-1),QPoint(1,2),QPoint(-1,2),QPoint(1,-2),QPoint(-1,-2)}){
            const QPoint cursore = pedina+shift;
            if(cursore.x()<0 || cursore.y()<0 || cursore.x()>=8 || cursore.y()>=8)
                continue;
            const QVariant dataCursore=m_model->index(cursore.y(),cursore.x()).data();
            if(dataCursore.isValid()){
                if(dataCursore.value<Pezzo>().colore==pezzoCorrente.colore)
                    continue;
            }
            result.append(cursore);
        }
    }break;
    case Pezzo::Tipo::Re:{
        for(int i=-1;i<=1;++i){
            for(int j=-1;j<=1;++j){
                const QPoint cursore = pedina+QPoint(i,j);
                if(cursore.x()<0 || cursore.y()<0 || cursore.x()>=8 || cursore.y()>=8)
                    continue;
                const QVariant dataCursore=m_model->index(cursore.y(),cursore.x()).data();
                if(dataCursore.isValid()){
                    if(dataCursore.value<Pezzo>().colore==pezzoCorrente.colore)
                        continue;
                }
                result.append(cursore);
            }
        }
        if(cntrlArrocco){
            if(controllaArrocco(pezzoCorrente.colore,true))
                result.append(QPoint(1,pedina.y()));
            if(controllaArrocco(pezzoCorrente.colore,false))
                result.append(QPoint(6,pedina.y()));
        }
    }break;
    }
    return result;
}

QModelIndex OggettoScacchi::indexForPoint(const QPoint &pnt) const{
    return m_model->index(pnt.y(),pnt.x());
}

QPoint OggettoScacchi::pointForIndex(const QModelIndex &idx) const
{
    if(!idx.isValid())
        return QPoint();
    return QPoint(idx.column(),idx.row());
}

void OggettoScacchi::rimuoviTuttiStati(Pezzo::Colore colr, bool rimuoviScacco)
{
    for(int i=0;i<8;++i){
        for(int j=0;j<8;++j){
            m_model->setData(m_model->index(i,j),QVariant(),StatusCellRole);
            const QVariant pezzoData = m_model->index(i,j).data();
            if(pezzoData.isValid() && pezzoData.value<Pezzo>().colore!=colr)
                m_model->setData(m_model->index(i,j),QVariant(),DoppioPassoRole);
            if(rimuoviScacco)
                m_model->setData(m_model->index(i,j),QVariant(),ScaccoRole);
        }
    }
}
void OggettoScacchi::rimuoviTuttiStati(bool rimuoviScacco)
{
    for(int i=0;i<8;++i){
        for(int j=0;j<8;++j){
            m_model->setData(m_model->index(i,j),QVariant(),StatusCellRole);
            if(rimuoviScacco)
                m_model->setData(m_model->index(i,j),QVariant(),ScaccoRole);
        }
    }
}

void OggettoScacchi::cambiaTurno()
{
    rimuoviTuttiStati(m_turnoDi, true);
    m_pezzoCorrente.setX(-1);
    m_pezzoCorrente.setY(-1);
    m_turnoDi = (m_turnoDi == Pezzo::Colore::Bianco ? Pezzo::Colore::Nero : Pezzo::Colore::Bianco);
    const QPoint posRe = posizioneRe(m_turnoDi);
    const bool isScacco = scacco(m_turnoDi,posRe);
    if(isScacco)
        m_model->setData(indexForPoint(posRe),CellaScacco,ScaccoRole);
    const bool isstallo = isStallo(m_turnoDi);
    if(isstallo){
        if(isScacco)
            scaccoMatto(m_turnoDi == Pezzo::Colore::Bianco ? Pezzo::Colore::Nero : Pezzo::Colore::Bianco);
        else
            stallo();
        return startGame();
    }
    turnoDi(m_turnoDi);
}

QPoint OggettoScacchi::posizioneRe(Pezzo::Colore colr) const
{
    for(int i=0;i<8;++i){
        for(int j=0;j<8;++j){
            const Pezzo tempPezzo = m_model->index(i,j).data().value<Pezzo>();
            if(tempPezzo.tipo==Pezzo::Tipo::Re && tempPezzo.colore==colr)
                return QPoint(j,i);
        }
    }
    Q_UNREACHABLE();
    return QPoint(-1,-1);
}

bool OggettoScacchi::scacco(Pezzo::Colore colr, const QPoint &posRe) const
{
    Q_ASSERT(colr!=Pezzo::Colore::Nessuno);
    Q_ASSERT(posRe.x()>=0 && posRe.y()>=0 && posRe.x()<8 && posRe.y()<8);
    for(int i=0;i<8;++i){
        for(int j=0;j<8;++j){
            const QVariant dataCella = m_model->index(i,j).data();
            if(dataCella.isValid()){
                if(dataCella.value<Pezzo>().colore != colr){
                    if(mossePossibili(QPoint(j,i)).contains(posRe))
                        return true;
                }
            }
        }
    }
    return false;
}

bool OggettoScacchi::scacco(Pezzo::Colore colr) const
{
    return scacco(colr,posizioneRe(colr));
}

bool OggettoScacchi::isStallo(Pezzo::Colore colr) const
{
    for(int i=0;i<8;++i){
        for(int j=0;j<8;++j){
            const QVariant dataCella = m_model->index(i,j).data();
            if(dataCella.isValid()){
                if(dataCella.value<Pezzo>().colore == colr){
                    QList<QPoint> tutteLeMosse = mossePossibili(QPoint(j,i));
                    filtraScacco(tutteLeMosse,QPoint(j,i));
                    if(!tutteLeMosse.isEmpty())
                        return false;
                }
            }
        }
    }
    return true;
}

