#ifndef KNOTLISTDEST_H
#define KNOTLISTDEST_H

#include <BaseKnotSequence.h>
#include <Bspline.h>
#include <QListWidget>

class KnotListDest : public QListWidget
{
    Q_OBJECT
    public:
        KnotListDest();
        static QString knotMimeType() { return QStringLiteral("knotItem"); }

        void addKnot(QString type);
        void addKnot(QString type, int index);

        int bspline_n;
        int bspline_cpnum;
        KnotSequences getKnotSequence();

    signals:
        void knotChanged(BaseFixedKnotSequence* knot);

    protected slots:
        void currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);

    protected:
        Bspline * bspline;
        vector<BaseFixedKnotSequence*> knots;
        BaseFixedKnotSequence* dragging_knot;

        bool is_sorting = false;
        QModelIndex sorting_modelindex_start;
        int sorting_index_start;

        void dragEnterEvent(QDragEnterEvent *event) override;
        void dragMoveEvent(QDragMoveEvent *event) override;
        void dropEvent(QDropEvent *event) override;
        void dragLeaveEvent(QDragLeaveEvent *event) override;
        //void startDrag(Qt::DropActions supportedActions) override;
        void mousePressEvent(QMouseEvent *event) override;
        void mouseReleaseEvent(QMouseEvent *event) override;
        void mouseDoubleClickEvent( QMouseEvent * e) override;
        Qt::DropAction supportedDropActions();

        vector<BaseFixedKnotSequence*> getKnots();
        BaseFixedKnotSequence* getKnotFromName(QString name);
};

#endif // KNOTLISTDEST_H
