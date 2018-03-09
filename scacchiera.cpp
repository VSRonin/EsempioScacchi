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
    , m_turnoBianco(true)
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
    m_turnoBianco=true;
    for(int i=0;i<8;++i){
        for(int j=0;j<8;++j){
            m_model->setData(m_model->index(i,j),QVariant(),StatusCellRole);
            m_model->setData(m_model->index(i,j),QVariant(),ScaccoRole);
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
    m_turnoLabel->setText(tr("Turno di %1").arg(m_turnoBianco ? QChar(0x2654) : QChar(0x265A)));
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
            if((tempPezzo.colore==Pezzo::Colore::Bianco)==m_turnoBianco){ //il pezzo selezionato e' del giocatore che ha il turno
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
            else if((idx.row()==0 || idx.row()==7) && tempPezzo.tipo == Pezzo::Tipo::Pedone){
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
    for(QList<QPoint>::const_iterator i = mosse.constBegin();i!=mosse.constEnd();++i){
        const QModelIndex currIdx=m_model->index(i->y(),i->x());
        m_model->setData(currIdx,currIdx.data().isValid() ? CellaMangiabile : CellaVuotaMuovibile,StatusCellRole);
    }
}

bool Scacchiera::controllaDiagonalePedone(int rig, int colDiag, const Pezzo& pedone) const{
    if(rig<0 || rig>=8 || colDiag<0 || colDiag>=8 || pedone.tipo!=Pezzo::Tipo::Pedone)
        return false;
    const QVariant cellaDiagonale = m_model->index(rig,colDiag).data();
    if(cellaDiagonale.isValid()){ // c'e' un altro pezzo nella cella in diagonale
        const Pezzo tempPezzo = cellaDiagonale.value<Pezzo>();
        if(tempPezzo.colore != pedone.colore) // il pezzo e' dell'avversario
            return true;
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
    Q_ASSERT(dataRe.value<Pezzo>().colore == dataTorre.value<Pezzo>().colore);
    Q_ASSERT(dataRe.value<Pezzo>().colore==colr);
    for(int i=qMin(colonnaTorre,4)+1;i<qMax(4,colonnaTorre);++i)
        if(m_model->index(actionRow,i).data().isValid())
            return false;
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
        if(controllaDiagonalePedone(rigaAzione,pedina.x()+1,pezzoCorrente))
            result.append(QPoint(pedina.x()+1,rigaAzione));
        if(controllaDiagonalePedone(rigaAzione,pedina.x()-1,pezzoCorrente))
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
    rimuoviTuttiStati(true);
    m_pezzoCorrente.setX(-1);
    m_pezzoCorrente.setY(-1);
    m_turnoBianco = !m_turnoBianco;
    const QPoint posRe = posizioneRe(m_turnoBianco ? Pezzo::Colore::Bianco : Pezzo::Colore::Nero);
    const bool isScacco = scacco(m_turnoBianco ? Pezzo::Colore::Bianco : Pezzo::Colore::Nero,posRe);
    if(isScacco)
        m_model->setData(indexForPoint(posRe),CellaScacco,ScaccoRole);
    const bool isStallo = stallo(m_turnoBianco ? Pezzo::Colore::Bianco : Pezzo::Colore::Nero);
    if(isStallo){
        if(isScacco)
            QMessageBox::information(this,tr("Scacco Matto"),tr("Il Giocatore %1 Vince!").arg(m_turnoBianco ? tr("Nero"):tr("Bianco")));
        else
            QMessageBox::information(this,tr("Stallo"),tr("Pareggio!"));
        return startGame();
    }
    m_turnoLabel->setText(tr("Turno di %1").arg(m_turnoBianco ? QChar(0x2654) : QChar(0x265A)));

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
    Q_ASSERT(indexForPoint(posRe).data().isValid());
    Q_ASSERT(indexForPoint(posRe).data().value<Pezzo>().colore == colr);
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

