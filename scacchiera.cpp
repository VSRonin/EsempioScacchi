#include "scacchiera.h"
#include "pezzo.h"
#include <QGridLayout>
#include <QTableView>
#include <QHeaderView>
#include <QStandardItemModel>
#include <QStandardItem>
#include "pezzodelegate.h"
#include "scacchiglobal.h"
#include <QLabel>
#include <QPushButton>
#include <QMessageBox>
Scacchiera::Scacchiera(QWidget *parent)
    : QWidget(parent)
    , m_turnoDi(Pezzo::Colore::Bianco)
    , m_pezzoCorrente(-1,-1)
{
    m_turnoLabel = new QLabel(this);
    m_turnoLabel->setAlignment(Qt::AlignCenter);
    m_turnoLabel->setFont(QFont("Arial",20));
    m_tableView=new QTableView(this);
    m_model=new QStandardItemModel(this);
    m_model->insertRows(0,8);
    m_model->insertColumns(0,8);
    m_tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    m_tableView->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    m_tableView->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
    m_tableView->setModel(m_model);
    m_tableView->setSortingEnabled(false);
    m_tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_tableView->setItemDelegate(new PezzoDelegate(this));
    m_tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    m_tableView->setSelectionBehavior(QAbstractItemView::SelectItems);
    for(int i=0;i<8;++i){
        m_tableView->horizontalHeader()->resizeSection(i,50);
        m_tableView->verticalHeader()->resizeSection(i,50);
        m_model->setHeaderData(i,Qt::Horizontal,QChar('A'+i));
        m_model->setHeaderData(i,Qt::Vertical,i+1);
        for(int j=0;j<8;++j){
            m_model->setData(m_model->index(i,j),Qt::AlignCenter,Qt::TextAlignmentRole);
            m_model->setData(m_model->index(i,j), QBrush(j%2==i%2 ? Qt::white : Qt::lightGray),Qt::BackgroundRole);
            m_model->item(i,j)->setFlags(Qt::ItemIsEnabled);
        }
    }
    QGridLayout* mainlay=new QGridLayout(this);
    mainlay->addWidget(m_tableView,2,1);
    mainlay->addWidget(m_turnoLabel,1,1);
    mainlay->addItem(new QSpacerItem(0,0,QSizePolicy::Expanding,QSizePolicy::Preferred),1,0);
    mainlay->addItem(new QSpacerItem(0,0,QSizePolicy::Expanding,QSizePolicy::Preferred),1,2);
    mainlay->addItem(new QSpacerItem(0,0,QSizePolicy::Preferred,QSizePolicy::Expanding),0,1);
    mainlay->addItem(new QSpacerItem(0,0,QSizePolicy::Preferred,QSizePolicy::Expanding),3,1);
    connect(m_tableView,&QTableView::clicked,this,&Scacchiera::cliccato);
    retranslateUi();
    startGame();
}
void Scacchiera::startGame(){
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
}

void Scacchiera::retranslateUi()
{
    setWindowTitle(tr("Scacchi"));
    m_turnoLabel->setText(tr("Turno di %1").arg(m_turnoDi == Pezzo::Colore::Bianco ? QChar(0x2654) : QChar(0x265A)));
}


void Scacchiera::cliccato(const QModelIndex& idx)
{
    if(!idx.isValid())
        return;
    Q_ASSERT(idx.model()==m_model);
    if(m_pezzoCorrente.y()<0){ //nessun pezzo selezionato
        const QVariant tempData = idx.data();
        if(tempData.isValid()){ //la cella contiene un pezzo
            Pezzo tempPezzo = tempData.value<Pezzo>();
            if(tempPezzo.colore==m_turnoDi){ //il pezzo selezionato e' del giocatore che ha il turno
                m_pezzoCorrente.setY(idx.row());
                m_pezzoCorrente.setX(idx.column());
                m_model->setData(idx,PezzoSelezionato,StatusCellRole);
                formattaMossePossibili();
            }
        }
    }
    else{
        if(m_pezzoCorrente.y() == idx.row() && m_pezzoCorrente.x() == idx.column()){ //deseleziona pezzo corrente
            m_pezzoCorrente.setX(-1);
            m_pezzoCorrente.setY(-1);
            rimuoviTuttiStati(false);
        }
        switch(idx.data(StatusCellRole).toInt()){
        case CellaMangiabile:
        case CellaVuotaMuovibile:{
            Pezzo tempPezzo = m_model->index(m_pezzoCorrente.y(),m_pezzoCorrente.x()).data().value<Pezzo>();
            tempPezzo.primaMossa = false;
            if(tempPezzo.tipo == Pezzo::Tipo::Re && qAbs(m_pezzoCorrente.x()-idx.column())>1){
                //Arrocco
                const QModelIndex idxTorre = m_model->index(m_pezzoCorrente.y(),idx.column()>m_pezzoCorrente.x() ? 7:0);
                Pezzo tempTorre = idxTorre.data().value<Pezzo>();
                Q_ASSERT(tempTorre.tipo == Pezzo::Tipo::Torre && tempTorre.colore == tempPezzo.colore);
                tempTorre.primaMossa = false;
                m_model->setData(m_model->index(m_pezzoCorrente.y(),idx.column()>m_pezzoCorrente.x() ? 5:2),QVariant::fromValue(tempTorre));
                m_model->setData(idxTorre,QVariant());
            }
            else if(tempPezzo.tipo == Pezzo::Tipo::Pedone){
                if((idx.row()==0 || idx.row()==7)){
                    // pedone raggiunge la fine
                    QMessageBox domandaTrasformazione;
                    domandaTrasformazione.setWindowTitle(tr("Trasforma Pedone"));
                    domandaTrasformazione.setText(tr("In cosa vuoi trasformare il pedone?"));
                    QPushButton *reginaBut =domandaTrasformazione.addButton(tr("Regina"),QMessageBox::AcceptRole);
                    QPushButton *torreBut =domandaTrasformazione.addButton(tr("Torre"),QMessageBox::AcceptRole);
                    QPushButton *alfiereBut =domandaTrasformazione.addButton(tr("Alfiere"),QMessageBox::AcceptRole);
                    QPushButton *cavalloBut =domandaTrasformazione.addButton(tr("Cavallo"),QMessageBox::AcceptRole);
                    domandaTrasformazione.exec();
                    if(domandaTrasformazione.clickedButton()==cavalloBut)
                        tempPezzo.tipo = Pezzo::Tipo::Cavallo;
                    else if(domandaTrasformazione.clickedButton()==torreBut)
                        tempPezzo.tipo = Pezzo::Tipo::Torre;
                    else if(domandaTrasformazione.clickedButton()==reginaBut)
                        tempPezzo.tipo = Pezzo::Tipo::Regina;
                    else if(domandaTrasformazione.clickedButton()==alfiereBut)
                        tempPezzo.tipo = Pezzo::Tipo::Alfiere;
                }
                if(!idx.data().isValid() && m_pezzoCorrente.x()!=idx.column()){
                    // un pedone si muove in diagonale su una cella vuota solo se mangia "en passant"
                    Q_ASSERT(m_model->index(m_pezzoCorrente.y(),idx.column()).data(DoppioPassoRole).toBool());
                    m_model->setData(m_model->index(m_pezzoCorrente.y(),idx.column()),QVariant());
                }
                if(qAbs(m_pezzoCorrente.y()-idx.row())==2)
                    m_model->setData(idx,true,DoppioPassoRole);
            }
            m_model->setData(idx,QVariant::fromValue(tempPezzo));
            m_model->setData(m_model->index(m_pezzoCorrente.y(),m_pezzoCorrente.x()),QVariant());
            cambiaTurno();
        }break;
        default: break;
        }
    }
}

void Scacchiera::formattaMossePossibili()
{
    if(m_pezzoCorrente.y()<0) return;
    QList<QPoint> mosse = mossePossibili(m_pezzoCorrente);
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

bool Scacchiera::controllaDiagonalePedone(int rigaPartenza, int rigaArrivo, int colDiag, const Pezzo& pedone) const{
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
        Q_ASSERT(tempPezzo.tipo == Pezzo::Tipo::Pedone);
        if(m_model->index(rigaPartenza,colDiag).data(DoppioPassoRole).toBool() && tempPezzo.colore != pedone.colore){
            //en passant
            return true;
        }
    }
    return false;
}

bool Scacchiera::controllaArrocco(Pezzo::Colore colr, bool aSinistra) const
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
        if(scacco(colr,QPoint(i,actionRow))) //il re, durante il movimento dell'arrocco, non deve attraversare caselle in cui si troverebbe sotto scacco.
            return false;
    }
    return true;
}

void Scacchiera::filtraScacco(QList<QPoint> &mosse,const QPoint &pedina) const
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

QList<QPoint> Scacchiera::mossePossibili(const QPoint &pedina) const
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
        if(controllaArrocco(pezzoCorrente.colore,true))
            result.append(QPoint(1,pedina.y()));
        if(controllaArrocco(pezzoCorrente.colore,false))
            result.append(QPoint(6,pedina.y()));
    }break;
    }
    return result;
}

QModelIndex Scacchiera::indexForPoint(const QPoint &pnt) const{
    return m_model->index(pnt.y(),pnt.x());
}

void Scacchiera::rimuoviTuttiStati(Pezzo::Colore colr, bool rimuoviScacco)
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
void Scacchiera::rimuoviTuttiStati(bool rimuoviScacco)
{
    for(int i=0;i<8;++i){
        for(int j=0;j<8;++j){
            m_model->setData(m_model->index(i,j),QVariant(),StatusCellRole);
            if(rimuoviScacco)
                m_model->setData(m_model->index(i,j),QVariant(),ScaccoRole);
        }
    }
}

void Scacchiera::cambiaTurno()
{
    rimuoviTuttiStati(m_turnoDi, true);
    m_pezzoCorrente.setX(-1);
    m_pezzoCorrente.setY(-1);
    m_turnoDi = (m_turnoDi == Pezzo::Colore::Bianco ? Pezzo::Colore::Nero : Pezzo::Colore::Bianco);
    const QPoint posRe = posizioneRe(m_turnoDi);
    const bool isScacco = scacco(m_turnoDi,posRe);
    if(isScacco)
        m_model->setData(indexForPoint(posRe),CellaScacco,ScaccoRole);
    const bool isStallo = stallo(m_turnoDi);
    if(isStallo){
        if(isScacco)
            QMessageBox::information(this,tr("Scacco Matto"),tr("Il Giocatore %1 Vince!").arg(m_turnoDi == Pezzo::Colore::Bianco ? tr("Nero"):tr("Bianco")));
        else
            QMessageBox::information(this,tr("Stallo"),tr("Pareggio!"));
        return startGame();
    }
    m_turnoLabel->setText(tr("Turno di %1").arg(m_turnoDi == Pezzo::Colore::Bianco ? QChar(0x2654) : QChar(0x265A)));

}

QPoint Scacchiera::posizioneRe(Pezzo::Colore colr) const
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

bool Scacchiera::scacco(Pezzo::Colore colr, const QPoint &posRe) const
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

bool Scacchiera::scacco(Pezzo::Colore colr) const
{
    return scacco(colr,posizioneRe(colr));
}

bool Scacchiera::stallo(Pezzo::Colore colr) const
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

