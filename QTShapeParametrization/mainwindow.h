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

    static QColor Red() { return QColor("#d84315"); }
    static QColor Blue() { return QColor("#1e88e5"); } //2196f3
    static QColor Brown() { return QColor("#795548"); }
    static QColor Orange() { return QColor("#f57c00"); }
    static QColor Green() { return QColor("#43a047"); }

public slots:
        void longFunctionStart(const QString progresstext);
        void longFunctionEnd();

        void bsplineTaskStarted(const BsplineTask &task);
        void bsplineTaskFinished(const BsplineTask &task);
        void bsplineAllTasksFinished();

        void optimizationKnotsFinished(const BsplineTask &task, const Bspline &bspline);
        void interpolationCPFinished(const BsplineTask &task, const Bspline &bspline);
        void evaluationPointsFinished(const BsplineTask &task, const Points & points);
        void evaluationTEFinished(const BsplineTask &task, const Points & points);
        void evaluationMinFinished(const BsplineTask &task, const Points & points);
        void evaluationMaxFinished(const BsplineTask &task, const Points & points);
        void evaluationNormalsFinished(const BsplineTask &task, const Points & normals);
        void evaluationTangentsFinished(const BsplineTask &task, const Points & tangents);

private:
    Bspline *bspline;
    ProfileData data;
    int oldCPnumber;
    BsplineTask *interpolationTask;
    BsplineTask *evaluationPointsTask;
    BsplineTask *evaluationTETask ;
    BsplineTask *evaluationMINTask ;
    BsplineTask *evaluationMAXTask ;

    //Ui::MainWindow *ui;
    QTabWidget *tabLayout;

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
    // progressbar
    QProgressBar *progressBar;
    QHBoxLayout *progressBarLayout;
    QLabel *progressBarLabel;
    QFutureWatcher<Bspline> futureWatcherBspline;
    QFutureWatcher<Points> futureWatcherPoints;
    QFutureWatcher<void> futureWatcherVoid;
    Bspline runLongTask(QFuture<Bspline> future, QString progressMsg);
    Points runLongTask(QFuture<Points> future, QString progressMsg);
    void runLongTask(QFuture<void> future, QString progressMsg);
    void runLongTask(QFuture<void> future, QString progressMsg, const std::function <void (void)>& resultCallback);

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

    QLayoutItem* separator();
    KnotSequences getKnotSequence();

    QScatterSeries *knotsSeries;
    ChartView *knotsChartView;

    void tabChanged(int index);
    void updateRangesWidgets();
    void updateCheckBoxEnable();
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

    void checkBoxAutoMINMAXChanged(int state);
    void checkBoxAutoTEChanged(int state);
    void updateMINMAX();
    void updateTE();
    void computeCP();
    void optimizeKnots();

    void applyAllMinRange();
    void applyAllMaxRange();
    void applyAllMinValue();
    void applyAllMaxValue();
    void loadFile();

    void updateChart();
    void updateBspline();
    void updateTangentsNormals(bool changednumcp);
    void interpolatebspline();

    int updateKnotSeries(doubles uarray);

    int getN();
    int getNumCP();
    int getNumPoints();
    int getTENumPoints();
    string getTEShape();
    doubles getMinParams();
    doubles getMaxParams();
    ints getAdjustableIndexes();
    TEMotion getTEMotion();
    void appendPointsToSeries(QLineSeries *series, const Points &points);



};

#endif // MAINWINDOW_H
