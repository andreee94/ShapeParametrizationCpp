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

namespace Ui {
class LoadProfileDialog;
}

class LoadProfileDialog : public QDialog
{
    Q_OBJECT

public:
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
};

#endif // LOADPROFILEDIALOG_H
