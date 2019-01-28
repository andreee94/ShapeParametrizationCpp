#ifndef KNOTLISTDEST_H
#define KNOTLISTDEST_H

#include <QListWidget>

class KnotListDest : public QListWidget
{
    Q_OBJECT
    public:
        KnotListDest();
        static QString knotMimeType() { return QStringLiteral("knotItem"); }

        void addKnot(QString type);

    protected:
        void dragEnterEvent(QDragEnterEvent *event) override;
        void dragMoveEvent(QDragMoveEvent *event) override;
        void dropEvent(QDropEvent *event) override;
        void startDrag(Qt::DropActions supportedActions) override;
        void mousePressEvent(QMouseEvent *event) override;
};

#endif // KNOTLISTDEST_H
