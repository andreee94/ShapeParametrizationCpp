#include "knotlabel.h"
#include "qtutils.h"
#include <QtGui>

KnotLabel::KnotLabel(const QString &text, QWidget *parent)
    : QLabel(text, parent)
{
    //setAutoFillBackground(false);
    setFrameShape(QFrame::Panel);
    setFrameShadow(QFrame::Raised);
    //setSelected(false);
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    setMinimumHeight(28);
    setAlignment(Qt::AlignCenter);
    setMargin(0);
    setFocusPolicy(Qt::FocusPolicy::NoFocus);
    this->style()->unpolish(this);
    this->style()->polish(this);
    setStyleSheet("");
    //setAttribute(Qt::WA_TranslucentBackground);
}

void KnotLabel::setSelected(bool selected)
{
    if  (false)
    {
        setStyleSheet("");
        if (!selected)
        {
            QString colorstr = QTUtils::color2str(QTUtils::Blue());
            QString style = "";
            style += "QLabel {";
            style += "background: white;";
            //style += "background-clip: border;";
            style += "color : " + colorstr +";";
            style += "padding : 2px;";
            style += "margin-left : 8px;";
            style += "margin-right : 8px;";
            style += "border: 2px solid " + colorstr +";";
            style += "border-radius: 10px;";
            style += "}";
            style += "QListWidget::item:selected { selection-background-color: transparent; };";
            setStyleSheet(style);
        }
        else
        {
            QString colorstr = QTUtils::color2str(QTUtils::Blue());
            QString style = "";
            style += "QLabel {";
            style += "background : " + colorstr +";";
            //style += "background: white;";
            //style += "background-clip: border;";
            style += "color : " + colorstr +";";
            style += "color: white;";
            style += "padding : 2px;";
            style += "margin-left : 8px;";
            style += "margin-right : 8px;";
            style += "border: 2px solid " + colorstr +";";
            style += "border-radius: 10px;";
            style += "}";
            style += "QListWidget::item:selected { selection-background-color: transparent; };";
            setStyleSheet(style);
        }
    }
}
