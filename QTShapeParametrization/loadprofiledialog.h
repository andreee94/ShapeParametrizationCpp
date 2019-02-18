#ifndef LOADPROFILEDIALOG_H
#define LOADPROFILEDIALOG_H

#include "profiledata.h"
#include "mychart.h"
#include "mychartview.h"

#include <QDialog>
#include <QTableView>
#include <QLayoutItem>
#include <QtWidgets>
#include <QtCharts>
#include <QLayoutItem>

typedef std::vector<Point> Points;
typedef std::vector<double> doubles;
typedef std::vector<string> strings;

namespace Ui {
class LoadProfileDialog;
}

class LoadProfileDialog : public QDialog
{
    Q_OBJECT

public:

    enum SelectionType{
        None,
        Points,
        First,
        Last
    };

    explicit LoadProfileDialog(QWidget *parent = nullptr);
    ~LoadProfileDialog();

    ProfileData getProfileData() const;
    void setProfileData(const ProfileData &value);

signals:
    void ok(ProfileData *data);


private slots:
    void RadioCartesianSelected() { RadioSelected(FrameOfReference::CARTESIAN); }
    void RadioCylindricalSelected() { RadioSelected(FrameOfReference::CYLINDRICAL); }
    void OpenFileDialog();
    void ComboXActivated(int index);
    void ComboYActivated(int index);
    void ComboRActivated(int index);
    void CheckReverseXChanged(int state);
    void CheckReverseYChanged(int state);
    void CheckReverseZChanged(int state);

    void checkAutoTEChanged(int state);
    void selectPointsClicked();
    void selectFirstClicked();
    void selectLastClicked();
    void resetTEClicked();

    void clickableEventList(QScatterSeries* series, QPointF point, int index);

private:

    const int CARTESIAN = 1;
    const int CYLINDRICAL = 2;

    //Ui::LoadProfileDia  log *ui;
    void verticalResizeTableViewToContents(QTableView *tableView, int maxnumrows=MAX_32BIT_INT);
    QStringList getColumnComboBox(int numcolumns);
    bool boolFromState(int state) { return state == 2 ? true : false; }

    void RadioSelected(FrameOfReference type);

    void updateTableAndChart(bool hasUpdatedFilename=false);

    QPushButton *loadButton;
    QLineEdit *loadEdit;
    QTableWidget *table;
    ChartView *chartView;
    QComboBox *comboXcol;
    QComboBox *comboYcol;
    QComboBox *comboRcol;
    QLabel *colsRLabel;
    QRadioButton *radioCartesian;
    QRadioButton *radioCylindrical;
    QCheckBox *checkboxReverseX;
    QCheckBox *checkboxReverseY;
    QCheckBox *checkboxReverseZ;
    QPushButton *okButton;
    QPushButton *cancelButton;

    ProfileData data;
    SelectionType selectionType = SelectionType::None;

    QCheckBox *checkBoxAutoTE;
    QPushButton *buttonSelectTEPoints;
    QPushButton *buttonSelectStartPoint;
    QPushButton *buttonSelectEndPoint;
    QPushButton *buttonResetSelection;
    void updateSelectionType();
    void toggleSelectionType(SelectionType type);

    QScatterSeries *seriesPoints = new QScatterSeries();
    QLineSeries *seriesLine = new QLineSeries();
    QScatterSeries *seriesTE = new QScatterSeries();
    QScatterSeries *seriesFirst = new QScatterSeries();
    QScatterSeries *seriesLast = new QScatterSeries();
    //std::optional<QPointF> firstPoint, lastPoint;

    QWidget *generateChartLayout();
    QLayout *generateLoadLayout();
    QWidget *generateTableLayout();
    QWidget *generateTabLayout();
    QLayout *generatePointsLayout();
    QLayout *generateTELayout();
    QLayout *generateBottomLayout();
};

#endif // LOADPROFILEDIALOG_H
