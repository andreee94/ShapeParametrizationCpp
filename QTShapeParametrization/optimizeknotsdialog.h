/********************************************************************************
 *                                                                              *
 *   Copyright (C) 2019 by Andreee94                                            *
 *                                                                              *
 *   SHAPE PARAMETRIZATION                                                      *
 *                                                                              *
 * This program is free software: you can redistribute it and/or modify         *
 * it under the terms of the GNU Affero General Public License as published by  *
 * the Free Software Foundation, either version 3 of the License, or            *
 * (at your option) any later version.                                          *
 *                                                                              *
 * This program is distributed in the hope that it will be useful,              *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of               *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                *
 * GNU Affero General Public License for more details.                          *
 *                                                                              *
 * You should have received a copy of the GNU Affero General Public License     *
 * along with this program. If not, see <http://www.gnu.org/licenses/>.         *
 *                                                                              *
 ********************************************************************************/


#ifndef OPTIMIZEKNOTSDIALOG_H
#define OPTIMIZEKNOTSDIALOG_H

#include "knotlistdest.h"
#include "knotlistsource.h"
#include "mychartview.h"

#include <QDialog>
#include <QScatterSeries>

namespace Ui {
class OptimizeKnotsDialog;
}

class OptimizeKnotsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit OptimizeKnotsDialog(QWidget *parent = nullptr);
    ~OptimizeKnotsDialog();

    KnotSequences getKnotSequence();
    void setKnotSequence(KnotSequences knotsequence);
    int getKnotsCount();

    int getNumCP() const;
    void setNumCP(int value);

    int getN() const;
    void setN(int value);

public slots:

    void knotSelectedChanged(BaseKnotSequence* knot);
    void knotAddedRemoved(BaseKnotSequence* knot);

private:
    QWidget *generateKnotChartLayout();
    QLayout *generateMainLayout();

    int updateKnotSeries();
    int updateKnotCount();
    int updateFixedKnotProps(Knots knots);
    int updateFixedKnotProps(KnotSequences knotsequence);
    int updateFixedKnotProps();

    QScatterSeries *knotsSeries;
    ChartView *knotsChartView;
    QFormLayout *propLayout;
    QList<QWidget*> propLayoutListWidgets;
    KnotListDest *listActiveKnots;
    KnotListSource *listAllKnots;
    QVBoxLayout *listAllProps;
    QWidget *knotCountOKWidget;
    QLabel *knotCountLabel;

    int n;
    int numCP;
};

#endif // OPTIMIZEKNOTSDIALOG_H
