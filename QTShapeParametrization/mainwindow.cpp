#include "MultiSliderView.h"
#include "loadprofiledialog.h"
#include "mainwindow.h"
#include "Bspline.h"
#include "rangesliderlayout.h"
//#include "ui_mainwindow.h"
#include <QtWidgets>
#include <QtCharts>
#include <QLayoutItem>
#include <vector>
//#include <QtQuickControls2>

typedef std::vector<Point> Points;
typedef std::vector<double> doubles;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
    //,ui(new Ui::MainWindow)
{
    QVBoxLayout *mainVLayout = new QVBoxLayout;
    QHBoxLayout *subMainHLayout = new QHBoxLayout;
    tabLayout = new QTabWidget;
        tabLayout->addTab(generateTabBspline(), tr("Bspline"));
        tabLayout->addTab(generateTabRanges(), tr("Ranges"));
        tabLayout->addTab(generateTabSettings(), tr("Settings"));

    subMainHLayout->addWidget(tabLayout, 0);
    subMainHLayout->addLayout(generateChartLayout(), 1);

    mainVLayout->addLayout(subMainHLayout);
    //mainVLayout->addWidget(multislider);
    mainVLayout->addWidget(generateKnotChartLayout());

    connect(tabLayout ,QOverload<int>::of(&QTabWidget::currentChanged),this, &MainWindow::tabChanged);

    QWidget *widget = new QWidget(this);
    setCentralWidget(widget);
    widget->setLayout(mainVLayout);
    setWindowTitle(tr("Interpolate profile with Bspline curves"));
    resize(1440, 900);
}

MainWindow::~MainWindow()
{
    //delete ui;
}



QWidget* MainWindow::generateTabBspline()
{
    QWidget *widget = new QWidget;
    QVBoxLayout *formContainerVLayout = new QVBoxLayout;
    QFormLayout *formLayout1 = new QFormLayout;
    QFormLayout *formLayout2 = new QFormLayout;
    QFormLayout *formLayout3 = new QFormLayout;
    QGroupBox *groupLayout1 = new QGroupBox(tr("Input data"));
    QGroupBox *groupLayout2 = new QGroupBox(tr("BSpline"));
    QGroupBox *groupLayout3 = new QGroupBox(tr("Trailing edge"));
    QHBoxLayout *buttonsHLayout = new QHBoxLayout;
    QGridLayout *updateMINMAX_TEgridLayout = new QGridLayout;

    QHBoxLayout *filenameLayout = new QHBoxLayout;
    bspineFilenameEdit = new QLineEdit("");
    bspineFilenameButton = new QPushButton("Choose");
        filenameLayout->addWidget(bspineFilenameEdit);
        filenameLayout->addWidget(bspineFilenameButton);

    bsplineNEdit = new QLineEdit("3");
    bsplineCPnumEdit = new QLineEdit("21");
    bsplineNumPointsEdit = new QLineEdit("1000");
    bsplineTEShapeCombo = new QComboBox;
    bsplineTEMotionCombo = new QComboBox;
    bsplineTENumPointsEdit = new QLineEdit("100");

        bsplineNEdit->setValidator( new QIntValidator(1, 10, this) );
        bsplineCPnumEdit->setValidator( new QIntValidator(5, 100, this) );
        bsplineNumPointsEdit->setValidator( new QIntValidator(10, 10000, this) );
        bsplineTEShapeCombo->addItems(QStringList{"Ellipse", "Circle", "None"});
        bsplineTEMotionCombo->addItems(QStringList{"Rigid", "First point", "Second point", "Second-Last point", "Last point", "Auto", "None"});
        bsplineTENumPointsEdit->setValidator( new QIntValidator(10, 1000, this) );

    buttonComputeCP = new QPushButton("Compute CP");
    buttonOptimizeKnots = new QPushButton("Optimize Knots");
    buttonComputeMINMAX = new QPushButton("Update");
    buttonComputeTE = new QPushButton("Update");
    checkBoxAutoMINMAX = new QCheckBox("Auto update min-max curves");
    checkBoxAutoTE = new QCheckBox("Auto update trailing edge");
        checkBoxAutoMINMAX->setCheckState(Qt::CheckState::Checked);
        checkBoxAutoTE->setCheckState(Qt::CheckState::Checked);
        checkBoxAutoMINMAXChanged(checkBoxAutoMINMAX->isChecked());
        checkBoxAutoTEChanged(checkBoxAutoTE->isChecked());

    formLayout1->addRow("File:", filenameLayout);
    //formLayout->addItem(separator());
    formLayout2->addRow("Control Points number:", bsplineCPnumEdit);
    formLayout2->addRow("Degree:", bsplineNEdit);
    formLayout2->addRow("Interpolation Points:", bsplineNumPointsEdit);
    //formLayout->addItem(separator());
    formLayout3->addRow("Trailing Edge shape:", bsplineTEShapeCombo);
    formLayout3->addRow("Trailing Edge points:", bsplineTENumPointsEdit);
    formLayout3->addRow("Trailing Edge motion:", bsplineTEMotionCombo);
    groupLayout1->setLayout(formLayout1);
    groupLayout2->setLayout(formLayout2);
    groupLayout3->setLayout(formLayout3);

    buttonsHLayout->addWidget(buttonComputeCP);
    buttonsHLayout->addWidget(buttonOptimizeKnots);
    updateMINMAX_TEgridLayout->addWidget(checkBoxAutoMINMAX, 0, 0, 1, 1);
    updateMINMAX_TEgridLayout->addWidget(buttonComputeMINMAX, 0, 1, 1, 1);
    updateMINMAX_TEgridLayout->addWidget(checkBoxAutoTE, 1, 0, 1, 1);
    updateMINMAX_TEgridLayout->addWidget(buttonComputeTE, 1, 1, 1, 1);

    formContainerVLayout->addWidget(groupLayout1);
    formContainerVLayout->addWidget(groupLayout2);
    formContainerVLayout->addWidget(groupLayout3);
    formContainerVLayout->addStretch(1);
    formContainerVLayout->addLayout(updateMINMAX_TEgridLayout);
    formContainerVLayout->addItem(separator());
    formContainerVLayout->addLayout(buttonsHLayout);
    widget->setLayout(formContainerVLayout);

    connect(bspineFilenameButton, &QPushButton::clicked, this, &MainWindow::loadFile);
    connect(buttonComputeCP, &QPushButton::clicked, this, &MainWindow::computeCP);
    connect(buttonOptimizeKnots, &QPushButton::clicked, this, &MainWindow::optimizeKnots);
    connect(buttonComputeMINMAX, &QPushButton::clicked, this, &MainWindow::updateMINMAX);
    connect(buttonComputeTE, &QPushButton::clicked, this, &MainWindow::updateTE);
    connect(checkBoxAutoMINMAX,QOverload<int>::of(&QCheckBox::stateChanged),this, &MainWindow::checkBoxAutoMINMAXChanged);
    connect(checkBoxAutoTE,QOverload<int>::of(&QCheckBox::stateChanged),this, &MainWindow::checkBoxAutoTEChanged);
    return widget;
}

QWidget* MainWindow::generateTabRanges()
{
    QVBoxLayout *boxVLayout = new QVBoxLayout;
    QScrollArea *scrollLayout = new QScrollArea;
    scrollLayout->setWidgetResizable(true);
    //QVBoxLayout *boxScrollVLayout = new QVBoxLayout;
    rangesContainerLayout = new QFormLayout;
    updateRangesWidgets();
    rangesContainerLayout->setLabelAlignment(Qt::AlignCenter);

    QWidget *widgetscroll = new QWidget();
    widgetscroll->setLayout(rangesContainerLayout);
    scrollLayout->setWidget(widgetscroll);
    scrollLayout->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    QHBoxLayout *minrangeLayout = new QHBoxLayout;
    minRangeAllEdit = new QLineEdit("-1");
    minRangeAllButton = new QPushButton("Set All");
    minrangeLayout->addWidget(minRangeAllEdit);
    minrangeLayout->addWidget(minRangeAllButton);

    QHBoxLayout *maxrangeLayout = new QHBoxLayout;
    maxRangeAllEdit = new QLineEdit("1");
    maxRangeAllButton = new QPushButton("Set All");
    maxrangeLayout->addWidget(maxRangeAllEdit);
    maxrangeLayout->addWidget(maxRangeAllButton);

    QHBoxLayout *minValueLayout = new QHBoxLayout;
    minValueAllEdit = new QLineEdit("-1");
    minValueAllButton = new QPushButton("Set All");
    minValueLayout->addWidget(minValueAllEdit);
    minValueLayout->addWidget(minValueAllButton);

    QHBoxLayout *maxValueLayout = new QHBoxLayout;
    maxValueAllEdit = new QLineEdit("1");
    maxValueAllButton = new QPushButton("Set All");
    maxValueLayout->addWidget(maxValueAllEdit);
    maxValueLayout->addWidget(maxValueAllButton);

    minRangeAllEdit->setAlignment(Qt::AlignRight);
    maxRangeAllEdit->setAlignment(Qt::AlignRight);
    minValueAllEdit->setAlignment(Qt::AlignRight);
    maxValueAllEdit->setAlignment(Qt::AlignRight);

    QGroupBox *setAllGroup = new QGroupBox;
    QFormLayout *setAllVFormLayout = new QFormLayout;
    setAllVFormLayout->addRow("Min Range:", minrangeLayout);
    setAllVFormLayout->addRow("Max Range:", maxrangeLayout);
    setAllVFormLayout->addRow("Min Value:", minValueLayout);
    setAllVFormLayout->addRow("Max Value:", maxValueLayout);
    setAllGroup->setLayout(setAllVFormLayout);

    boxVLayout->addWidget(scrollLayout, 0);
    //boxVLayout->addStretch(1);
    boxVLayout->addWidget(setAllGroup, 0, Qt::AlignBottom);

    connect(minRangeAllButton, &QPushButton::clicked, this, &MainWindow::applyAllMinRange);
    connect(maxRangeAllButton, &QPushButton::clicked, this, &MainWindow::applyAllMaxRange);
    connect(minValueAllButton, &QPushButton::clicked, this, &MainWindow::applyAllMinValue);
    connect(maxValueAllButton, &QPushButton::clicked, this, &MainWindow::applyAllMaxValue);

    QWidget *widget = new QWidget;
    widget->setLayout(boxVLayout);
    return widget;
}

QWidget* MainWindow::generateTabSettings()
{
    QVBoxLayout *formContainerVLayout = new QVBoxLayout;
    QFormLayout *formLayout1 = new QFormLayout;
    QGroupBox *groupLayout1 = new QGroupBox(tr("Machine Data"));

    machineHubRadius = new QLineEdit;
    machineTipRadius = new QLineEdit;
    machineType = new QComboBox;
    machineAnalysis = new QComboBox;

    machineType->addItems(QStringList{"Axial", "Radial"});
    machineAnalysis->addItems(QStringList{"Stator Only", "Rotor Only", "Stator-Rotor"});

    formLayout1->addRow("Machine Hub Radius", machineHubRadius);
    formLayout1->addRow("Machine Tip Radius", machineTipRadius);
    formLayout1->addRow("Machine Axis Type", machineType);
    formLayout1->addRow("Analysis Type", machineAnalysis);

    groupLayout1->setLayout(formLayout1);
    formContainerVLayout->addWidget(groupLayout1);
    formContainerVLayout->addStretch(1);
    QWidget *widget = new QWidget;
    widget->setLayout(formContainerVLayout);
    return widget;
}

QLayout* MainWindow::generateChartLayout()
{
    QVBoxLayout *rightLayout = new QVBoxLayout;

    QLineSeries *series = new QLineSeries();
    Chart *chart = new Chart();
       chart->legend()->hide();
       chart->addSeries(series);
       chart->createDefaultAxes();
    chartView = new ChartView(chart);
        chartView->setRenderHint(QPainter::Antialiasing);
        chartView->setDirectionZoom(ChartView::BothDirectionZoom);
        chartView->setRubberBand(QChartView::RectangleRubberBand);

    QGroupBox *groupCheckBox = new QGroupBox(tr("Plots visibility"));
    QGridLayout *checkBoxVLayout = new QGridLayout;
    checkBoxOriginalPoints = new QCheckBox("Original Points");
    checkBoxOriginalCurve = new QCheckBox("Original Curve");
    checkBoxInterpolatedPoints = new QCheckBox("Interpolated Points");
    checkBoxInterpolatedCurve = new QCheckBox("Interpolated Curve");
    checkBoxCP = new QCheckBox("Control Points");
    checkBoxTE = new QCheckBox("Trailing Edge");
    checkBoxMaxParams = new QCheckBox("Max parameters Curve");
    checkBoxMinParams = new QCheckBox("Min parameters Curve");
    checkBoxNormals = new QCheckBox("Normals in CP");
    checkBoxTangents = new QCheckBox("Tangents in CP");
    checkBoxVLayout->addWidget(checkBoxOriginalPoints, 0, 0, 1, 1);
    checkBoxVLayout->addWidget(checkBoxOriginalCurve, 1, 0, 1, 1);
    checkBoxVLayout->addWidget(checkBoxInterpolatedPoints, 0, 1, 1, 1);
    checkBoxVLayout->addWidget(checkBoxInterpolatedCurve, 1, 1, 1, 1);
    checkBoxVLayout->addWidget(checkBoxCP, 0, 2, 1, 1);
    checkBoxVLayout->addWidget(checkBoxTE, 1, 2, 1, 1);
    checkBoxVLayout->addWidget(checkBoxMaxParams, 0, 3, 1, 1);
    checkBoxVLayout->addWidget(checkBoxMinParams, 1, 3, 1, 1);
    checkBoxVLayout->addWidget(checkBoxNormals, 0, 4, 1, 1);
    checkBoxVLayout->addWidget(checkBoxTangents, 1, 4, 1, 1);

    checkBoxOriginalPoints->setCheckState(Qt::CheckState::Checked);
    checkBoxOriginalCurve->setCheckState(Qt::CheckState::Checked);
    checkBoxInterpolatedPoints->setCheckState(Qt::CheckState::Unchecked);
    checkBoxInterpolatedCurve->setCheckState(Qt::CheckState::Checked);
    checkBoxCP->setCheckState(Qt::CheckState::Checked);
    checkBoxTE->setCheckState(Qt::CheckState::Checked);
    checkBoxMaxParams->setCheckState(Qt::CheckState::Checked);
    checkBoxMinParams->setCheckState(Qt::CheckState::Checked);
    checkBoxNormals->setCheckState(Qt::CheckState::Checked);
    checkBoxTangents->setCheckState(Qt::CheckState::Unchecked);
    updateCheckBoxEnable();

    connect(checkBoxOriginalPoints,QOverload<int>::of(&QCheckBox::stateChanged),this, &MainWindow::checkBoxOriginalPointsChanged);
    connect(checkBoxOriginalCurve,QOverload<int>::of(&QCheckBox::stateChanged),this, &MainWindow::checkBoxOriginalCurveChanged);
    connect(checkBoxInterpolatedPoints,QOverload<int>::of(&QCheckBox::stateChanged),this, &MainWindow::checkBoxInterpolatedPointsChanged);
    connect(checkBoxInterpolatedCurve,QOverload<int>::of(&QCheckBox::stateChanged),this, &MainWindow::checkBoxInterpolatedCurveChanged);
    connect(checkBoxCP,QOverload<int>::of(&QCheckBox::stateChanged),this, &MainWindow::checkBoxCPChanged);
    connect(checkBoxTE,QOverload<int>::of(&QCheckBox::stateChanged),this, &MainWindow::checkBoxTEChanged);
    connect(checkBoxMaxParams,QOverload<int>::of(&QCheckBox::stateChanged),this, &MainWindow::checkBoxMaxParamsChanged);
    connect(checkBoxMinParams,QOverload<int>::of(&QCheckBox::stateChanged),this, &MainWindow::checkBoxMinParamsChanged);
    connect(checkBoxNormals,QOverload<int>::of(&QCheckBox::stateChanged),this, &MainWindow::checkBoxNormalsChanged);
    connect(checkBoxTangents,QOverload<int>::of(&QCheckBox::stateChanged),this, &MainWindow::checkBoxTangentsChanged);

    chartView->chart()->addSeries(series_maxParams);
    chartView->chart()->addSeries(series_minParams);
    chartView->chart()->addSeries(series_CP);
    chartView->chart()->addSeries(series_originalPoints);
    chartView->chart()->addSeries(series_interpolatePoints);
    chartView->chart()->addSeries(series_originalCurve);
    chartView->chart()->addSeries(series_TE);
    chartView->chart()->addSeries(series_interpolatedCurve);

    groupCheckBox->setLayout(checkBoxVLayout);
    rightLayout->addWidget(chartView);
    rightLayout->addWidget(groupCheckBox);
    return rightLayout;
}


QWidget* MainWindow::generateKnotChartLayout()
{
//    MultiSliderView *multislider = new MultiSliderView;
//    multislider->SetRange(0,1);
//    multislider->SetValues(KnotSequences::getCompleteBirationalFixedKS(3, 21, 1.2, 1.0/1.2, 0.7).getSequence({}));

    knotsSeries = new QScatterSeries();
    knotsSeries->setMarkerSize(10);
    knotsSeries->setColor(QColor(0x1E, 0x90, 0xFF)); //series->pen().color());
    knotsSeries->setBorderColor(QColor(0x1E, 0x90, 0xFF)); //series->pen().color());

    doubles values = getKnotSequence().getSequence({});
    int maxmultiplicity = updateKnotSeries(values);

    Chart *chart = new Chart();
       chart->legend()->hide();
       chart->addSeries(knotsSeries);
       chart->createDefaultAxes();
       chart->axisY()->setMin(-0.05);
       chart->axisY()->setMax(0.05 * (maxmultiplicity + 1));
       chart->removeAxis(chart->axisY());
       chart->setMargins(QMargins(2, 2, 2, 2));
       chart->axisX()->setRange(0-0.05, 1+0.05);
       ((QValueAxis*)chart->axisX())->setLabelFormat("%1.2f");
       //((QValueAxis*)chart->axisX())->applyNiceNumbers();
       //((QValueAxis*)chart->axisX())->setTickType(QValueAxis::TickType::TicksDynamic);
       ((QValueAxis*)chart->axisX())->setTickCount(23);
    knotsChartView = new ChartView(chart);
        knotsChartView->setRenderHint(QPainter::Antialiasing);
        knotsChartView->setFixedHeight(110);
        knotsChartView->setCanZoom(false);
        knotsChartView->setCanScroll(false);
        knotsChartView->setCanUseKeys(false);
        knotsChartView->setShowTooltip(true);


    QColor blue = QColor(0x1E, 0x90, 0xFF);
    QString bluestr = QString("rgb(").append(QString::number(blue.red())).append(", ").append(QString::number(blue.green())).append(", ").append(QString::number(blue.blue())).append(")");
    QLabel *popup = new QLabel("       ");
    popup->setAlignment(Qt::AlignCenter);popup->setStyleSheet("QLabel {background: white; color : " + bluestr +"; padding : 4px; border: 2px solid " + bluestr +"; border-radius: 10px;}");
    knotsChartView->setPopup(popup);
    knotsChartView->setPopupShowX(true);
    knotsChartView->setPopupShowY(false);
    knotsChartView->setPopupPrecision(3);
    return knotsChartView;
}


QLayoutItem* MainWindow::separator()
{
    QSpacerItem* item = new QSpacerItem(10, 10, QSizePolicy::Minimum, QSizePolicy::Expanding);
    return item;
//    QFrame* line = new QFrame();
//    line->setFrameShape(QFrame::HLine);
//    line->setFrameShadow(QFrame::Sunken);
    //    return line;
}

KnotSequences MainWindow::getKnotSequence()
{
    return KnotSequences::getCompleteBirationalFixedKS(getN(), getNumCP(), 1.2, 1.0/1.2, 0.7);
}



void MainWindow::updateChart()
{
    if (this->data.getFileName().size() > 0)
    {
        //QLineSeries *series = new QLineSeries();
        //QScatterSeries *seriesPoints = new QScatterSeries();
        series_originalCurve->clear();
        series_originalPoints->clear();
        Points points = data.getPoints();
        for (unsigned int i = 0; i < points.size(); i++)
        {
            series_originalCurve->append(points[i].getx(), points[i].gety());
            series_originalPoints->append(points[i].getx(), points[i].gety());
        }
        QPen pen =QPen(QColor(Qt::black));
        pen.setWidth(2);
        series_originalCurve->setPen(pen);
        series_originalPoints->setMarkerSize(6);
        series_originalPoints->setColor(Qt::black); //series->pen().color());
        series_originalPoints->setBorderColor(Qt::black); //series->pen().color());
        //chartView->chart()->removeAllSeries();
        if (!chartView->chart()->series().contains(series_originalCurve))
            chartView->chart()->addSeries(series_originalCurve);
        if (!chartView->chart()->series().contains(series_originalPoints))
            chartView->chart()->addSeries(series_originalPoints);
        chartView->chart()->createDefaultAxes();
        updateCheckBoxEnable();
        checkBoxOriginalCurveChanged(checkBoxOriginalCurve->isChecked());
        checkBoxOriginalPointsChanged(checkBoxOriginalPoints->isChecked());
    }
}

void MainWindow::updateBspline()
{
    if (this->data.getFileName().size() > 0 && bspline && bspline->getCParray().size() > 0)
    {
        series_CP->clear();
        series_interpolatePoints->clear();
        series_interpolatedCurve->clear();

        Points controlpoints = bspline->getCParray();
        for (unsigned int i = 0; i < controlpoints.size(); i++)
        {
            series_CP->append(controlpoints[i].getx(), controlpoints[i].gety());
        }

        Points points = bspline->evaluate(bsplineNumPointsEdit->text().toInt());
        for (unsigned int i = 0; i < points.size(); i++)
        {
            series_interpolatePoints->append(points[i].getx(), points[i].gety());
            series_interpolatedCurve->append(points[i].getx(), points[i].gety());
        }

        QPen pen =QPen(QColor(Blue));
        pen.setWidth(2);

        series_CP->setMarkerSize(10);
        series_CP->setColor(Red); //series->pen().color());
        series_CP->setBorderColor(Red); //series->pen().color());

        series_interpolatePoints->setMarkerSize(5);
        series_interpolatePoints->setColor(Blue); //series->pen().color());
        series_interpolatePoints->setBorderColor(Blue); //series->pen().color());

        series_interpolatedCurve->setPen(pen);

        if (!chartView->chart()->series().contains(series_CP))
            chartView->chart()->addSeries(series_CP);
        if (!chartView->chart()->series().contains(series_interpolatePoints))
            chartView->chart()->addSeries(series_interpolatePoints);
        if (!chartView->chart()->series().contains(series_interpolatedCurve))
            chartView->chart()->addSeries(series_interpolatedCurve);
        chartView->chart()->createDefaultAxes();
        updateCheckBoxEnable();
        checkBoxCPChanged(checkBoxCP->isChecked());
        checkBoxInterpolatedCurveChanged(checkBoxInterpolatedCurve->isChecked());
        checkBoxInterpolatedPointsChanged(checkBoxInterpolatedPoints->isChecked());
    }
}

void MainWindow::updateRangesWidgets()
{
    int numCP = getNumCP();
    if (rangesContainerLayout->rowCount() > numCP)
        while (rangesContainerLayout->rowCount() > numCP)
        {
            rangesContainerLayout->removeRow(rangesContainerLayout->rowCount() - 1);
            rangeSliders.pop_back();
        }
    if (rangesContainerLayout->rowCount() < numCP)
        for (int i = rangesContainerLayout->rowCount(); i < numCP; i++)
        {
            RangeSliderLayout *rangeslider = new RangeSliderLayout;
            rangeslider->SetRange(-1, 1);
            rangeslider->setMinimumWidth(320);
            QLabel *label = new QLabel("CP" + QString::number(i+1));
            label->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
            label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
            //boxScrollVLayout->addWidget(rangeslider, 1);
            rangesContainerLayout->addRow(label, rangeslider);
            rangeSliders.push_back(rangeslider);
        }
}

void MainWindow::updateTangentsNormals(bool changednumcp)
{
    if (this->data.getFileName().size() > 0 && bspline && bspline->getCParray().size() > 0)
    {
        Points normals = bspline->getNormalsInCP();
        Points tangents = bspline->getTangentsInCP();
        Points cps = bspline->getCParray();
        doubles maxparams = getMaxParams();
        doubles minparams = getMinParams();

        if (changednumcp)
        {
            for (int i = 0; i < series_normals.size(); i++)
            {
                chartView->chart()->removeSeries(series_normals[i]);
                chartView->chart()->removeSeries(series_tangents[i]);
            }
            series_normals.clear();
            series_tangents.clear();
        }

        QPen pen =QPen(Red);
        pen.setWidth(2);
        Point start;
        Point end;

        for (unsigned int i = 0; i < normals.size(); i++)
        {
            QLineSeries *series_n = changednumcp ? new QLineSeries : series_normals[i];
            start = cps[i] + maxparams[i] * normals[i];
            end = cps[i] + minparams[i] * normals[i];

            series_n->append(start.getx(), start.gety());
            series_n->append(end.getx(), end.gety());
            series_n->setPen(pen);

            if (changednumcp)
                series_normals.append(series_n);

            if (!chartView->chart()->series().contains(series_n))
                chartView->chart()->addSeries(series_n);
        }

        for (unsigned int i = 0; i < tangents.size(); i++)
        {
            QLineSeries *series_t = changednumcp ? new QLineSeries : series_tangents[i];
            start = cps[i] + maxparams[i] * tangents[i];
            end = cps[i] + minparams[i] * tangents[i];

            series_t->append(start.getx(), start.gety());
            series_t->append(end.getx(), end.gety());
            series_t->setPen(pen);

            if (changednumcp)
                series_tangents.append(series_t);

            if (!chartView->chart()->series().contains(series_t))
                chartView->chart()->addSeries(series_t);
        }
        updateCheckBoxEnable();
        checkBoxNormalsChanged(checkBoxNormals->isChecked());
        checkBoxTangentsChanged(checkBoxTangents->isChecked());
    }
}

void MainWindow::tabChanged(int index)
{
    if (index == 1) // ranges tab
    {
        updateRangesWidgets();
    }
}

void MainWindow::loadFile()
{
    LoadProfileDialog *dialog = new LoadProfileDialog;
    dialog->setWindowModality(Qt::WindowModality::ApplicationModal);
    dialog->setProfileData(this->data);
    int ok = dialog->exec();
    if (ok)
    {
        data = dialog->getProfileData();
        this->bspineFilenameEdit->setText(QString::fromStdString(data.getFileName()));
        updateChart();
    }
}

void MainWindow::applyAllMinRange()
{
    for (auto slider : rangeSliders)
        slider->SetMinimum(minRangeAllEdit->text().toDouble());
}

void MainWindow::applyAllMaxRange()
{
    for (auto slider : rangeSliders)
        slider->SetMaximum(maxRangeAllEdit->text().toDouble());
}

void MainWindow::applyAllMinValue()
{
    for (auto slider : rangeSliders)
        slider->SetLowerValue(minValueAllEdit->text().toDouble());
}

void MainWindow::applyAllMaxValue()
{
    for (auto slider : rangeSliders)
        slider->SetUpperValue(maxValueAllEdit->text().toDouble());
}

void MainWindow::updateCheckBoxEnable()
{
    checkBoxOriginalPoints->setEnabled(series_originalPoints->points().size() > 0);
    checkBoxOriginalCurve->setEnabled(series_originalCurve->points().size() > 0);
    checkBoxInterpolatedPoints->setEnabled(series_interpolatePoints->points().size() > 0);
    checkBoxInterpolatedCurve->setEnabled(series_interpolatedCurve->points().size() > 0);
    checkBoxCP->setEnabled(series_CP->points().size() > 0);
    checkBoxTE->setEnabled(series_TE->points().size() > 0);
    checkBoxMaxParams->setEnabled(series_maxParams->lowerSeries()); //->pointsVector().size() > 0);
    checkBoxMinParams->setEnabled(series_minParams->lowerSeries()); //->pointsVector().size() > 0);
    checkBoxNormals->setEnabled(series_normals.size() > 0);
    checkBoxTangents->setEnabled(series_tangents.size() > 0);
}

void MainWindow::checkBoxOriginalPointsChanged(int state)
{
    if (!state) series_originalPoints->hide();
    else series_originalPoints->show();
}

void MainWindow::checkBoxOriginalCurveChanged(int state)
{
    if (!state) series_originalCurve->hide();
    else series_originalCurve->show();
}

void MainWindow::checkBoxInterpolatedPointsChanged(int state)
{
    if (!state) series_interpolatePoints->hide();
    else series_interpolatePoints->show();
}

void MainWindow::checkBoxInterpolatedCurveChanged(int state)
{
    if (!state) series_interpolatedCurve->hide();
    else series_interpolatedCurve->show();
}

void MainWindow::checkBoxCPChanged(int state)
{
    if (!state) series_CP->hide();
    else series_CP->show();
}

void MainWindow::checkBoxTEChanged(int state)
{
    if (!state) series_TE->hide();
    else series_TE->show();
}

void MainWindow::checkBoxMaxParamsChanged(int state)
{
    if (!state) series_maxParams->hide();
    else series_maxParams->show();
}

void MainWindow::checkBoxMinParamsChanged(int state)
{
    if (!state) series_minParams->hide();
    else series_minParams->show();
}

void MainWindow::checkBoxNormalsChanged(int state)
{
    if (!state)
        for (auto series : series_normals)
            series->hide();
    else
        for (auto series : series_normals)
            series->show();
}

void MainWindow::checkBoxTangentsChanged(int state)
{
    if (!state)
        for (auto series : series_tangents)
            series->hide();
    else
        for (auto series : series_tangents)
            series->show();
}


void MainWindow::updateMINMAX()
{
    if (this->data.getFileName().size() > 0 && bspline && bspline->getCParray().size() > 0)
    {
        if (series_minParams->upperSeries())
        {
            series_minParams->upperSeries()->clear();
            series_minParams->lowerSeries()->clear();
            series_maxParams->upperSeries()->clear();
            series_maxParams->lowerSeries()->clear();
        }
        bspline->setTEMotion(getTEMotion());
        bspline->fillFixedParams({}, getNumCP());
        Points minBsplinePoints = bspline->getExtremeMin(getMinParams()).evaluateWithTE(getNumPoints(), getTENumPoints(), getTEShape(), true);
        Points maxBsplinePoints = bspline->getExtremeMax(getMaxParams()).evaluateWithTE(getNumPoints(), getTENumPoints(), getTEShape(), true);

        Points lowermin_p, uppermin_p;
        Points lowermax_p, uppermax_p;
        QLineSeries *lowermin_s = new QLineSeries;
        QLineSeries *lowermax_s = new QLineSeries;
        QLineSeries *uppermin_s = new QLineSeries;
        QLineSeries *uppermax_s = new QLineSeries;

        Utils::getupperlowercurves(minBsplinePoints, lowermin_p, uppermin_p);
        Utils::getupperlowercurves(maxBsplinePoints, lowermax_p, uppermax_p);

        appendPointsToSeries(lowermin_s, lowermin_p);
        appendPointsToSeries(uppermin_s, uppermin_p);
        appendPointsToSeries(lowermax_s, lowermax_p);
        appendPointsToSeries(uppermax_s, uppermax_p);

        series_minParams->setLowerSeries(lowermin_s);
        series_minParams->setUpperSeries(uppermin_s);
        series_maxParams->setLowerSeries(lowermax_s);
        series_maxParams->setUpperSeries(uppermax_s);

        QColor colorfillmin = Orange; // orange
        QColor colorfillmax = Brown; // brown

        colorfillmin.setAlpha(100);
        colorfillmax.setAlpha(100);

        QPen penmin =QPen(Orange);
        penmin.setWidth(2);
        QPen penmax =QPen(Brown);
        penmax.setWidth(2);

        QBrush brushmin = QBrush(colorfillmin);
        QBrush brushmax = QBrush(colorfillmax);

        series_minParams->setPen(penmin);
        series_maxParams->setPen(penmax);
        series_minParams->setBrush(brushmin);
        series_maxParams->setBrush(brushmax);

        if (!chartView->chart()->series().contains(series_minParams))
            chartView->chart()->addSeries(series_minParams);
        if (!chartView->chart()->series().contains(series_maxParams))
            chartView->chart()->addSeries(series_maxParams);
        chartView->chart()->createDefaultAxes();
        updateCheckBoxEnable();
        checkBoxMinParamsChanged(checkBoxMinParams->isChecked());
        checkBoxMaxParamsChanged(checkBoxMaxParams->isChecked());
    }
}

void MainWindow::updateTE()
{
    if (this->data.getFileName().size() > 0 && bspline && bspline->getCParray().size() > 0)
    {
        series_TE->clear();
        Points tepoints = bspline->evaluateTE(getTENumPoints(), getTEShape(), true);

        for (unsigned int i = 0; i < tepoints.size(); i++)
        {
            series_TE->append(tepoints[i].getx(), tepoints[i].gety());
        }
        QPen pen =QPen(Blue);
        pen.setWidth(2);

        series_TE->setPen(pen);

        if (!chartView->chart()->series().contains(series_TE))
            chartView->chart()->addSeries(series_TE);
        chartView->chart()->createDefaultAxes();
        updateCheckBoxEnable();
        checkBoxTEChanged(checkBoxTE->isChecked());
    }
}

void MainWindow::computeCP()
{
    bool changedCPnumber = oldCPnumber != getNumCP();
    Bspline tempbspline = Bspline::interpolate(data.getPoints(), getNumCP(), getN(), getKnotSequence());
    bspline = &tempbspline;
    updateBspline();
    updateKnotSeries(getKnotSequence().getSequence({}));
    if (checkBoxAutoMINMAX->isChecked())
        updateMINMAX();
    if (checkBoxAutoTE->isChecked())
        updateTE();
    updateTangentsNormals(changedCPnumber);
    oldCPnumber = getNumCP();
}

void MainWindow::optimizeKnots()
{

}

int MainWindow::updateKnotSeries(doubles uarray)
{
    knotsSeries->clear();
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
    return maxmultiplicity;
}

void MainWindow::checkBoxAutoMINMAXChanged(int state)
{
    buttonComputeMINMAX->setEnabled(!state);
}

void MainWindow::checkBoxAutoTEChanged(int state)
{
    buttonComputeTE->setEnabled(!state);
}


int MainWindow::getN()
{
    return bsplineNEdit->text().toInt();
}

int MainWindow::getNumCP()
{
    return bsplineCPnumEdit->text().toInt();
}

int MainWindow::getNumPoints()
{
    return bsplineNumPointsEdit->text().toInt();
}

int MainWindow::getTENumPoints()
{
    return bsplineTENumPointsEdit->text().toInt();
}

string MainWindow::getTEShape()
{
    return bsplineTEShapeCombo->currentText().toLower().toStdString();
}

doubles MainWindow::getMinParams()
{
    doubles params;
    for (unsigned int i = 0; i < rangeSliders.size(); i++)
    {
        params.push_back(rangeSliders[i]->GetLowerValue());
    }
    return params;
}

doubles MainWindow::getMaxParams()
{
    doubles params;
    for (unsigned int i = 0; i < rangeSliders.size(); i++)
    {
        params.push_back(rangeSliders[i]->GetUpperValue());
    }
    return params;
}

TEMotion MainWindow::getTEMotion()
{
    return (TEMotion)bsplineTEMotionCombo->currentIndex();
}

void MainWindow::appendPointsToSeries(QLineSeries *series, const Points &points)
{
    for (auto p : points)
    {
        series->append(p.getx(), p.gety());
    }
}
