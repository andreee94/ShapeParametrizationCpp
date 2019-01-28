#include "knotlabel.h"
#include "knotlistdest.h"
#include "knotlistsource.h"

#include <BaseKnotSequence.h>
#include <QDrag>
#include <QDragEnterEvent>
#include <QMimeData>

KnotListDest::KnotListDest()
{
    setDragEnabled(true);
    setViewMode(QListView::ViewMode::ListMode);
    //setSpacing(10);
    setAcceptDrops(true);
    setDropIndicatorShown(true);
    setDragDropMode(QAbstractItemView::DropOnly);
}

void KnotListDest::addKnot(QString type)
{
    KnotLabel *label = new KnotLabel(type);

    QListWidgetItem *pieceItem = new QListWidgetItem;
    //pieceItem->setText(type);
    pieceItem->setData(Qt::UserRole, QVariant(type));
    pieceItem->setData(Qt::UserRole+1, QPoint(0, 0));
    pieceItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled);
    addItem(pieceItem);
    setItemWidget(pieceItem, label);
}


void KnotListDest::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasFormat(KnotListSource::knotMimeType()))
        event->accept();
    else
        event->ignore();
}

void KnotListDest::dragMoveEvent(QDragMoveEvent *event)
{
    if (event->mimeData()->hasFormat(KnotListSource::knotMimeType())) {
        event->setDropAction(Qt::MoveAction);
        event->accept();
    } else {
        event->ignore();
    }
}

void KnotListDest::dropEvent(QDropEvent *event)
{
    if (event->mimeData()->hasFormat(KnotListSource::knotMimeType())) {
        QByteArray pieceData = event->mimeData()->data(KnotListSource::knotMimeType());
        QDataStream dataStream(&pieceData, QIODevice::ReadOnly);
        QString knottype;
        QPoint location;
        dataStream >> knottype >> location;

        addKnot(knottype);

        event->setDropAction(Qt::MoveAction);
        event->accept();
    } else {
        event->ignore();
    }
}

void KnotListDest::startDrag(Qt::DropActions supportedActions)
{

}

void KnotListDest::mousePressEvent(QMouseEvent *event)
{

}
