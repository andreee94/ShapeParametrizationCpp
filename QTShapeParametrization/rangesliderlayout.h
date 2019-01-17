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

signals:
    void lowerValueChanged(double aLowerValue);
    void upperValueChanged(double aUpperValue);
    void rangeChanged(double aMin, double aMax);

private:
    void private_lowerValueChanged(double aLowerValue);
    void private_upperValueChanged(double aUpperValue);
    void private_rangeChanged(double aMin, double aMax);

    void changedMinMaxValue();

    void changedTextValueMin();
    void changedTextValueMax();

    RangeSlider *rangeSlider;
    QLineEdit *minEdit;
    QLineEdit *maxEdit;

    //Ui::RangeSliderLayout *ui;
};

#endif // RANGESLIDERLAYOUT_H
