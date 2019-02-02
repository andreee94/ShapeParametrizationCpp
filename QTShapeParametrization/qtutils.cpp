#include "qtutils.h"

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
