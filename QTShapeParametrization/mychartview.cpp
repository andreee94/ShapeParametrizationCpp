#include "mychartview.h"
#include <QtGui/QMouseEvent>
#include <QApplication>
#include "cmath"
#include <QtWidgets>
#include <QtCharts>
#include <QLayoutItem>
#include <QGraphicsSimpleTextItem>

ChartView::ChartView(QChart *chart, QWidget *parent) :
    QChartView(chart, parent),
    m_isTouching(false)
{
    setDragMode(QGraphicsView::NoDrag);
    this->setMouseTracking(true);

    //setRubberBand(QChartView::RectangleRubberBand);
}

bool ChartView::viewportEvent(QEvent *event)
{
    if (event->type() == QEvent::TouchBegin) {
        // By default touch events are converted to mouse events. So
        // after this event we will get a mouse event also but we want
        // to handle touch events as gestures only. So we need this safeguard
        // to block mouse events that are actually generated from touch.
        m_isTouching = true;

        // Turn off animations when handling gestures they
        // will only slow us down.
        chart()->setAnimationOptions(QChart::NoAnimation);
    }
    return QChartView::viewportEvent(event);
}

void ChartView::mousePressEvent(QMouseEvent *event)
{
    if (canScroll())
        if (event->button() == Qt::MiddleButton)
        {
            QApplication::setOverrideCursor(QCursor(Qt::SizeAllCursor));
            m_lastMousePos = event->pos();
            event->accept();
        }

    QChartView::mousePressEvent(event);
}

void ChartView::mouseMoveEvent(QMouseEvent *event)
{
    // pan the chart with a middle mouse drag
    if (canScroll())
        if (event->buttons() & Qt::MiddleButton)
        {
            auto dPos = event->pos() - m_lastMousePos;
            chart()->scroll(-dPos.x(), dPos.y());

            m_lastMousePos = event->pos();
            event->accept();
        }
    if (showTooltip())
        manageTooltip(event);
    QChartView::mouseMoveEvent(event);
}

void ChartView::manageTooltip(QMouseEvent * event)
{
    if (popup)
    {
        bool foundsomething = false;
        //QPointF mousePoint = chart()->mapToValue(event->pos());
        if (chart()->series().size() > 0)
        {
            if (qobject_cast<QScatterSeries*>(chart()->series().at(0))) // series is made by points
            {
                QList<QPointF> points = qobject_cast<QScatterSeries*>(chart()->series().at(0))->points();
                for (QPointF point : points)
                {
                    if (QLineF(event->pos(), chart()->mapToPosition(point)).length() < 12) // less than 10 px
                    {
                        QString text;
                        text.append(popupShowX ? QString::number(point.x(), 'g', popupPrecision) : "");
                        text.append(popupShowX && popupShowY ? ", " : "");
                        text.append(popupShowY ? QString::number(point.y(), 'g', popupPrecision) : "");
                        popup->setText(text);
                        proxy->setPos(chart()->mapToPosition(point) - QPointF(popup->width() / 2, 10 + popup->height()));
                        foundsomething = true;
                    }
                }
            }
        }
        proxy->widget()->setVisible(foundsomething);
    }
}

int ChartView::getPopupPrecision() const
{
    return popupPrecision;
}

void ChartView::setPopupPrecision(int value)
{
    popupPrecision = value;
}

bool ChartView::getPopupShowY() const
{
    return popupShowY;
}

void ChartView::setPopupShowY(bool value)
{
    popupShowY = value;
}

bool ChartView::getPopupShowX() const
{
    return popupShowX;
}

void ChartView::setPopupShowX(bool value)
{
    popupShowX = value;
}

QLabel *ChartView::getPopup() const
{
    return popup;
}

void ChartView::setPopup(QLabel *value)
{
    this->popup = value;
    this->popup->setVisible(false);
    this->proxy = scene()->addWidget(popup);
}


void ChartView::mouseReleaseEvent(QMouseEvent *event)
{
    QApplication::restoreOverrideCursor();
    QChartView::mouseReleaseEvent(event);
}

void ChartView::keyPressEvent(QKeyEvent *event)
{
    if (!canUseKeys())
    {
        QGraphicsView::keyPressEvent(event);
        return;
    }
    switch (event->key()) {
    case Qt::Key_Plus:
        chart()->zoomIn();
        break;
    case Qt::Key_Minus:
        chart()->zoomOut();
        break;
    case Qt::Key_Left:
        chart()->scroll(10, 0);
        break;
    case Qt::Key_Right:
        chart()->scroll(-10, 0);
        break;
    case Qt::Key_Up:
        chart()->scroll(0, -10);
        break;
    case Qt::Key_Down:
        chart()->scroll(0, 10);
        break;
    case Qt::Key_0:
    case 96: //numpad 0
        chart()->resetTransform();
        chart()->resetMatrix();
        chart()->createDefaultAxes();
        chart()->zoomReset();
        break;
    default:
        QGraphicsView::keyPressEvent(event);
        break;
    }
}

void ChartView::wheelEvent(QWheelEvent *event)
{
    if (canZoom())
        if(chart() && mDirectionZoom != NotZoom)
        {
            const qreal factor = 1.001;
            QRectF r = chart()->plotArea();
            QPointF c = r.center();
            qreal val = std::pow(factor, event->delta());
            if(mDirectionZoom & VerticalZoom)
                r.setHeight(r.height()*val);
            if (mDirectionZoom & HorizontalZoom) {
                r.setWidth(r.width()*val);
            }
            r.moveCenter(c);
            chart()->zoomIn(r);
        }
    QChartView::wheelEvent(event);
}

bool ChartView::showTooltip() const
{
    return mShowTooltip;
}

void ChartView::setShowTooltip(bool showTooltip)
{
    mShowTooltip = showTooltip;
}

bool ChartView::canUseKeys() const
{
    return mCanUseKeys;
}

void ChartView::setCanUseKeys(bool canUseKeys)
{
    mCanUseKeys = canUseKeys;
}

bool ChartView::canScroll() const
{
    return mCanScroll;
}

void ChartView::setCanScroll(bool canScroll)
{
    mCanScroll = canScroll;
}

bool ChartView::canZoom() const
{
    return mCanZoom;
}

void ChartView::setCanZoom(bool canZoom)
{
    mCanZoom = canZoom;
}
