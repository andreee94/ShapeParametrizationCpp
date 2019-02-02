#ifndef KNOTLABEL_H
#define KNOTLABEL_H

#include <QtGui>
#include <QMainWindow>
#include <QObject>
#include <QWidget>
#include <QLabel>

class KnotLabel : public QLabel
{
    public:
        KnotLabel(const QString &text, QWidget *parent=0);
        void setSelected(bool selected);
};

#endif // KNOTLABEL_H
