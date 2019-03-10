#include "knotlabel.h"
#include "knotlistdest.h"
#include "knotlistsource.h"
#include "qtutils.h"

#include <BaseKnotSequence.h>
#include <QDrag>
#include <QDragEnterEvent>
#include <QMimeData>

KnotListDest::KnotListDest()
{
    setDragEnabled(true);
    setViewMode(QListView::ViewMode::ListMode);
    setSpacing(1);
    setAcceptDrops(true);
    setDropIndicatorShown(true);
    setSelectionMode(QAbstractItemView::SingleSelection);
    //setDragDropMode(QAbstractItemView::InternalMove);
    connect(this, &QListWidget::currentItemChanged, this, &KnotListDest::currentItemChanged);
    QString colorstr = QTUtils::color2str(QTUtils::Blue());
    QString style = "";
    style += "QListWidget::item {";
    style += "background: white;";
    //style += "background-clip: border;";
    style += "color : " + colorstr +";";
    //style += "padding : 2px;";
//    style += "margin-left : 8px;";
//    style += "margin-right : 8px;";
//    style += "border: 2px solid " + colorstr +";";
//    style += "border-radius: 10px;";
    style += "}";
    style += "QListWidget::item:selected {";
    style += "background : " + colorstr +";";
    //style += "background-clip: border;";
    style += "color : white;";
    //style += "padding : 2px;";
    style += "margin-left : 8px;";
    style += "margin-right : 8px;";
    style += "border: 2px solid " + colorstr +";";
    style += "border-radius: 10px;";
    style += "}";
    //style += "QListWidget::item:selected { selection-background-color: transparent; };";
    setStyleSheet(style);
}

void KnotListDest::addKnot(QString type)
{
    KnotLabel *label = new KnotLabel(type);

    QListWidgetItem *pieceItem = new QListWidgetItem;
    pieceItem->setText(type);
    pieceItem->setData(Qt::UserRole, QVariant(type));
    pieceItem->setData(Qt::UserRole+1, QPoint(0, 0));
    pieceItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled);
    pieceItem->setSizeHint(QSize(pieceItem->sizeHint().width(), label->minimumHeight()));
    pieceItem->setTextAlignment(Qt::AlignCenter);
    //pieceItem->setSelected(true);
    addItem(pieceItem);
    //setItemWidget(pieceItem, label);
    cout << "adding item at index = " << count() << endl;
    //knots.push_back(getKnotFromName(type));
    //getKnots();
}

void KnotListDest::addKnot(QString type, int index)
{
    KnotLabel *label = new KnotLabel(type);

    QListWidgetItem *pieceItem = new QListWidgetItem;
    pieceItem->setText(type);
    pieceItem->setData(Qt::UserRole, QVariant(type));
    pieceItem->setData(Qt::UserRole+1, QPoint(0, 0));
    pieceItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled);
    pieceItem->setSizeHint(QSize(pieceItem->sizeHint().width(), label->minimumHeight()));
    pieceItem->setTextAlignment(Qt::AlignCenter);
    //pieceItem->setSelected(true);
    insertItem(index, pieceItem);
    //setItemWidget(pieceItem, label);
    cout << "adding item at index = " << index << endl;
    //knots.insert(knots.begin() + index, getKnotFromName(type));
    //getKnots();
}

void KnotListDest::currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    if (current)
    {
        emit knotChanged(knots.at(indexFromItem(current).row()));
    }
    //    KnotLabel *current_child = static_cast<KnotLabel*>(itemWidget(current));
//    KnotLabel *previous_child = static_cast<KnotLabel*>(itemWidget(previous));
//    if (current_child)
//        current_child->setSelected(true);
//    if (previous_child)
//        previous_child->setSelected(false);
}


void KnotListDest::dragEnterEvent(QDragEnterEvent *event)
{
    cout << "dragEnterEvent" << endl;
    if (event->mimeData()->hasFormat(KnotListSource::knotMimeType()))
        event->accept();
    else
        event->ignore();
}

void KnotListDest::dragMoveEvent(QDragMoveEvent *event)
{
    //cout << "dragMoveEvent" << endl;
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

        //KnotLabel *child = static_cast<KnotLabel*>(childAt(event->pos()));
        QModelIndex modelindex = indexAt(event->pos());
        int index = modelindex.row();
        //if (event->pos().y() > child->geometry().center().y()) // if it is after the middle of the view
        //    index = index + 1;

        if (index != -1) addKnot(knottype, index);
        else addKnot(knottype);

        if (index == -1) index = count() - 1; // already added to the count whith addknot
        if (is_sorting) // it comes from inside this list
        {
            if (sorting_index_start != index)
                Utils::move(knots, sorting_index_start, index);
            //model()->moveRow(sorting_modelindex_start, sorting_index_start, modelindex, index);
            is_sorting = false;
        }
        else {
            BaseKnotSequence *knot = getKnotFromName(knottype);
            knots.insert(knots.begin() + index, knot);
            emit knotAdded(knot);
        }

        setCurrentRow(index);

        event->setDropAction(Qt::MoveAction);
        event->accept();
    } else {
        event->ignore();
    }
}

void KnotListDest::dragLeaveEvent(QDragLeaveEvent *event)
{
    if (is_sorting) // it comes from inside this list
    {
        BaseKnotSequence *knot = knots.at(sorting_index_start);
        knots.erase(knots.begin() + sorting_index_start);
        emit knotRemoved(knot);
        is_sorting = false;
        event->accept();
    }
    else
    {
        event->ignore();
    }
    cout << "dragLeaveEvent" << endl;
    event->accept();
}

//void KnotListDest::startDrag(Qt::DropActions supportedActions)
//{
//    QListWidgetItem* item = currentItem();

//    QByteArray itemData;
//    QDataStream dataStream(&itemData, QIODevice::WriteOnly);
//    dataStream << item->text() << QPoint(0, 0);

//    QMimeData *mimeData = new QMimeData;
//    mimeData->setData(KnotListSource::knotMimeType(), itemData);
//    mimeData->setText(item->text());

//    QDrag *drag = new QDrag(this);
//    drag->setMimeData(mimeData);
//    //drag->setPixmap(*child->pixmap());

//    if (drag->exec(Qt::MoveAction) == Qt::MoveAction) {
//        delete takeItem(row(item));
//        //emit itemDroped();
//    }
//}

void KnotListDest::mousePressEvent(QMouseEvent *event)
{
    if ( event->button() == Qt::LeftButton )
    {
        QModelIndex modelind = indexAt(event->pos());
        int ind = modelind.row();
        if (ind == -1)
            return;

        is_sorting = true;
        sorting_index_start = ind;
        sorting_modelindex_start = modelind;

//        KnotLabel *child = static_cast<KnotLabel*>(childAt(event->pos()));
//        if (!child)
//            return;

        QListWidgetItem* item = takeItem(ind);
        //BaseFixedKnotSequence* knot = knots.at(ind);
        //dragging_knot = knots.at(ind);

        QByteArray itemData;
        QDataStream dataStream(&itemData, QIODevice::WriteOnly);
        dataStream << item->text() << QPoint(0, 0);

        QMimeData *mimeData = new QMimeData;
        mimeData->setData(KnotListSource::knotMimeType(), itemData);
        QString t = item->text();
        mimeData->setText(t);

        QDrag *drag = new QDrag(this);
        drag->setMimeData(mimeData);
        //drag->setPixmap(*child->pixmap());

        if (drag->exec(Qt::MoveAction) == Qt::MoveAction)
        {
            delete item;
            //delete knots.at(ind);
            cout << "deleting item index = " << ind << endl;
        }
        ////    if (drag->exec(Qt::MoveAction | Qt::CopyAction, Qt::CopyAction) == Qt::MoveAction)
        ////        child->close();
        ////    else
        ////        child->show();
    }
}

void KnotListDest::mouseReleaseEvent(QMouseEvent *event)
{
    cout << "mouseReleaseEvent" << endl;
}

void KnotListDest::mouseDoubleClickEvent( QMouseEvent *event )
{
    if ( event->button() == Qt::LeftButton )
    {
//        KnotLabel *child = static_cast<KnotLabel*>(childAt(event->pos()));
//        if (!child)
//            return;
        int i = indexAt(event->pos()).row();
        if (i != -1)
        {
            delete takeItem(i);
            BaseKnotSequence *knot = knots.at(i);
            knots.erase(knots.begin() + i);
            emit knotRemoved(knot);
            //delete knots[i];
        }
    }
}

Qt::DropAction KnotListDest::supportedDropActions()
{
    return Qt::MoveAction;
}

vector<BaseKnotSequence *> KnotListDest::getKnots()
{
    knots.clear();
    for(int i = 0; i < count(); ++i)
    {
        QListWidgetItem* item = this->item(i);
        knots.push_back(getKnotFromName(item->text()));
        cout << "TEXT = " << item->text().toStdString() << endl;;
        //cout << "TYPE = " << typeid(knots.at(knots.size() - 1)).name() << endl;
    }
    return knots;
}

KnotSequences KnotListDest::getKnotSequence()
{
    return KnotSequences(knots);
}

void KnotListDest::setKnots(Knots knots_input)
{
    knots.clear();
    clear();
    for (auto knot : knots_input)
    {
        knots.push_back(knot);
        addKnot(QString::fromStdString(knot->type()));
    }
    //setCurrentRow(0);
}

BaseKnotSequence *KnotListDest::getKnotFromName(QString name)
{
    QByteArray nameba = name.toLocal8Bit();
    const char *namechars = nameba.data();
    switch (Utils::str2int(namechars)) {
        case Utils::str2int("Begin Knots"):
            return  new BeginKS(bspline_n);
        case Utils::str2int("End Knots"):
            return  new EndKS(bspline_n);
        case Utils::str2int("Rational Knots"):
            return  new RationalKS(0, 1, 10, 1.5);
        case Utils::str2int("Value Knot"):
            return  new ValueKS(0.5);
        case Utils::str2int("Birational Knots"):
            return  new BiRationalKS(0, 1, 10, 1.5, 0.7, 0.5);
        case Utils::str2int("Uniform Knots"):
            return  new UniformKS(0, 1, 10);
        case Utils::str2int("Multiplicity Knots"):
            return  new MultiplicityValueKS(0.5, 2);
    }
}

