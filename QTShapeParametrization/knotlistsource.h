#ifndef KNOTLISTSOURCE_H
#define KNOTLISTSOURCE_H



#include <QListWidget>

class KnotListSource : public QListWidget
{
    Q_OBJECT
    public:
        KnotListSource();
        static QString knotMimeType() { return QStringLiteral("knotItem"); }

        void addKnot(QString type);

    protected:
        void dragEnterEvent(QDragEnterEvent *event) override;
        void dragMoveEvent(QDragMoveEvent *event) override;
        void dropEvent(QDropEvent *event) override;
        void startDrag(Qt::DropActions supportedActions) override;
        void mousePressEvent(QMouseEvent *event) override;
};

#endif // KNOTLISTSOURCE_H
