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


#include "mychart.h"
#include "optimizeknotsdialog.h"
#include "qtutils.h"

#include <QVBoxLayout>

OptimizeKnotsDialog::OptimizeKnotsDialog(QWidget *parent) :
    QDialog(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    mainLayout->addLayout(generateMainLayout());
    mainLayout->addWidget(generateKnotChartLayout());

    // choice of x and y
//    QHBoxLayout *chartLayout = new QHBoxLayout;
//    chartLayout->addWidget(generateChartLayout(), 1);
//    chartLayout->addWidget(generateTabLayout(), 0);

//    mainLayout->addLayout(generateLoadLayout(), Qt::AlignTop);
//    mainLayout->addItem(QTUtils::separator());
//    mainLayout->addWidget(generateTableLayout());
//    mainLayout->addLayout(chartLayout, 1);
//    //mainLayout->addStretch(1);
//    mainLayout->addLayout(generateBottomLayout(), Qt::AlignRight | Qt::AlignBottom);
    setLayout(mainLayout);
    setWindowTitle(tr("Optimize the Knot Sequence"));
    resize(1024, 560);
}

OptimizeKnotsDialog::~OptimizeKnotsDialog()
{
}


// //////////////////////////////////////////////////////////////////
// //////////////////////////////////////////////////////////////////

QWidget* OptimizeKnotsDialog::generateKnotChartLayout()
{
//    MultiSliderView *multislider = new MultiSliderView;
//    multislider->SetRange(0,1);
//    multislider->SetValues(KnotSequences::getCompleteBirationalFixedKS(3, 21, 1.2, 1.0/1.2, 0.7).getSequence({}));

    knotsSeries = new QScatterSeries();
    knotsSeries->setMarkerSize(10);
    knotsSeries->setColor(QColor(0x1E, 0x90, 0xFF)); //series->pen().color());
    knotsSeries->setBorderColor(QColor(0x1E, 0x90, 0xFF)); //series->pen().color());


    Chart *chart = new Chart();
       chart->legend()->hide();
       chart->addSeries(knotsSeries);
       chart->createDefaultAxes();
       chart->axisY()->setMin(-0.05);
       //chart->axisY()->setMax(0.05 * (maxmultiplicity + 1));
       chart->removeAxis(chart->axisY());
       chart->setMargins(QMargins(2, 2, 2, 2));
       chart->axisX()->setRange(0-0.05, 1+0.05);
       ((QValueAxis*)chart->axisX())->setLabelFormat("%1.2f");
       //((QValueAxis*)chart->axisX())->applyNiceNumbers();
       //((QValueAxis*)chart->axisX())->setTickType(QValueAxis::TickType::TicksDynamic);
       ((QValueAxis*)chart->axisX())->setTickCount(23);
    knotsChartView = new ChartView();
        knotsChartView->setChart(chart);
        knotsChartView->setRenderHint(QPainter::Antialiasing);
        //knotsChartView->resize(knotsChartView->width(), max(2*10+30+14*maxmultiplicity, 2*10+30+2*14));
        //knotsChartView->setFixedHeight(max(2*10+30+14*maxmultiplicity, 2*10+30+2*14));
        knotsChartView->setDirectionZoom(ChartView::NotZoom);
        knotsChartView->setDirectionScroll(ChartView::NotScroll);
        knotsChartView->setCanUseKeys(false);
        knotsChartView->setShowTooltip(true);

    int maxmultiplicity = updateKnotSeries();

    QColor blue = QColor(0x1E, 0x90, 0xFF);
    QString bluestr = QString("rgb(").append(QString::number(blue.red())).append(", ").append(QString::number(blue.green())).append(", ").append(QString::number(blue.blue())).append(")");
    QLabel *popup = new QLabel("       ");
    popup->setAlignment(Qt::AlignCenter);
    popup->setStyleSheet("QLabel {background: white; color : " + bluestr +"; padding : 4px; border: 2px solid " + bluestr +"; border-radius: 10px;}");
    knotsChartView->setPopup(popup);
    knotsChartView->addTooltip(knotsSeries, new XTooltipText(3));
    return knotsChartView;
}

QLayout *OptimizeKnotsDialog::generateMainLayout()
{
    QGridLayout *gridLayout = new QGridLayout;
    QLabel *labelActiveKnots = new QLabel("Knot List");
    QLabel *labelAllKnots = new QLabel("All Knots");
    listActiveKnots = new KnotListDest;
    listAllKnots = new KnotListSource;
    listActiveKnots->bspline_n = getN();
    listActiveKnots->bspline_cpnum = getNumCP();
    QStringList listknotnames = {"Begin Knots", "End Knots", "Rational Knots", "Value Knot", "Birational Knots", "Uniform Knots", "Multiplicity Knots"};
    for (int i = 0; i < listknotnames.size(); i++)
    {
        listAllKnots->addKnot(listknotnames[i]);
    }

    QWidget* empty = new QWidget();
    empty->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    empty->setMinimumWidth(180);
    //QVBoxLayout *formContainerLayout = new QVBoxLayout;
    QGroupBox *propGroupBox = new QGroupBox(tr("Parameters"));
    propLayout = new QFormLayout;
    propLayout->addRow(new QLabel("Start"), new QLineEdit);
    propLayout->addRow(new QLabel("End"), new QLineEdit);
    propLayout->addRow(empty);
    propLayout->setFormAlignment(Qt::AlignRight);
    propGroupBox->setLayout(propLayout);
    //formContainerLayout->addLayout(propLayout);
    //formContainerLayout->addStretch(10);

    listActiveKnots->setMinimumWidth(160);
    listActiveKnots->setMaximumWidth(240);
    listActiveKnots->setMinimumHeight(211);
    listAllKnots->setMinimumWidth(160);
    listAllKnots->setMaximumWidth(240);
    listAllKnots->setMinimumHeight(211);
    connect(listActiveKnots, &KnotListDest::knotChanged, this, &OptimizeKnotsDialog::knotSelectedChanged);
    connect(listActiveKnots, &KnotListDest::knotAdded, this, &OptimizeKnotsDialog::knotAddedRemoved);
    connect(listActiveKnots, &KnotListDest::knotRemoved, this, &OptimizeKnotsDialog::knotAddedRemoved);

    knotCountLabel = new QLabel;
    QLabel *knotCountLabelLabel = new QLabel("(CP<sub>num</sub> + Degree + 1 = Knots<sub>num</sub>)");
    knotCountOKWidget = new QWidget;
    //knotCountOKWidget->setFixedSize(20, 20);
    //knotCountOKWidget->setStyleSheet("QWidget {background: " + QTUtils::color2str(QTUtils::Red()) +"; padding : 4px; border: 2px solid " + QTUtils::color2str(QTUtils::Red()) +"; border-radius: " + QString::number(10) + "px;}");
    QHBoxLayout *knotHBox = new QHBoxLayout;
    knotHBox->addWidget(knotCountOKWidget);
    knotHBox->addWidget(knotCountLabel);
    knotHBox->addWidget(knotCountLabelLabel);
    updateKnotCount();

    QGroupBox *allPropsGroupBox = new QGroupBox(tr("Parameters to Optimize"));
    QVBoxLayout *listAllPropsOut = new QVBoxLayout; // todo add checkboxes
    listAllProps = new QVBoxLayout; // todo add checkboxes
    QScrollArea *scrollLayout = new QScrollArea;
    QWidget *widgetscroll = new QWidget();
    widgetscroll->setLayout(listAllProps);
    scrollLayout->setWidget(widgetscroll);
    scrollLayout->setWidgetResizable(true);
//    scrollLayout->setMinimumHeight(300);
    scrollLayout->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    listAllPropsOut->addWidget(scrollLayout);
    allPropsGroupBox->setLayout(listAllPropsOut);

    gridLayout->addWidget(labelActiveKnots, 0, 0, 1, 1);
    gridLayout->addWidget(labelAllKnots, 0, 1, 1, 1);

    gridLayout->addWidget(listActiveKnots, 1, 0, 1, 1);
    gridLayout->addWidget(listAllKnots, 1, 1, 1, 1);
    gridLayout->addWidget(allPropsGroupBox, 0, 4, 4, 2);

    gridLayout->addWidget(propGroupBox, 0, 2, 4, 2);
    gridLayout->addLayout(knotHBox, 3, 0, 1, 2);
    return gridLayout;
}

// //////////////////////////////////////////////////////////////////
// //////////////////////////////////////////////////////////////////

void OptimizeKnotsDialog::knotSelectedChanged(BaseKnotSequence *knot)
{
    if (knot && knotsSeries)
    {
        QPushButton *sendBtn = new QPushButton("Apply");
        QTUtils::clearLayout(propLayout);
        propLayoutListWidgets.clear();
        vector<std::variant<int, double, bool>> values = knot->getValues();
        for (unsigned int i = 0; i < knot->propNames().size(); i++)
        {
            string name = knot->propName(i);
            BaseKnotSequence::ParamType type = knot->propType(i);
            QWidget *qwidget;
            if (knot->propType(i) == BaseKnotSequence::ParamType::INT || knot->propType(i) == BaseKnotSequence::ParamType::DOUBLE)
            {
                QLineEdit *qlineedit = new QLineEdit;
                QString value_str = "";
                if (type == BaseKnotSequence::ParamType::INT)
                {
                    value_str = QString::number(std::get<int>(values[i]));
                    qlineedit->setValidator( new QIntValidator(this));
                }
                else if (type == BaseKnotSequence::ParamType::DOUBLE)
                {
                    value_str = QString::number(std::get<double>(values[i]));
                    qlineedit->setValidator( new QDoubleValidator(this));
                }
                qlineedit->setText(value_str);
                connect(qlineedit, &QLineEdit::returnPressed, sendBtn, &QPushButton::click);
                qwidget = qlineedit;
            }
            else if (knot->propType(i) == BaseKnotSequence::ParamType::BOOL)
            {
                QCheckBox *qcheckbox = new QCheckBox;
                qcheckbox->setChecked(std::get<bool>(values[i]));
                qcheckbox->setLayoutDirection(Qt::LayoutDirection::RightToLeft);
                connect(qcheckbox, &QCheckBox::stateChanged, sendBtn, &QPushButton::click);
                qwidget = qcheckbox;
            }
            propLayout->addRow(new QLabel(QString::fromStdString(name)), qwidget);
            propLayoutListWidgets.push_back(qwidget);
        }
        QWidget* empty = new QWidget();
        empty->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        empty->setMinimumWidth(180);
        propLayout->addWidget(empty);
        propLayout->addWidget(sendBtn);
        this->updateKnotSeries();
        this->updateKnotCount();

        connect(sendBtn, &QPushButton::clicked, this, [this, knot] () {
            vector<std::variant<int, double, bool>> values;
            for (size_t i = 0; i < knot->propNames().size(); i++)
            {
                if (knot->propType(i) == BaseKnotSequence::ParamType::INT || knot->propType(i) == BaseKnotSequence::ParamType::DOUBLE)
                {
                    QString text = static_cast<QLineEdit*>(propLayoutListWidgets.at(i))->text(); //static_cast<QLineEdit>(propLayout->itemAt(i, QFormLayout::FieldRole)->widget()).text();
                    cout << "text: " << text.toStdString() << endl;
                    if (knot->propType(i) == BaseKnotSequence::ParamType::INT)
                    {
                        values.push_back(text.toInt());
                        //cout << "value: " << text.toInt() << endl;
                    }
                    else if (knot->propType(i) == BaseKnotSequence::ParamType::DOUBLE)
                    {
                        values.push_back(text.toDouble());
                        //cout << "value: " << text.toDouble() << endl;
                    }
                }
                else if (knot->propType(i) == BaseKnotSequence::ParamType::BOOL)
                {
                    bool checked = static_cast<QCheckBox*>(propLayoutListWidgets.at(i))->isChecked();
                    values.push_back(checked);
                }
            }
            knot->setValues(values);
            this->updateKnotSeries();
            this->updateKnotCount();
        });
    }
}

void OptimizeKnotsDialog::knotAddedRemoved(BaseKnotSequence *knot)
{
    updateFixedKnotProps();
}

// //////////////////////////////////////////////////////////////////
// //////////////////////////////////////////////////////////////////

int OptimizeKnotsDialog::updateKnotSeries()
{
    knotsSeries->clear();
    doubles uarray = getKnotSequence().getSequence({});
    int maxmultiplicity = 0;
    for (unsigned int i = 0; i < uarray.size(); i++)
    {
        int multiplicity = 0;
        int j = i - 1;
        while (j >= 0 && Utils::eq(uarray[i], uarray[j]))
        {
            j--;
            multiplicity++;
        }
        knotsSeries->append(uarray[i], 0.05 * multiplicity);
        maxmultiplicity = max(multiplicity, maxmultiplicity);
    }
    if (knotsChartView)
    {
        knotsChartView->setFixedHeight(max(2*10+30+14*maxmultiplicity+14, 2*10+30+2*14));
        //knotsChartView->resize(knotsChartView->width(), max(2*10+30+14*maxmultiplicity, 2*10+30+2*14));
        knotsChartView->chart()->createDefaultAxes();
        knotsChartView->chart()->axisX()->setRange(0-0.05, 1+0.05);
        ((QValueAxis*)knotsChartView->chart()->axisX())->setLabelFormat("%1.2f");
        ((QValueAxis*)knotsChartView->chart()->axisX())->setTickCount(23);
        knotsChartView->chart()->axisY()->setMin(-0.05);
        knotsChartView->chart()->axisY()->setMax(0.05 * (maxmultiplicity + 1));
        knotsChartView->chart()->removeAxis(knotsChartView->chart()->axisY());
    }
    return maxmultiplicity;
}

int OptimizeKnotsDialog::updateKnotCount()
{
    int numcp = getNumCP();
    int n = getN();
    int knotscount = getKnotsCount();
    knotCountLabel->setText(QString::number(numcp) + " + " + QString::number(n) + " + 1 = " + QString::number(knotscount));
    bool are_equal = numcp + n + 1 == knotscount;
    QColor color = are_equal ? QTUtils::Green() : QTUtils::Red();
    knotCountOKWidget->setFixedSize(20, 20);
    knotCountOKWidget->setStyleSheet("QWidget {background: " + QTUtils::color2str(color) +"; padding : 4px; border: 2px solid " + QTUtils::color2str(color) +"; border-radius: " + QString::number(10) + "px;}");
}

int OptimizeKnotsDialog::updateFixedKnotProps(Knots knots)
{
    QTUtils::clearLayout(listAllProps);
    for (auto knot : knots)
    {
        listAllProps->addWidget(new QLabel("- " + QString::fromStdString(knot->type())), Qt::AlignCenter);
        for (size_t i = 0; i < knot->propsCount(); i++)
        {
            QCheckBox *checkBox = new QCheckBox(QString::fromStdString(knot->propName(i)));
            listAllProps->addWidget(checkBox);
        }
        listAllProps->addItem(QTUtils::separator());
    }
}

int OptimizeKnotsDialog::updateFixedKnotProps(KnotSequences knotsequence)
{
    updateFixedKnotProps(knotsequence.getKnots());
}

int OptimizeKnotsDialog::updateFixedKnotProps()
{
    updateFixedKnotProps(getKnotSequence());
}

// //////////////////////////////////////////////////////////////////
// //////////////////////////////////////////////////////////////////

KnotSequences OptimizeKnotsDialog::getKnotSequence()
{
    return listActiveKnots->getKnotSequence();
}

void OptimizeKnotsDialog::setKnotSequence(KnotSequences knotsequence)
{
    listActiveKnots->setKnots(knotsequence.getKnots());
    updateFixedKnotProps(knotsequence);
}

int OptimizeKnotsDialog::getKnotsCount()
{
    return getKnotSequence().getSequence({}).size();
}

int OptimizeKnotsDialog::getN() const
{
    return n;
}

void OptimizeKnotsDialog::setN(int value)
{
    n = value;
}

int OptimizeKnotsDialog::getNumCP() const
{
    return numCP;
}

void OptimizeKnotsDialog::setNumCP(int value)
{
    numCP = value;
}

// //////////////////////////////////////////////////////////////////
// //////////////////////////////////////////////////////////////////
