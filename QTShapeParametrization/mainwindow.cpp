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
#include "Utils.h"
#include "qtutils.h"
#include <QDebug>
#include <QElapsedTimer>
#include <qtconcurrentrun.h>

//#include <QtQuickControls2>

typedef std::vector<Point> Points;
typedef std::vector<double> doubles;

class CPIndexTooltipText : public TooltipTextFunction {
    public:
        CPIndexTooltipText(Bspline &bspline)
            : bspline(bspline) {}

      virtual QString operator() (QLabel *popup, const QPointF &point_px, const QPointF &point_xy, const QPointF &cursor_px, const QPointF &cursor_xy, int index)
      {
            Points CParray = bspline.getCParray();
            int absolute_index = std::find(CParray.begin(), CParray.end(), Point(point_xy.x(), point_xy.y())) - CParray.begin();

            QColor color = Utils::contains(bspline.getAdjustableIndices(), absolute_index) ? QTUtils::Green() : QTUtils::Red();
            QString colorstr = QTUtils::color2str(color);

            popup->setStyleSheet("QLabel {background: white; color : " + colorstr +"; padding : 4px; border: 2px solid " + colorstr +"; border-radius: 10px;}");
            popup->setAlignment(Qt::AlignCenter);

            QString text;
            text.append("CP");
            text.append(QString::number(absolute_index + 1));
            return text;
      }

       Bspline bspline;
};

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

// //////////////////////////////////////////////////////////////////
// //////////////////////////////////////////////////////////////////
// //////////////////////////////////////////////////////////////////
// //////////////////////////////////////////////////////////////////
// //////////////////////////////////////////////////////////////////

void MainWindow::bsplineTaskStarted(const BsplineTask &task)
{
    progressBarLabel->setText(task.messange);
    progressBar->show();
    progressBarLabel->show();
    qDebug() << "bsplineTaskStarted =  " << task.messange << endl;
}

void MainWindow::bsplineTaskFinished(const BsplineTask &task)
{

}

void MainWindow::bsplineAllTasksFinished()
{
    progressBar->hide();
    progressBarLabel->hide();
    qDebug() << "bsplineAllTasksFinished =  " << endl;
}

// //////////////////////////////////////////////////////////////////
// //////////////////////////////////////////////////////////////////

void MainWindow::optimizationKnotsFinished(const BsplineTask &task, const Bspline &bspline)
{

}

void MainWindow::interpolationCPFinished(const BsplineTask &task, const Bspline &bspline)
{
    this->bspline = new Bspline(bspline);
}

void MainWindow::evaluationPointsFinished(const BsplineTask &task, const Points &points)
{
    if (this->data.getFileName().size() > 0 && bspline && bspline->getCParray().size() > 0)
    {
        QElapsedTimer timer;
        timer.start();

        series_interpolatePoints->clear();
        series_interpolatedCurve->clear();

        Points controlpoints = bspline->getCParray();
        QList<QPointF> points_adj = series_CP_adjustable->points();
        QList<QPointF> points_fix = series_CP_fixed->points();

        series_CP_fixed->clear();
        series_CP_adjustable->clear();

        for (unsigned int i = 0; i < controlpoints.size(); i++)
        {
            if (points_adj.contains(QPointF(controlpoints[i].getx(), controlpoints[i].gety())))
                series_CP_adjustable->append(controlpoints[i].getx(), controlpoints[i].gety());
            else // by default fixed
                series_CP_fixed->append(controlpoints[i].getx(), controlpoints[i].gety());
        }

        qDebug() << "time 2.1 = " << timer.elapsed() << endl;

        qDebug() << "time 2.2 = " << timer.elapsed() << endl;

        for (unsigned int i = 0; i < points.size(); i++)
        {
            Point p = points.operator[](i);
            series_interpolatePoints->append(p.getx(), p.gety());
            series_interpolatedCurve->append(p.getx(), p.gety());
        }

        qDebug() << "time 2.3 = " << timer.elapsed() << endl;

        QPen pen =QPen(QTUtils::Blue());
        pen.setWidth(2);

        series_CP_fixed->setMarkerSize(10);
        series_CP_fixed->setColor(QTUtils::Red()); //series->pen().color());
        series_CP_fixed->setBorderColor(QTUtils::Red()); //series->pen().color());

        series_CP_adjustable->setMarkerSize(10);
        series_CP_adjustable->setColor(QTUtils::Green()); //series->pen().color());
        series_CP_adjustable->setBorderColor(QTUtils::Green()); //series->pen().color());

        series_interpolatePoints->setMarkerSize(5);
        series_interpolatePoints->setColor(QTUtils::Blue()); //series->pen().color());
        series_interpolatePoints->setBorderColor(QTUtils::Blue()); //series->pen().color());

        series_interpolatedCurve->setPen(pen);

        if (!chartView->chart()->series().contains(series_CP_adjustable))
            chartView->chart()->addSeries(series_CP_adjustable);
        if (!chartView->chart()->series().contains(series_CP_fixed))
            chartView->chart()->addSeries(series_CP_fixed);
        if (!chartView->chart()->series().contains(series_interpolatePoints))
            chartView->chart()->addSeries(series_interpolatePoints);
        if (!chartView->chart()->series().contains(series_interpolatedCurve))
            chartView->chart()->addSeries(series_interpolatedCurve);
        chartView->chart()->createDefaultAxes();
        updateCheckBoxEnable();
        checkBoxCPChanged(checkBoxCP->isChecked());
        checkBoxInterpolatedCurveChanged(checkBoxInterpolatedCurve->isChecked());
        checkBoxInterpolatedPointsChanged(checkBoxInterpolatedPoints->isChecked());

        qDebug() << "time 2.4 = " << timer.elapsed() << endl;
        // set tooltip
        chartView->addTooltip(series_CP_fixed, new CPIndexTooltipText(*bspline));
        chartView->addTooltip(series_CP_adjustable, new CPIndexTooltipText(*bspline));
    }
}

void MainWindow::evaluationTEFinished(const BsplineTask &task, const Points &tepoints)
{
    if (this->data.getFileName().size() > 0 && bspline && bspline->getCParray().size() > 0)
    {
        series_TE->clear();
        //Points tepoints = bspline->evaluateTE(getTENumPoints(), getTEShape(), true);

        QTUtils::appendPointsToSeries(series_TE, tepoints);

//        for (unsigned int i = 0; i < tepoints.size(); i++)
//        {
//            Point p = tepoints.operator[](i);
//            series_TE->append(p.getx(), p.gety());
//        }
        QPen pen =QPen(QTUtils::Blue());
        pen.setWidth(2);

        series_TE->setPen(pen);

        if (!chartView->chart()->series().contains(series_TE))
            chartView->chart()->addSeries(series_TE);
        chartView->chart()->createDefaultAxes();
        updateCheckBoxEnable();
        checkBoxTEChanged(checkBoxTE->isChecked());
    }
}

void MainWindow::evaluationMinFinished(const BsplineTask &task, const Points & points)
{
    if (this->data.getFileName().size() > 0 && bspline && bspline->getCParray().size() > 0)
    {
        if (task.changedNumCP)
            updateRangesWidgets();
        if (series_minParams->upperSeries())
        {
            series_minParams->upperSeries()->clear();
            series_minParams->lowerSeries()->clear();
        }
        bspline->setAdjustableIndices(getAdjustableIndexes());
        bspline->setTEMotion(getTEMotion());
        //bspline->fillFixedParams({}, getNumCP());
        Points minBsplinePoints = bspline->getExtremeMin(getMinParams()).evaluateWithTE(getNumPoints(), getTENumPoints(), getTEShape(), true);

        Points lowermin_p, uppermin_p;
        QLineSeries *lowermin_s = new QLineSeries;
        QLineSeries *uppermin_s = new QLineSeries;

        Utils::getupperlowercurves(minBsplinePoints, lowermin_p, uppermin_p);

        QTUtils::appendPointsToSeries(lowermin_s, lowermin_p);
        QTUtils::appendPointsToSeries(uppermin_s, uppermin_p);

        series_minParams->setLowerSeries(lowermin_s);
        series_minParams->setUpperSeries(uppermin_s);

        QColor colorfillmin = QTUtils::Orange(); // orange
        colorfillmin.setAlpha(150);

        QPen penmin =QPen(QTUtils::Orange());
        penmin.setWidth(2);

        QBrush brushmin = QBrush(colorfillmin);

        series_minParams->setPen(penmin);
        series_minParams->setBrush(brushmin);

        if (!chartView->chart()->series().contains(series_minParams))
            chartView->chart()->addSeries(series_minParams);
        chartView->chart()->createDefaultAxes();
        updateCheckBoxEnable();
        checkBoxMinParamsChanged(checkBoxMinParams->isChecked());
    }
}

void MainWindow::evaluationMaxFinished(const BsplineTask & task, const Points & points)
{
    if (this->data.getFileName().size() > 0 && bspline && bspline->getCParray().size() > 0)
    {
        if (task.changedNumCP)
            updateRangesWidgets();
        if (series_maxParams->upperSeries())
        {
            series_maxParams->upperSeries()->clear();
            series_maxParams->lowerSeries()->clear();
        }
        bspline->setAdjustableIndices(getAdjustableIndexes());
        bspline->setTEMotion(getTEMotion());
        //bspline->fillFixedParams({}, getNumCP());
        Points maxBsplinePoints = bspline->getExtremeMax(getMaxParams()).evaluateWithTE(getNumPoints(), getTENumPoints(), getTEShape(), true);

        Points lowermax_p, uppermax_p;
        QLineSeries *lowermax_s = new QLineSeries;
        QLineSeries *uppermax_s = new QLineSeries;

        Utils::getupperlowercurves(maxBsplinePoints, lowermax_p, uppermax_p);

        QTUtils::appendPointsToSeries(lowermax_s, lowermax_p);
        QTUtils::appendPointsToSeries(uppermax_s, uppermax_p);

        series_maxParams->setLowerSeries(lowermax_s);
        series_maxParams->setUpperSeries(uppermax_s);

        QColor colorfillmax = QTUtils::Brown(); // brown
        colorfillmax.setAlpha(150);

        QPen penmax =QPen(QTUtils::Brown());
        penmax.setWidth(2);

        QBrush brushmax = QBrush(colorfillmax);

        series_maxParams->setPen(penmax);
        series_maxParams->setBrush(brushmax);

        if (!chartView->chart()->series().contains(series_maxParams))
            chartView->chart()->addSeries(series_maxParams);
        chartView->chart()->createDefaultAxes();
        updateCheckBoxEnable();
        checkBoxMaxParamsChanged(checkBoxMaxParams->isChecked());
        // set tooltip
        //chartView->addTooltip(series_CP_fixed, new CPIndexTooltipText(*bspline));
        //chartView->addTooltip(series_CP_adjustable, new CPIndexTooltipText(*bspline));
    }
}

void MainWindow::evaluationNormalsFinished(const BsplineTask & task,  const Points & normals)
{
    if (this->data.getFileName().size() > 0 && bspline && bspline->getCParray().size() > 0)
    {
        if (task.changedNumCP)
            updateRangesWidgets();
        //Points normals = bspline->getNormalsInCP();
        Points cps = bspline->getCParray();
        doubles maxparams = getMaxParams();
        doubles minparams = getMinParams();

        if (task.changedNumCP)
        {
            for (int i = 0; i < series_normals.size(); i++)
            {
                chartView->chart()->removeSeries(series_normals[i]);
            }
            series_normals.clear();
        }

        QPen pen =QPen(QTUtils::Red());
        pen.setWidth(2);
        Point start;
        Point end;

        for (unsigned int i = 0; i < normals.size(); i++)
        {
            QLineSeries *series_n = task.changedNumCP ? new QLineSeries : series_normals[i];
            start = cps[i] + maxparams[i] * normals[i];
            end = cps[i] + minparams[i] * normals[i];

            series_n->append(start.getx(), start.gety());
            series_n->append(end.getx(), end.gety());
            series_n->setPen(pen);

            if (task.changedNumCP)
                series_normals.append(series_n);

            if (!chartView->chart()->series().contains(series_n))
                chartView->chart()->addSeries(series_n);
        }

        updateCheckBoxEnable();
        checkBoxNormalsChanged(checkBoxNormals->isChecked());
    }
}

void MainWindow::evaluationTangentsFinished(const BsplineTask & task, const Points &tangents)
{
    if (this->data.getFileName().size() > 0 && bspline && bspline->getCParray().size() > 0)
    {
        if (task.changedNumCP)
            updateRangesWidgets();
        //Points tangents = bspline->getTangentsInCP();
        Points cps = bspline->getCParray();
        doubles maxparams = getMaxParams();
        doubles minparams = getMinParams();

        if (task.changedNumCP)
        {
            for (int i = 0; i < series_tangents.size(); i++)
            {
                chartView->chart()->removeSeries(series_tangents[i]);
            }
            series_tangents.clear();
        }

        QPen pen =QPen(QTUtils::Red());
        pen.setWidth(2);
        Point start;
        Point end;

        for (unsigned int i = 0; i < tangents.size(); i++)
        {
            QLineSeries *series_t = task.changedNumCP ? new QLineSeries : series_tangents[i];
            start = cps[i] + maxparams[i] * tangents[i];
            end = cps[i] + minparams[i] * tangents[i];

            series_t->append(start.getx(), start.gety());
            series_t->append(end.getx(), end.gety());
            series_t->setPen(pen);

            if (task.changedNumCP)
                series_tangents.append(series_t);

            if (!chartView->chart()->series().contains(series_t))
                chartView->chart()->addSeries(series_t);
        }
        updateCheckBoxEnable();
        checkBoxTangentsChanged(checkBoxTangents->isChecked());
    }
}

void MainWindow::evaluationErrorApproxFinished(const BsplineTask &task, const doubles &us, const doubles &error)
{
    if (this->data.getFileName().size() > 0 && bspline && bspline->getCParray().size() > 0)
    {
        series_errorApprox->clear();

        Points points = Point::fromDoubles(us, error);
        QTUtils::appendPointsToSeries(series_errorApprox, points);

        QPen pen =QPen(QTUtils::Blue());
        pen.setWidth(2);

        series_errorApprox->setPen(pen);

        if (!chartErrorView->chart()->series().contains(series_errorApprox))
            chartErrorView->chart()->addSeries(series_errorApprox);
        chartErrorView->chart()->createDefaultAxes();
        //updateCheckBoxEnable();
    }
}

void MainWindow::evaluationErrorPreciseFinished(const BsplineTask &task, const doubles &us, const doubles &error)
{
    if (this->data.getFileName().size() > 0 && bspline && bspline->getCParray().size() > 0)
    {
        series_errorPrecise->clear();

        Points points = Point::fromDoubles(us, error);
        QTUtils::appendPointsToSeries(series_errorPrecise, points);

        QPen pen =QPen(QTUtils::Red());
        pen.setWidth(2);
        series_errorPrecise->setPen(pen);

        if (!chartErrorView->chart()->series().contains(series_errorPrecise))
            chartErrorView->chart()->addSeries(series_errorPrecise);
        chartErrorView->chart()->createDefaultAxes();
        //updateCheckBoxEnable();
    }
}

// //////////////////////////////////////////////////////////////////
// //////////////////////////////////////////////////////////////////


BsplineTask *MainWindow::addErrorApproxTask(BsplineTaskManager *bsplinemanager)
{
    errorApproxTask = new BsplineTask(BsplineTaskType::EVALUATION_ERRORS_APPROX, "Evaluating approximate error..");
    errorApproxTask->bspline = this->bspline;
    errorApproxTask->importBSplineByPrevious = true;
    errorApproxTask->data = &data;
    connect(bsplinemanager, QOverload<const BsplineTask&, const doubles&, const doubles&>::of(&BsplineTaskManager::evaluationErrorApproxFinished), this, &MainWindow::evaluationErrorApproxFinished);
    bsplinemanager->addTask(*errorApproxTask);
    return errorApproxTask;
}

BsplineTask *MainWindow::addErrorPreciseTask(BsplineTaskManager *bsplinemanager)
{
    errorPreciseTask = new BsplineTask(BsplineTaskType::EVALUATION_ERRORS_PRECISE, "Evaluating precise error..");
    errorPreciseTask->bspline = this->bspline;
    errorPreciseTask->importBSplineByPrevious = true;
    errorPreciseTask->data = &data;
    connect(bsplinemanager, QOverload<const BsplineTask&, const doubles&, const doubles&>::of(&BsplineTaskManager::evaluationErrorPreciseFinished), this, &MainWindow::evaluationErrorPreciseFinished);
    bsplinemanager->addTask(*errorPreciseTask);
    return errorPreciseTask;
}

BsplineTask *MainWindow::addEvaluationTangentsTask(BsplineTaskManager *bsplinemanager)
{
    evaluationTangentsTask = new BsplineTask(BsplineTaskType::EVALUATION_TANGENTS, "Evaluating tangents..");
    evaluationTangentsTask->bspline = this->bspline;
    evaluationTangentsTask->importBSplineByPrevious = true;
    evaluationTangentsTask->changedNumCP = isNumCPChanged(false);
    connect(bsplinemanager, QOverload<const BsplineTask&, const Points&>::of(&BsplineTaskManager::evaluationTangentsFinished), this, &MainWindow::evaluationTangentsFinished);
    bsplinemanager->addTask(*evaluationTangentsTask);
    return evaluationTangentsTask;
}

BsplineTask *MainWindow::addEvaluationNormalsTask(BsplineTaskManager *bsplinemanager)
{
    evaluationNormalsTask = new BsplineTask(BsplineTaskType::EVALUATION_NORMALS, "Evaluating normals..");
    evaluationNormalsTask->bspline = this->bspline;
    evaluationNormalsTask->importBSplineByPrevious = true;
    evaluationNormalsTask->changedNumCP = isNumCPChanged(false);
    connect(bsplinemanager, QOverload<const BsplineTask&, const Points&>::of(&BsplineTaskManager::evaluationNormalsFinished), this, &MainWindow::evaluationNormalsFinished);
    bsplinemanager->addTask(*evaluationNormalsTask);
    return evaluationNormalsTask;
}

BsplineTask *MainWindow::addEvaluationMAXTask(BsplineTaskManager *bsplinemanager)
{
    evaluationMAXTask = new BsplineTask(BsplineTaskType::EVALUATION_MAX, "Evaluating max range bspline..");
    evaluationMAXTask->bspline = this->bspline;
    evaluationMAXTask->maxparams = getMaxParams();
    evaluationMAXTask->TEshape = getTEShape();
    evaluationMAXTask->TEnumPoints = getTENumPoints();
    evaluationMAXTask->numPoints = getNumPoints();
    evaluationMAXTask->importBSplineByPrevious = true;
    evaluationMAXTask->changedNumCP = isNumCPChanged(false);
    connect(bsplinemanager, QOverload<const BsplineTask&, const Points&>::of(&BsplineTaskManager::evaluationMaxFinished), this, &MainWindow::evaluationMaxFinished);
    bsplinemanager->addTask(*evaluationMAXTask);
    return evaluationMAXTask;
}

BsplineTask *MainWindow::addEvaluationMINTask(BsplineTaskManager *bsplinemanager)
{
    evaluationMINTask = new BsplineTask(BsplineTaskType::EVALUATION_MIN, "Evaluating min range bspline..");
    evaluationMINTask->bspline = this->bspline;
    evaluationMINTask->minparams = getMinParams();
    evaluationMINTask->TEshape = getTEShape();
    evaluationMINTask->TEnumPoints = getTENumPoints();
    evaluationMINTask->numPoints = getNumPoints();
    evaluationMINTask->importBSplineByPrevious = true;
    evaluationMINTask->changedNumCP = isNumCPChanged(false);
    connect(bsplinemanager, QOverload<const BsplineTask&, const Points&>::of(&BsplineTaskManager::evaluationMinFinished), this, &MainWindow::evaluationMinFinished);
    bsplinemanager->addTask(*evaluationMINTask);
    return evaluationMINTask;
}

BsplineTask *MainWindow::addEvaluationTETask(BsplineTaskManager *bsplinemanager)
{
    evaluationTETask = new BsplineTask(BsplineTaskType::EVALUATION_TE, "Evaluating trailing edge..");
    evaluationTETask->bspline = this->bspline;
    evaluationTETask->TEshape = getTEShape();
    evaluationTETask->TEnumPoints = getTENumPoints();
    evaluationTETask->importBSplineByPrevious = true;
    connect(bsplinemanager, QOverload<const BsplineTask&, const Points&>::of(&BsplineTaskManager::evaluationTEFinished), this, &MainWindow::evaluationTEFinished);
    bsplinemanager->addTask(*evaluationTETask);
    return evaluationTETask;
}

BsplineTask *MainWindow::addEvaluationPointsTask(BsplineTaskManager *bsplinemanager)
{
    evaluationPointsTask = new BsplineTask(BsplineTaskType::EVALUATION_POINTS, "Evaluating points..");
    evaluationPointsTask->bspline = this->bspline;
    evaluationPointsTask->numPoints = getNumPoints();
    evaluationPointsTask->importBSplineByPrevious = true;
    evaluationPointsTask->data = &data;
    connect(bsplinemanager, QOverload<const BsplineTask&, const Points&>::of(&BsplineTaskManager::evaluationPointsFinished), this, &MainWindow::evaluationPointsFinished);
    bsplinemanager->addTask(*evaluationPointsTask);
    return evaluationPointsTask;
}

BsplineTask *MainWindow::addInterpolationTask(BsplineTaskManager *bsplinemanager)
{
    interpolationTask = new BsplineTask(BsplineTaskType::INTERPOLATION_CP, "Computing best CP..");
    interpolationTask->n = getN();
    interpolationTask->numCP = getNumCP();
    interpolationTask->data = &this->data;
    interpolationTask->knotSequence = getKnotSequence();
    connect(bsplinemanager, QOverload<const BsplineTask&, const Bspline&>::of(&BsplineTaskManager::interpolationCPFinished), this, &MainWindow::interpolationCPFinished);
    bsplinemanager->addTask(*interpolationTask);
    return interpolationTask;
}

// //////////////////////////////////////////////////////////////////
// //////////////////////////////////////////////////////////////////
// //////////////////////////////////////////////////////////////////
// //////////////////////////////////////////////////////////////////
// //////////////////////////////////////////////////////////////////

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
    formContainerVLayout->addItem(QTUtils::separator());
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
    chartView = new ChartView();
        chartView->setChart(chart);
        chartView->setRenderHint(QPainter::Antialiasing);
        chartView->setDirectionZoom(ChartView::BothDirectionZoom);
        chartView->setRubberBand(new QRubberBand(QRubberBand::Shape::Rectangle, chartView));

    progressBarLayout = new QHBoxLayout;
    progressBar = new QProgressBar;
    progressBarLabel = new QLabel("Computing Bspline...");
    progressBarLayout->addWidget(progressBarLabel);
    progressBarLayout->addWidget(progressBar);
    progressBarLayout->setContentsMargins(QMargins(14, 2 , 14 , 2));
    progressBar->setRange(0,0);
    progressBar->setValue(0);
    progressBarLabel->hide();
    progressBar->hide();
    //longFunctionStart("test");

    //QGraphicsProxyWidget *proxy = chart->scene()->addWidget(progressBarRootWidget);

    QGroupBox *groupCheckBoxChart = new QGroupBox(tr("Plots visibility"));
    QHBoxLayout *checkBoxHLayout = new QHBoxLayout;
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
    showErrorButton = new QPushButton("Show Error");
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
    chartView->chart()->addSeries(series_CP_fixed);
    chartView->chart()->addSeries(series_CP_adjustable);
    chartView->chart()->addSeries(series_originalPoints);
    chartView->chart()->addSeries(series_interpolatePoints);
    chartView->chart()->addSeries(series_originalCurve);
    chartView->chart()->addSeries(series_TE);
    chartView->chart()->addSeries(series_interpolatedCurve);

    series_maxParams->setUseOpenGL(true);
    series_minParams->setUseOpenGL(true);
    series_CP_fixed->setUseOpenGL(false);
    series_CP_adjustable->setUseOpenGL(false);
    series_originalPoints->setUseOpenGL(false);
    series_interpolatePoints->setUseOpenGL(true);
    series_originalCurve->setUseOpenGL(false);
    series_TE->setUseOpenGL(true);
    series_interpolatedCurve->setUseOpenGL(true);

    QString colorstr = QTUtils::color2str(QTUtils::Red());
    QLabel *popup = new QLabel("    ");
    popup->setAlignment(Qt::AlignCenter);
    popup->setStyleSheet("QLabel {background: white; color : " + colorstr +"; padding : 4px; border: 2px solid " + colorstr +"; border-radius: 10px;}");
    chartView->setShowTooltip(true);
    chartView->setPopup(popup);
    //chartView->addTooltip(series_CP_fixed, new CPIndexTooltipText(bspline));
    //chartView->addTooltip(series_CP_adjustable, new CPIndexTooltipText(bspline));
    chartView->addToggleable(series_CP_fixed, series_CP_adjustable);

    //////////////
    // TODO
    QLineSeries *series2 = new QLineSeries();
    Chart *chart2 = new Chart();
    chart2->legend()->hide();
    chart2->addSeries(series2);
    chart2->createDefaultAxes();
    chartErrorView = new ChartView;
    chartErrorView->setChart(chart2);
    chartErrorView->setRenderHint(QPainter::Antialiasing);
    chartErrorView->setCanScroll(false);
    chartErrorView->setDirectionZoom(ChartView::VerticalZoom);
    chartErrorView->setRubberBand(new QRubberBand(QRubberBand::Shape::Rectangle, chartView));

    chartErrorView->chart()->addSeries(series_errorApprox);
    chartErrorView->chart()->addSeries(series_errorPrecise);

    showBsplineButton = new QPushButton("Show Curve");

    QGroupBox *groupCheckBoxChartError = new QGroupBox(tr("Errors visibility"));
    QGridLayout *checkBoxGridErrorLayout = new QGridLayout;
    checkBoxGridErrorLayout->addWidget(showBsplineButton);

    groupCheckBoxChartError->setLayout(checkBoxGridErrorLayout);
    //////////////

    stackedLayoutChart = new QStackedLayout;
    stackedLayoutChart->addWidget(chartView);
    stackedLayoutChart->addWidget(chartErrorView);

    stackedLayoutCheckBoxGroup = new QStackedLayout;
    stackedLayoutCheckBoxGroup->addWidget(groupCheckBoxChart);
    stackedLayoutCheckBoxGroup->addWidget(groupCheckBoxChartError);

    checkBoxHLayout->addLayout(checkBoxVLayout, 8);
    checkBoxHLayout->addWidget(showErrorButton, 1);
    groupCheckBoxChart->setLayout(checkBoxHLayout);
    rightLayout->addLayout(progressBarLayout, 0);
    rightLayout->addLayout(stackedLayoutChart, 1);
    rightLayout->addLayout(stackedLayoutCheckBoxGroup, 0);

    connect(showErrorButton, &QPushButton::clicked, this, &MainWindow::showErrorChart);
    connect(showBsplineButton, &QPushButton::clicked, this, &MainWindow::showBsplineChart);

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
    knotsChartView = new ChartView();
        knotsChartView->setChart(chart);
        knotsChartView->setRenderHint(QPainter::Antialiasing);
        knotsChartView->setFixedHeight(110);
        knotsChartView->setCanZoom(false);
        knotsChartView->setCanScroll(false);
        knotsChartView->setCanUseKeys(false);
        knotsChartView->setShowTooltip(true);


    QColor blue = QColor(0x1E, 0x90, 0xFF);
    QString bluestr = QString("rgb(").append(QString::number(blue.red())).append(", ").append(QString::number(blue.green())).append(", ").append(QString::number(blue.blue())).append(")");
    QLabel *popup = new QLabel("       ");
    popup->setAlignment(Qt::AlignCenter);
    popup->setStyleSheet("QLabel {background: white; color : " + bluestr +"; padding : 4px; border: 2px solid " + bluestr +"; border-radius: 10px;}");
    knotsChartView->setPopup(popup);
    knotsChartView->addTooltip(knotsSeries, new XTooltipText(3));
    return knotsChartView;
}

// //////////////////////////////////////////////////////////////////
// //////////////////////////////////////////////////////////////////
// //////////////////////////////////////////////////////////////////
// //////////////////////////////////////////////////////////////////
// //////////////////////////////////////////////////////////////////

void MainWindow::tabChanged(int index)
{
    if (index == 1) // ranges tab
    {
        updateRangesWidgets();
    }
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
    if (!state)
    {
        series_CP_fixed->hide();
        series_CP_adjustable->hide();
    }
    else
    {
        series_CP_fixed->show();
        series_CP_adjustable->show();
    }
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

void MainWindow::checkBoxAutoMINMAXChanged(int state)
{
    buttonComputeMINMAX->setEnabled(!state);
}

void MainWindow::checkBoxAutoTEChanged(int state)
{
    buttonComputeTE->setEnabled(!state);
}

// //////////////////////////////////////////////////////////////////
// //////////////////////////////////////////////////////////////////

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
        updatePointsChart();
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

// //////////////////////////////////////////////////////////////////
// //////////////////////////////////////////////////////////////////

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

void MainWindow::updateCheckBoxEnable()
{
    checkBoxOriginalPoints->setEnabled(series_originalPoints->points().size() > 0);
    checkBoxOriginalCurve->setEnabled(series_originalCurve->points().size() > 0);
    checkBoxInterpolatedPoints->setEnabled(series_interpolatePoints->points().size() > 0);
    checkBoxInterpolatedCurve->setEnabled(series_interpolatedCurve->points().size() > 0);
    checkBoxCP->setEnabled(series_CP_fixed->points().size() + series_CP_adjustable->points().size() > 0);
    checkBoxTE->setEnabled(series_TE->points().size() > 0);
    checkBoxMaxParams->setEnabled(series_maxParams->lowerSeries()); //->pointsVector().size() > 0);
    checkBoxMinParams->setEnabled(series_minParams->lowerSeries()); //->pointsVector().size() > 0);
    checkBoxNormals->setEnabled(series_normals.size() > 0);
    checkBoxTangents->setEnabled(series_tangents.size() > 0);
}

void MainWindow::updatePointsChart()
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

// //////////////////////////////////////////////////////////////////
// //////////////////////////////////////////////////////////////////

void MainWindow::showErrorChart()
{
    stackedLayoutChart->setCurrentIndex(1);
    stackedLayoutCheckBoxGroup->setCurrentIndex(1);
}

void MainWindow::showBsplineChart()
{
    stackedLayoutChart->setCurrentIndex(0);
    stackedLayoutCheckBoxGroup->setCurrentIndex(0);
}

// //////////////////////////////////////////////////////////////////
// //////////////////////////////////////////////////////////////////
// //////////////////////////////////////////////////////////////////
// //////////////////////////////////////////////////////////////////
// //////////////////////////////////////////////////////////////////

//void MainWindow::updateBspline()
//{
//    if (this->data.getFileName().size() > 0 && bspline && bspline->getCParray().size() > 0)
//    {
//        QElapsedTimer timer;
//        timer.start();

//        series_interpolatePoints->clear();
//        series_interpolatedCurve->clear();

//        Points controlpoints = bspline->getCParray();
//        QList<QPointF> points_adj = series_CP_adjustable->points();
//        QList<QPointF> points_fix = series_CP_fixed->points();

//        series_CP_fixed->clear();
//        series_CP_adjustable->clear();

//        for (unsigned int i = 0; i < controlpoints.size(); i++)
//        {
//            if (points_adj.contains(QPointF(controlpoints[i].getx(), controlpoints[i].gety())))
//                series_CP_adjustable->append(controlpoints[i].getx(), controlpoints[i].gety());
//            else // by default fixed
//                series_CP_fixed->append(controlpoints[i].getx(), controlpoints[i].gety());
//        }

//        qDebug() << "time 2.1 = " << timer.elapsed() << endl;


//        int numsteps = bsplineNumPointsEdit->text().toInt();
//        Points points;// = runLongTask(QtConcurrent::run([this, numsteps ]() {return this->bspline->evaluate(numsteps);}), "Computing the interpolated curve..");

//        //Points points = bspline->evaluate(bsplineNumPointsEdit->text().toInt());

//        //cout << Utils::print(points, "  ", true);

//        qDebug() << "time 2.2 = " << timer.elapsed() << endl;

////        runLongTask(QtConcurrent::run([this, points ]()
////        {
////            QThread::sleep(3);
//            for (unsigned int i = 0; i < points.size(); i++)
//            {
//                series_interpolatePoints->append(points[i].getx(), points[i].gety());
//                series_interpolatedCurve->append(points[i].getx(), points[i].gety());
//            }
////        }), "Adding points to the chart..");

//        qDebug() << "time 2.3 = " << timer.elapsed() << endl;

//        QPen pen =QPen(QTUtils::Blue());
//        pen.setWidth(2);

//        series_CP_fixed->setMarkerSize(10);
//        series_CP_fixed->setColor(QTUtils::Red()); //series->pen().color());
//        series_CP_fixed->setBorderColor(QTUtils::Red()); //series->pen().color());

//        series_CP_adjustable->setMarkerSize(10);
//        series_CP_adjustable->setColor(QTUtils::Green()); //series->pen().color());
//        series_CP_adjustable->setBorderColor(QTUtils::Green()); //series->pen().color());

//        series_interpolatePoints->setMarkerSize(5);
//        series_interpolatePoints->setColor(QTUtils::Blue()); //series->pen().color());
//        series_interpolatePoints->setBorderColor(QTUtils::Blue()); //series->pen().color());

//        series_interpolatedCurve->setPen(pen);

//        if (!chartView->chart()->series().contains(series_CP_adjustable))
//            chartView->chart()->addSeries(series_CP_adjustable);
//        if (!chartView->chart()->series().contains(series_CP_fixed))
//            chartView->chart()->addSeries(series_CP_fixed);
//        if (!chartView->chart()->series().contains(series_interpolatePoints))
//            chartView->chart()->addSeries(series_interpolatePoints);
//        if (!chartView->chart()->series().contains(series_interpolatedCurve))
//            chartView->chart()->addSeries(series_interpolatedCurve);
//        chartView->chart()->createDefaultAxes();
//        updateCheckBoxEnable();
//        checkBoxCPChanged(checkBoxCP->isChecked());
//        checkBoxInterpolatedCurveChanged(checkBoxInterpolatedCurve->isChecked());
//        checkBoxInterpolatedPointsChanged(checkBoxInterpolatedPoints->isChecked());

//        qDebug() << "time 2.4 = " << timer.elapsed() << endl;
//    }
//}

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

        QPen pen =QPen(QTUtils::Red());
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


void MainWindow::updateMINMAX()
{
    BsplineTaskManager *bsplinemanager = new BsplineTaskManager;

    addEvaluationMINTask(bsplinemanager);
    addEvaluationMAXTask(bsplinemanager);

    QThreadPool::globalInstance()->start((QRunnable*)bsplinemanager);

    connect(bsplinemanager, QOverload<const BsplineTask&>::of(&BsplineTaskManager::taskStarted), this, &MainWindow::bsplineTaskStarted);
    connect(bsplinemanager, &BsplineTaskManager::allTasksFinished, this, &MainWindow::bsplineAllTasksFinished);
}

void MainWindow::updateTE()
{
    BsplineTaskManager *bsplinemanager = new BsplineTaskManager;

    addEvaluationTETask(bsplinemanager);

    QThreadPool::globalInstance()->start((QRunnable*)bsplinemanager);

    connect(bsplinemanager, QOverload<const BsplineTask&>::of(&BsplineTaskManager::taskStarted), this, &MainWindow::bsplineTaskStarted);
    connect(bsplinemanager, &BsplineTaskManager::allTasksFinished, this, &MainWindow::bsplineAllTasksFinished);
}

void MainWindow::computeCP()
{
    QElapsedTimer timer;
    timer.start();

    //bool changedCPnumber = oldCPnumber != getNumCP();

    BsplineTaskManager *bsplinemanager = new BsplineTaskManager;

    addInterpolationTask(bsplinemanager);
    addEvaluationPointsTask(bsplinemanager);
    if (checkBoxAutoTE->isChecked())
        addEvaluationTETask(bsplinemanager);
    if (checkBoxAutoMINMAX->isChecked())
    {
        addEvaluationMINTask(bsplinemanager);
        addEvaluationMAXTask(bsplinemanager);
    }
    addEvaluationNormalsTask(bsplinemanager);
    addEvaluationTangentsTask(bsplinemanager);
    addErrorApproxTask(bsplinemanager);
    addErrorPreciseTask(bsplinemanager);

//    bsplinemanager->addTask(*interpolationTask);
//    bsplinemanager->addTask(*evaluationPointsTask);
//    if (checkBoxAutoTE->isChecked())
//        bsplinemanager->addTask(*evaluationTETask);
//    if (checkBoxAutoMINMAX->isChecked())
//    {
//        bsplinemanager->addTask(*evaluationMINTask);
//        bsplinemanager->addTask(*evaluationMAXTask);
//    }

    //bsplinemanager->run();

    QThreadPool::globalInstance()->start((QRunnable*)bsplinemanager);

    connect(bsplinemanager, QOverload<const BsplineTask&>::of(&BsplineTaskManager::taskStarted), this, &MainWindow::bsplineTaskStarted);
    connect(bsplinemanager, &BsplineTaskManager::allTasksFinished, this, &MainWindow::bsplineAllTasksFinished);

//    connect(bsplinemanager, QOverload<const BsplineTask&, const Bspline&>::of(&BsplineTaskManager::interpolationCPFinished), this, &MainWindow::interpolationCPFinished);
//    connect(bsplinemanager, QOverload<const BsplineTask&, const Points&>::of(&BsplineTaskManager::evaluationPointsFinished), this, &MainWindow::evaluationPointsFinished);
//    connect(bsplinemanager, QOverload<const BsplineTask&, const Points&>::of(&BsplineTaskManager::evaluationTEFinished), this, &MainWindow::evaluationTEFinished);
//    connect(bsplinemanager, QOverload<const BsplineTask&, const Points&>::of(&BsplineTaskManager::evaluationMinFinished), this, &MainWindow::evaluationMinFinished);
//    connect(bsplinemanager, QOverload<const BsplineTask&, const Points&>::of(&BsplineTaskManager::evaluationMaxFinished), this, &MainWindow::evaluationMaxFinished);

    isNumCPChanged();
    //oldCPnumber = getNumCP();
}

//void MainWindow::computeCP()
//{
//    QElapsedTimer timer;
//    timer.start();

//    bool changedCPnumber = oldCPnumber != getNumCP();

//    runLongTask(QtConcurrent::run(this, &MainWindow::interpolatebspline), "Interpolating the curve..", [] () {});

//    //Bspline tempbspline = runLongTask(QtConcurrent::run(Bspline::interpolate, data.getPoints(), getNumCP(), getN(), getKnotSequence()), "Interpolating the curve..");

//    //Bspline tempbspline = Bspline::interpolate(data.getPoints(), getNumCP(), getN(), getKnotSequence());

//    qDebug() << "time 1 = " << timer.elapsed() << endl;

//    //bspline = &tempbspline;

//    updateBspline();

//    qDebug() << "time 3 = " << timer.elapsed() << endl;

//    updateKnotSeries(getKnotSequence().getSequence({}));
//    if (checkBoxAutoMINMAX->isChecked())
//        updateMINMAX();

//    qDebug() << "time 5 = " << timer.elapsed() << endl;

//    if (checkBoxAutoTE->isChecked())
//        updateTE();

//    qDebug() << "time 6 = " << timer.elapsed() << endl;

//    updateTangentsNormals(changedCPnumber);

//    qDebug() << "time 7 = " << timer.elapsed() << endl;

//    oldCPnumber = getNumCP();
//}

void MainWindow::optimizeKnots()
{

}


// //////////////////////////////////////////////////////////////////
// //////////////////////////////////////////////////////////////////
// //////////////////////////////////////////////////////////////////
// //////////////////////////////////////////////////////////////////
// //////////////////////////////////////////////////////////////////

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

ints MainWindow::getAdjustableIndexes()
{
    if (!bspline)
        return {};

    Points CParray = bspline->getCParray();
    ints adjustable_indexes;
    for (QPointF point : series_CP_adjustable->points())
    {
        auto absolute_index = std::find(CParray.begin(), CParray.end(), Point(point.x(), point.y())) - CParray.begin();
        adjustable_indexes.push_back(absolute_index);
    }
    bspline->setAdjustableIndices(adjustable_indexes);
    return adjustable_indexes;
}

TEMotion MainWindow::getTEMotion()
{
    return (TEMotion)bsplineTEMotionCombo->currentIndex();
}

KnotSequences MainWindow::getKnotSequence()
{
    return KnotSequences::getCompleteBirationalFixedKS(getN(), getNumCP(), 1.2, 1.0/1.2, 0.7);
}

bool MainWindow::isNumCPChanged(bool updateOld)
{
    bool changedCPnumber = oldCPnumber != getNumCP();
    if (updateOld)
        oldCPnumber = getNumCP();
    return changedCPnumber;
}

// //////////////////////////////////////////////////////////////////
// //////////////////////////////////////////////////////////////////
// //////////////////////////////////////////////////////////////////
// //////////////////////////////////////////////////////////////////
// //////////////////////////////////////////////////////////////////

