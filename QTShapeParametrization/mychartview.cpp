#include "mychartview.h"
#include <QtGui/QMouseEvent>
#include <QApplication>
#include "cmath"
#include <QtWidgets>
#include <QtCharts>
#include <QLayoutItem>
#include <QGraphicsSimpleTextItem>
#include "Utils.h"

using namespace QtCharts;

ChartView::~ChartView()
{
}

ChartView::ChartView(QWidget *parent)
    : QChartView(new QChart(), parent),
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

    if (event->button() == Qt::LeftButton && rubberBand)
    {
        startingDragPoint = event->pos();
        rubberBand = new QRubberBand(QRubberBand::Shape::Rectangle, this);
        rubberBand->setGeometry(QRect(startingDragPoint, QSize()).normalized()); // init empty rectangle with origin in starting point
        rubberBand->setEnabled(true);
        rubberBand->show();
    }

    manageClickedMap(event);
    manageClickedList(event);
    QChartView::mousePressEvent(event);
}

void ChartView::mouseMoveEvent(QMouseEvent *event)
{
    // pan the chart with a middle mouse drag
    if (canScroll())
        if (event->buttons() & Qt::MiddleButton)
        {
            auto dPos = event->pos() - m_lastMousePos;
            if (canScrollHorizontal())
                chart()->scroll(-dPos.x(), 0);
            if (canScrollVertical())
                chart()->scroll(0, dPos.y());

            m_lastMousePos = event->pos();
            event->accept();
        }
    if (showTooltip())
        manageTooltip(event);
    if (rubberBand && rubberBand->isVisible())// && event->button() & Qt::LeftButton)
    {
        QRect zoomrect = QRect(startingDragPoint, event->pos()).normalized();
        if (!canZoomHorizontal())
        {
            zoomrect.setX(chart()->plotArea().x());
            zoomrect.setRight(chart()->plotArea().right());
        }
        if (!canZoomVertical())
        {
            zoomrect.setY(chart()->plotArea().y());
            zoomrect.setBottom(chart()->plotArea().bottom());
        }
        rubberBand->setGeometry(zoomrect);
    }
    QChartView::mouseMoveEvent(event);
}

void ChartView::mouseReleaseEvent(QMouseEvent *event)
{
    QApplication::restoreOverrideCursor();

    if (rubberBand && rubberBand->isVisible() && event->button() == Qt::LeftButton)
    {
        if (QLineF(event->pos(), startingDragPoint).length() > 10) // zoom only if region is sufficiently big
        {
            chart()->zoomIn(rubberBand->geometry());
        }
        rubberBand->hide();
    }
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
        if (canZoom())
            chart()->zoomIn();
        break;
    case Qt::Key_Minus:
        if (canZoom())
            chart()->zoomOut();
        break;
    case Qt::Key_Left:
        if (canScrollHorizontal())
            chart()->scroll(10, 0);
        break;
    case Qt::Key_Right:
        if (canScrollHorizontal())
            chart()->scroll(-10, 0);
        break;
    case Qt::Key_Up:
        if (canScrollVertical())
            chart()->scroll(0, -10);
        break;
    case Qt::Key_Down:
        if (canScrollVertical())
            chart()->scroll(0, 10);
        break;
    case Qt::Key_0:
    case 96: //numpad 0
        //chart()->resetTransform();
        //chart()->resetMatrix();
        //chart()->createDefaultAxes();
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
            if(canZoomVertical())
                r.setHeight(r.height()*val);
            if (canZoomHorizontal()) {
                r.setWidth(r.width()*val);
            }
            r.moveCenter(c);
            chart()->zoomIn(r);
        }
    QChartView::wheelEvent(event);
}

void ChartView::manageTooltip(QMouseEvent * event)
{
    if (popup)
    {
        bool foundsomething = false;
        //QPointF mousePoint = chart()->mapToValue(event->pos());
        if (popup_map.size() > 0)
        {
            QPointF cursor_px = event->pos();
            QPointF cursor_xy = chart()->mapToPosition(cursor_px);

            for( auto const& [series, gettext] : popup_map )
            {
                if (series->isVisible())
                {
                    //QList<QPointF> points = qobject_cast<QScatterSeries*>(chart()->series().at(0))->points();
                    QList<QPointF> points = series->points();

                    for (auto const [index, point_xy] : Utils::enumerate(points))
                    {
                        QPointF point_px = chart()->mapToPosition(point_xy);
                        if (QLineF(cursor_px, point_px).length() < popupDistanceThresholdPX) // less than 10 px
                        {
                              QString text = (*gettext)(popup, point_px, point_xy, cursor_px, cursor_xy, index);
                              popup->setText(text);
                              proxy->setPos(point_px - QPointF(popup->width() / 2, 10 + popup->height()));
                              foundsomething = true;
                        }
                    }
                }
            }
        }
        proxy->widget()->setVisible(foundsomething);
    }
}

void ChartView::manageClickedMap(QMouseEvent *event)
{
    if (clickable_map.size() > 0)
    {
        if (event->buttons() & Qt::LeftButton)
        {
            QPointF cursor_px = event->pos();
            //QPointF cursor_xy = chart()->mapToPosition(cursor_px);

            for( auto const& [series1, series2] : clickable_map )
            {
                if (series1->isVisible() || series2->isVisible())
                {
                    //QList<QPointF> points = qobject_cast<QScatterSeries*>(chart()->series().at(0))->points();
                    QList<QPointF> points;
                    std::optional<QPointF> point;
                    bool from1to2 = false;
                    points = series1->points();
                    for (auto const [index, point1_xy] : Utils::enumerate(points))
                    {
                        QPointF point_px = chart()->mapToPosition(point1_xy);
                        if (QLineF(cursor_px, point_px).length() < clickableDistanceThresholdPX) // less than 10 px
                        {
                            point = point1_xy;
                            from1to2 = true; // found in series 1 so
                        }
                    }
                    if (!point) // not found in series1
                    {
                        points = series2->points();
                        for (auto const [index, point2_xy] : Utils::enumerate(points))
                        {
                            QPointF point_px = chart()->mapToPosition(point2_xy);
                            if (QLineF(cursor_px, point_px).length() < clickableDistanceThresholdPX) // less than 10 px
                            {
                                point = point2_xy;
                                from1to2 = false; // found in series 1 so
                            }
                        }
                    }
                    if (point) // click close to a point
                    {
                        if (from1to2)
                        {
                            series1->remove(point.value());
                            series2->append(point.value());
                        }
                        else
                        {
                            series1->append(point.value());
                            series2->remove(point.value());
                        }
                        emit clickableEventMap(series1, series2, point.value(), from1to2);
                    }
                }
            }
        }
    }
}

void ChartView::manageClickedList(QMouseEvent *event)
{
    if (clickable_list.size() > 0)
    {
        if (event->buttons() & Qt::LeftButton)
        {
            QPointF cursor_px = event->pos();
            //QPointF cursor_xy = chart()->mapToPosition(cursor_px);

            for( auto const& series : clickable_list )
            {
                if (series->isVisible())
                {
                    //QList<QPointF> points = qobject_cast<QScatterSeries*>(chart()->series().at(0))->points();
                    QList<QPointF> points;
                    std::optional<QPointF> point;
                    int index = -1;
                    points = series->points();
                    for (auto const [i, point_xy] : Utils::enumerate(points))
                    {
                        QPointF point_px = chart()->mapToPosition(point_xy);
                        if (QLineF(cursor_px, point_px).length() < clickableDistanceThresholdPX) // less than 10 px
                        {
                            point = point_xy;
                            index = i;
                            break; // points loop
                        }
                    }
                    if (point) // click close to a point
                    {
                        emit clickableEventList(series, point.value(), index);
                        break; // series loop
                    }
                }
            }
        }
    }
}


void ChartView::addTooltip(QScatterSeries * key, TooltipTextFunction * value)
{
    popup_map[key] = value;
}

void ChartView::addClickable(QScatterSeries * series)
{
    clickable_list.append(series);
}

void ChartView::addToggleable(QScatterSeries * key)
{
    // create an empty series for when toggling
    clickable_map[key] = new QScatterSeries;
}

void ChartView::addToggleable(QScatterSeries * key, QScatterSeries * value)
{
    // create an empty series for when toggling
    clickable_map[key] = value;
}


QRubberBand *ChartView::getRubberBand() const
{
    return rubberBand;
}

void ChartView::setRubberBand(QRubberBand *value)
{
    rubberBand = value;
}

int ChartView::getClickableDistanceThresholdPX() const
{
    return clickableDistanceThresholdPX;
}

void ChartView::setClickableDistanceThresholdPX(int value)
{
    clickableDistanceThresholdPX = value;
}

int ChartView::getPopupDistanceThresholdPX() const
{
    return popupDistanceThresholdPX;
}

void ChartView::setPopupDistanceThresholdPX(int value)
{
    popupDistanceThresholdPX = value;
}

int ChartView::getPopupPrecision() const
{
    return popupPrecision;
}

void ChartView::setPopupPrecision(int value)
{
    popupPrecision = value;
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
    return mDirectionScroll != DirectionScroll::NotScroll;
}

bool ChartView::canScrollHorizontal() const
{
    return mDirectionScroll & HorizontalScroll;
}

bool ChartView::canScrollVertical() const
{
    return mDirectionScroll & VerticalScroll;
}

bool ChartView::canScrollBoth() const
{
    return mDirectionScroll & BothDirectionScroll;
}

bool ChartView::canZoom() const
{
    return mDirectionZoom != DirectionZoom::NotZoom;
}

bool ChartView::canZoomHorizontal() const
{
    return mDirectionZoom & HorizontalZoom;
}

bool ChartView::canZoomVertical() const
{
    return mDirectionZoom & VerticalZoom;
}

bool ChartView::canZoomBoth() const
{
    return mDirectionZoom & BothDirectionZoom;
}






