#include "knotlabel.h"
#include "knotlistsource.h"

#include <BaseKnotSequence.h>
#include <QDrag>
#include <QDragEnterEvent>
#include <QMimeData>

KnotListSource::KnotListSource()
{
    setDragEnabled(true);
    setViewMode(QListView::ViewMode::ListMode);
    setSpacing(1);
    //setAcceptDrops(true);
    setDropIndicatorShown(true);
    setSelectionMode(QAbstractItemView::NoSelection);
}

void KnotListSource::addKnot(QString type)
{
    KnotLabel *label = new KnotLabel(type);

    QListWidgetItem *pieceItem = new QListWidgetItem;
    //pieceItem->setText(type);
    pieceItem->setData(Qt::UserRole, QVariant(type));
    pieceItem->setData(Qt::UserRole+1, QPoint(0, 0));
    pieceItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled);
    pieceItem->setSizeHint(QSize(pieceItem->sizeHint().width(), label->minimumHeight()));
    addItem(pieceItem);
    setItemWidget(pieceItem, label);
}

void KnotListSource::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasFormat(KnotListSource::knotMimeType()))
        event->accept();
    else
        event->ignore();
}

void KnotListSource::dragMoveEvent(QDragMoveEvent *event)
{
    if (event->mimeData()->hasFormat(KnotListSource::knotMimeType())) {
        event->setDropAction(Qt::MoveAction);
        event->accept();
    } else {
        event->ignore();
    }
}

void KnotListSource::dropEvent(QDropEvent *event)
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


void KnotListSource::mousePressEvent(QMouseEvent *event)
{
    KnotLabel *child = static_cast<KnotLabel*>(childAt(event->pos()));
    if (!child)
        return;

    QPoint hotSpot = event->pos() - child->pos();

    QByteArray itemData;
    QDataStream dataStream(&itemData, QIODevice::WriteOnly);
    dataStream << child->text() << QPoint(hotSpot);

    QMimeData *mimeData = new QMimeData;
    mimeData->setData(KnotListSource::knotMimeType(), itemData);
    mimeData->setText(child->text());

    QDrag *drag = new QDrag(this);
    drag->setMimeData(mimeData);
    //drag->setPixmap(*child->pixmap());
    drag->setHotSpot(hotSpot);

    //child->hide();

    if (drag->exec(Qt::MoveAction | Qt::CopyAction, Qt::CopyAction) == Qt::MoveAction)
        child->show();

//    if (drag->exec(Qt::MoveAction | Qt::CopyAction, Qt::CopyAction) == Qt::MoveAction)
//        child->close();
//    else
//        child->show();
}

void KnotListSource::startDrag(Qt::DropActions supportedActions)
{
//    QListWidgetItem *item = currentItem();

//    QByteArray itemData;
//    QDataStream dataStream(&itemData, QIODevice::WriteOnly);
//    QString knotstyle = qvariant_cast<QString>(item->data(Qt::UserRole));
//    QPoint location = item->data(Qt::UserRole+1).toPoint();

    //dataStream << knotstyle << location;

//    KnotLabel *child = static_cast<KnotLabel*>(childAt(event->pos()));
//    if (!child)
//       return;

//    QMimeData *mimeData = new QMimeData;
//    mimeData->setData(KnotListSource::knotMimeType(), itemData);
//    mimeData->setText(child->text());

//    QPoint hotSpot = event->pos() - child->pos();

//    QDrag *drag = new QDrag(this);
//    drag->setMimeData(mimeData);
//    drag->setPixmap(*child->pixmap());
//    drag->setHotSpot(hotSpot);

//    child->hide();

    //QDrag *drag = new QDrag(this);
    //drag->setMimeData(mimeData);
    //drag->setHotSpot(QPoint(pixmap.width()/2, pixmap.height()/2));
    //drag->setPixmap(pixmap);

    //if (drag->exec(Qt::MoveAction) == Qt::MoveAction)
    //    delete takeItem(row(item));
}







