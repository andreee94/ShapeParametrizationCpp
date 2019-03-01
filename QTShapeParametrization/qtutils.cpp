#include "qtutils.h"
#include "Utils.h"

QTUtils::QTUtils()
{

}

QString QTUtils::color2str(QColor color)
{
    return QString("rgb(").append(QString::number(color.red())).append(", ").append(QString::number(color.green())).append(", ").append(QString::number(color.blue())).append(")");
}

QLayoutItem* QTUtils::separator()
{
    QSpacerItem* item = new QSpacerItem(10, 10, QSizePolicy::Minimum, QSizePolicy::Expanding);
    return item;
//    QFrame* line = new QFrame();
//    line->setFrameShape(QFrame::HLine);
//    line->setFrameShadow(QFrame::Sunken);
    //    return line;
}


void QTUtils::appendPointsToSeries(QLineSeries *series, const Points &points)
{
    for (auto p : points)
    {
        series->append(p.getx(), p.gety());
    }
}


void QTUtils::appendPointsToSeries(QScatterSeries *series, const Points &points)
{
    for (auto p : points)
    {
        series->append(p.getx(), p.gety());
    }
}

void QTUtils::resetAxes(QChart *chart, QList<QPointF> points, double margin)
{
    QPointF maxpoint = QPointF(INT_MIN, INT_MIN);
    QPointF minpoint = QPointF(INT_MAX, INT_MAX);
    for (QPointF p : points)
    {
        if (p.x() < minpoint.x())
            minpoint.setX(p.x());
        if (p.y() < minpoint.y())
            minpoint.setY(p.y());
        if (p.x() > maxpoint.x())
            maxpoint.setX(p.x());
        if (p.y() > maxpoint.y())
            maxpoint.setY(p.y());
    }
    cout << "minX: " << minpoint.x() << " maxX: " << maxpoint.x() << endl;
    cout << "minY: " << minpoint.y() << " maxY: " << maxpoint.y() << endl;
    chart->axisX()->setRange(minpoint.x() - margin * (maxpoint.x() - minpoint.x()), maxpoint.x() + margin * (maxpoint.x() - minpoint.x()));
    chart->axisY()->setRange(minpoint.y() - margin * (maxpoint.y() - minpoint.y()), maxpoint.y() + margin * (maxpoint.y() - minpoint.y()));
}

void QTUtils::resetAxes(QChart *chart, vector<Point> points, double margin)
{
    QPointF maxpoint = QPointF(INT_MIN, INT_MIN);
    QPointF minpoint = QPointF(INT_MAX, INT_MAX);
    for (Point p : points)
    {
        if (p.getx() < minpoint.x())
            minpoint.setX(p.getx());
        if (p.gety() < minpoint.y())
            minpoint.setY(p.gety());
        if (p.getx() > maxpoint.x())
            maxpoint.setX(p.getx());
        if (p.gety() > maxpoint.y())
            maxpoint.setY(p.gety());
    }
    cout << "minX: " << minpoint.x() << " maxX: " << maxpoint.x() << endl;
    cout << "minY: " << minpoint.y() << " maxY: " << maxpoint.y() << endl;
    chart->axisX()->setRange(minpoint.x() - margin * (maxpoint.x() - minpoint.x()), maxpoint.x() + margin * (maxpoint.x() - minpoint.x()));
    chart->axisY()->setRange(minpoint.y() - margin * (maxpoint.y() - minpoint.y()), maxpoint.y() + margin * (maxpoint.y() - minpoint.y()));
}


void QTUtils::setRangeIfLarger(QValueAxis *axis, double min, double max)
{
    min = std::min(axis->min(), min);
    max = std::max(axis->max(), max);
    axis->setRange(min, max);
}

void QTUtils::clearLayout(QLayout *layout)
{
    QLayoutItem *item;
    while((item = layout->takeAt(0))) {
        if (item->layout()) {
            clearLayout(item->layout());
            delete item->layout();
        }
        if (item->widget()) {
            delete item->widget();
        }
        delete item;
    }
}
