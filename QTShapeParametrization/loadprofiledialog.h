#ifndef LOADPROFILEDIALOG_H
#define LOADPROFILEDIALOG_H

#include "profiledata.h"

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


private slots:
    void RadioCartesianSelected() { RadioSelected(FrameOfReference::CARTESIAN); }
    void RadioCylindricalSelected() { RadioSelected(FrameOfReference::CYLINDRICAL); }
    void OpenFileDialog();
    void ComboXActivated(int index);
    void ComboYActivated(int index);


private:
    const int CARTESIAN = 1;
    const int CYLINDRICAL = 2;

    //Ui::LoadProfileDia  log *ui;
    void verticalResizeTableViewToContents(QTableView *tableView);
    QStringList getColumnComboBox(int numcolumns);
    QLayoutItem* separator();

    void RadioSelected(FrameOfReference type);

    void updateTableAndChart(bool hasUpdatedFilename=false);

    QPushButton *loadButton;
    QLineEdit *loadEdit;
    QTableWidget *table;
    QChartView *chartView;
    QComboBox *comboXcol;
    QComboBox *comboYcol;
    QRadioButton *radioCartesian;
    QRadioButton *radioCylindrical;
    QPushButton *okButton;
    QPushButton *cancelButton;

    ProfileData data;
};

#endif // LOADPROFILEDIALOG_H
