#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "mychart.h"
#include "Bspline.h"
#include "mychartview.h"
#include "RangeSlider.h"
#include "rangesliderlayout.h"
#include "profiledata.h"
#include "bsplinetask.h"

#include <QDialog>
#include <QTableView>
#include <QLayoutItem>
#include <QtWidgets>
#include <QtCharts>
#include <QLayoutItem>
#include <vector>

//namespace Ui {
//class MainWindow;
//}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
        explicit MainWindow(QWidget *parent = nullptr);
        ~MainWindow();

public slots:
        void bsplineTaskStarted(const BsplineTask &task);
        void bsplineTaskFinished(const BsplineTask &task);
        void bsplineAllTasksFinished();

        void optimizationKnotsFinished(const BsplineTask &task, const Bspline &bspline);
        void interpolationCPFinished(const BsplineTask &task, const Bspline &bspline);
        void evaluationPointsFinished(const BsplineTask &task, const Points & points);
        void evaluationPointsCompareFinished(const BsplineTask &task, const Points & points);
        void evaluationTEFinished(const BsplineTask &task, const Points & points);
        void evaluationMinFinished(const BsplineTask &task, const Points & points);
        void evaluationMaxFinished(const BsplineTask &task, const Points & points);
        void evaluationNormalsFinished(const BsplineTask &task, const Points & normals);
        void evaluationTangentsFinished(const BsplineTask &task, const Points & tangents);
        void evaluationErrorApproxFinished(const BsplineTask &task, const doubles &us, const doubles &error);
        void evaluationErrorPreciseFinished(const BsplineTask &task, const doubles &us, const doubles &error);

        void showErrorChart();
        void showBsplineChart();
        void compareButtonClicked();
        void rangeValueChanged(double value, bool isFinished);
        void comboBoxTEMotionChanged(int index);
        void comboBoxTEShapeChanged(int index);

        void knotSelectedChanged(BaseFixedKnotSequence* knot);

protected:
        BsplineTask *addInterpolationTask(BsplineTaskManager *bsplinemanager) ;
        BsplineTask *addEvaluationPointsTask(BsplineTaskManager *bsplinemanager) ;
        BsplineTask *addEvaluationPointsCompareTask(BsplineTaskManager *bsplinemanager) ;
        BsplineTask *addEvaluationTETask(BsplineTaskManager *bsplinemanager) ;
        BsplineTask *addEvaluationMINTask(BsplineTaskManager *bsplinemanager) ;
        BsplineTask *addEvaluationMAXTask(BsplineTaskManager *bsplinemanager) ;
        BsplineTask *addEvaluationNormalsTask(BsplineTaskManager *bsplinemanager) ;
        BsplineTask *addEvaluationTangentsTask(BsplineTaskManager *bsplinemanager) ;
        BsplineTask *addErrorApproxTask(BsplineTaskManager *bsplinemanager) ;
        BsplineTask *addErrorPreciseTask(BsplineTaskManager *bsplinemanager) ;

        int getN();
        int getNumCP();
        int getNumPoints();
        int getTENumPoints();
        string getTEShape();
        doubles getMinParams();
        doubles getMaxParams();
        ints getAdjustableIndexes();
        TEMotion getTEMotion();
        KnotSequences getKnotSequence();
        bool isNumCPChanged(bool updateOld=true);

private:
        Bspline *bspline;
        ProfileData data;
        int oldCPnumber;
        BsplineTask *interpolationTask;
        BsplineTask *evaluationPointsTask;
        BsplineTask *evaluationPointsCompareTask;
        BsplineTask *evaluationTETask ;
        BsplineTask *evaluationMINTask ;
        BsplineTask *evaluationMAXTask ;
        BsplineTask *evaluationNormalsTask ;
        BsplineTask *evaluationTangentsTask ;
        BsplineTask *errorApproxTask ;
        BsplineTask *errorPreciseTask ;

        //Ui::MainWindow *ui;
        QTabWidget *tabLayout;

        QStackedLayout * stackedLayoutChart;
        QStackedLayout * stackedLayoutCheckBoxGroup;
        // chart widgets
        ChartView *chartView;
        // checkboxes of the chart
        QCheckBox *checkBoxOriginalPoints;
        QCheckBox *checkBoxOriginalCurve;
        QCheckBox *checkBoxInterpolatedPoints;
        QCheckBox *checkBoxInterpolatedCurve;
        QCheckBox *checkBoxCP;
        QCheckBox *checkBoxTE;
        QCheckBox *checkBoxMaxParams;
        QCheckBox *checkBoxMinParams;
        QCheckBox *checkBoxNormals;
        QCheckBox *checkBoxTangents;
        QPushButton *showErrorButton;
        QPushButton *compareErrorButton;
        // widgets of error chart
        ChartView *chartErrorView;
        QPushButton *showBsplineButton;
        QCheckBox *checkBoxErrorApprox;
        QCheckBox *checkBoxErrorPrecise;
        QLabel *labelErrorApproxMax;
        QLabel *labelErrorApproxAvg;
        QLabel *labelErrorPreciseMax;
        QLabel *labelErrorPreciseAvg;
        // progressbar
        QProgressBar *progressBar;
        QHBoxLayout *progressBarLayout;
        QLabel *progressBarLabel;

        QScatterSeries *series_originalPoints = new QScatterSeries();
        QLineSeries *series_originalCurve = new QLineSeries();
        QScatterSeries *series_interpolatePoints = new QScatterSeries();
        QLineSeries *series_interpolatedCurve = new QLineSeries();
        QScatterSeries *series_CP_fixed = new QScatterSeries();
        QScatterSeries *series_CP_adjustable = new QScatterSeries();
        QLineSeries *series_TE = new QLineSeries();
        QAreaSeries *series_maxParams = new QAreaSeries();
        QAreaSeries *series_minParams = new QAreaSeries();
        QList<QLineSeries*> series_normals = QList<QLineSeries*>();
        QList<QLineSeries*> series_tangents = QList<QLineSeries*>();
        //
        QScatterSeries *series_interpolatePointsComparison = new QScatterSeries();
        QLineSeries *series_errorApprox = new QLineSeries();
        QLineSeries *series_errorPrecise = new QLineSeries();
        QList<bool> series_visibilities;
        bool isInComparisonMode = false;

        // views for range
        QFormLayout *rangesContainerLayout;
        std::vector<RangeSliderLayout*> rangeSliders;
        QLineEdit *rangeMinEdit;
        QLineEdit *rangeMaxEdit;
        QLineEdit *minRangeAllEdit;
        QPushButton *minRangeAllButton;
        QLineEdit *maxRangeAllEdit;
        QPushButton *maxRangeAllButton;
        QLineEdit *minValueAllEdit;
        QPushButton *minValueAllButton;
        QLineEdit *maxValueAllEdit;
        QPushButton *maxValueAllButton;

        // views for bspline parameters
        QLineEdit *bspineFilenameEdit;
        QPushButton *bspineFilenameButton;
        QLineEdit *bsplineNEdit;
        QLineEdit *bsplineCPnumEdit;
        QLineEdit *bsplineNumPointsEdit;
        QComboBox *bsplineTEShapeCombo;
        QLineEdit *bsplineTENumPointsEdit;
        QComboBox *bsplineTEMotionCombo;

        // views for bottom buttons in bspline tab
        QPushButton *buttonComputeCP;
        QPushButton *buttonOptimizeKnots;
        QPushButton *buttonComputeMINMAX;
        QPushButton *buttonComputeTE;
        QCheckBox *checkBoxAutoMINMAX;
        QCheckBox *checkBoxAutoTE;

        // views for machine parameters
        QLineEdit *machineHubRadius;
        QLineEdit *machineTipRadius;
        QComboBox *machineType;
        QComboBox *machineAnalysis;

        QWidget* generateTabBspline();
        QWidget* generateTabRanges();
        QWidget* generateTabSettings();
        QLayout* generateChartLayout();
        QWidget* generateKnotChartLayout();
        QLayout* generateKnotListLayout();

        QScatterSeries *knotsSeries;
        ChartView *knotsChartView;
        QFormLayout *propLayout;
        QList<QLineEdit*> propLayoutListEdit;

        void tabChanged(int index);
        void updateRangesWidgets();
        void updateCheckBoxEnableCurve(bool forcedisablecheckbox=false);
        void checkBoxOriginalPointsChanged(int state);
        void checkBoxOriginalCurveChanged(int state);
        void checkBoxInterpolatedPointsChanged(int state);
        void checkBoxInterpolatedCurveChanged(int state);
        void checkBoxCPChanged(int state);
        void checkBoxTEChanged(int state);
        void checkBoxMaxParamsChanged(int state);
        void checkBoxMinParamsChanged(int state);
        void checkBoxNormalsChanged(int state);
        void checkBoxTangentsChanged(int state);

        void chartViewClicked(QScatterSeries* series1, QScatterSeries* series2, QPointF point, bool from1to2);
        void checkBoxAutoMINMAXChanged(int state);
        void checkBoxAutoTEChanged(int state);
        void updateMINMAX();
        void updateTE();
        void computeCP();
        void optimizeKnots();
        void updatePointsChart();

        void updateCheckBoxEnableError();
        void checkBoxErrorApproxChanged(int state);
        void checkBoxErrorPreciseChanged(int state);

        void applyAllMinRange();
        void applyAllMaxRange();
        void applyAllMinValue();
        void applyAllMaxValue();
        void loadFile();

        void updateChart();
        void updateBspline();
        void updateTangentsNormals(bool changednumcp);

        int updateKnotSeries(doubles uarray);

};

#endif // MAINWINDOW_H
