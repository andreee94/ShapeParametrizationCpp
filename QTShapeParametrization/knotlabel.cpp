#include "knotlabel.h"
#include <QtGui>

KnotLabel::KnotLabel(const QString &text, QWidget *parent)
    : QLabel(text, parent)
{
    setAutoFillBackground(true);
    setFrameShape(QFrame::Panel);
    setFrameShadow(QFrame::Raised);
}
