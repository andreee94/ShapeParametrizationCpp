#ifndef QTUTILS_H
#define QTUTILS_H

#include <QDialog>
#include <QTableView>
#include <QLayoutItem>
#include <QtWidgets>
#include <QtCharts>
#include <QLayoutItem>
#include <vector>
#include "Bspline.h"
#include "Utils.h"

class QTUtils
{
public:
    QTUtils();

    static QColor Red() { return QColor("#d84315"); }
    static QColor Blue() { return QColor("#1e88e5"); } //2196f3
    static QColor Brown() { return QColor("#795548"); }
    static QColor Orange() { return QColor("#f57c00"); }
    static QColor Green() { return QColor("#43a047"); }

    static QString color2str(QColor color);

    static QLayoutItem* separator();
    static void appendPointsToSeries(QLineSeries *series, const Points &points);
};

#endif // QTUTILS_H
