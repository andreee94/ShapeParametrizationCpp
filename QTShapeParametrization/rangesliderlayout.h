#ifndef RANGESLIDERLAYOUT_H
#define RANGESLIDERLAYOUT_H

#include <QWidget>
#include <QMainWindow>
#include "mychart.h"
#include "Bspline.h"
#include "mychartview.h"
#include "RangeSlider.h"

#include <QDialog>
#include <QTableView>
#include <QLayoutItem>
#include <QtWidgets>
#include <QtCharts>
#include <QLayoutItem>
#include <vector>

namespace Ui {
class RangeSliderLayout;
}

class RangeSliderLayout : public QWidget
{
    Q_OBJECT

public:
    explicit RangeSliderLayout(QWidget *parent = nullptr);

    double GetMinimun() const;
    void SetMinimum(double aMinimum);

    double GetMaximun() const;
    void SetMaximum(double aMaximum);

    double GetLowerValue() const;
    void SetLowerValue(double aLowerValue);

    double GetUpperValue() const;
    void SetUpperValue(double aUpperValue);

    void SetRange(double aMinimum, double aMaximum);

    ~RangeSliderLayout();

    bool getLetTextToIncreaseRange() const;
    void setLetTextToIncreaseRange(bool value);

signals:
    void lowerValueChanged(double aLowerValue, bool isFinished=false);
    void upperValueChanged(double aUpperValue, bool isFinished=false);
    void rangeChanged(double aMin, double aMax);

private slots:
    void changedTextValueMin(const QString &);
    void changedTextValueMax(const QString &);

private:
    void private_lowerValueChanged(double aLowerValue, bool isFinished=false);
    void private_upperValueChanged(double aUpperValue, bool isFinished=false);
    void private_rangeChanged(double aMin, double aMax);

    void changedMinMaxValue();


    RangeSlider *rangeSlider;
    QLineEdit *minEdit;
    QLineEdit *maxEdit;
    bool letTextToIncreaseRange;
    bool fromLineEdit = false;

    //Ui::RangeSliderLayout *ui;
};

#endif // RANGESLIDERLAYOUT_H
