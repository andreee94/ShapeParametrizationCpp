#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "mychart.h"
#include "Bspline.h"
#include "mychartview.h"
#include "RangeSlider.h"
#include "rangesliderlayout.h"
#include "profiledata.h"

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

    QColor Red = QColor("#d84315");
    QColor Blue = QColor("#1e88e5"); // 2196f3
    QColor Brown = QColor("#795548");
    QColor Orange = QColor("#f57c00");

private:
    Bspline *bspline;
    ProfileData data;
    int oldCPnumber;

    //Ui::MainWindow *ui;
    QTabWidget *tabLayout;

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

    QScatterSeries *series_originalPoints = new QScatterSeries();
    QLineSeries *series_originalCurve = new QLineSeries();
    QScatterSeries *series_interpolatePoints = new QScatterSeries();
    QLineSeries *series_interpolatedCurve = new QLineSeries();
    QScatterSeries *series_CP = new QScatterSeries();
    QLineSeries *series_TE = new QLineSeries();
    QAreaSeries *series_maxParams = new QAreaSeries();
    QAreaSeries *series_minParams = new QAreaSeries();
    QList<QLineSeries*> series_normals = QList<QLineSeries*>();
    QList<QLineSeries*> series_tangents = QList<QLineSeries*>();

    // views for range
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

    int updateKnotSeries(doubles uarray);

    int getN();
    int getNumCP();
    int getNumPoints();
    int getTENumPoints();
    string getTEShape();
    doubles getMinParams();
    doubles getMaxParams();
    void appendPointsToSeries(QLineSeries *series, const Points &points);
};

#endif // MAINWINDOW_H
