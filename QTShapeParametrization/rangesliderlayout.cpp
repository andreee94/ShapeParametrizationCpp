#include "RangeSlider.h"
#include "rangesliderlayout.h"
//#include "ui_rangesliderlayout.h"

#include <QMainWindow>
#include "mychart.h"
#include "Bspline.h"
#include "mychartview.h"

#include <QDialog>
#include <QTableView>
#include <QLayoutItem>
#include <QtWidgets>
#include <QtCharts>
#include <QLayoutItem>
#include <vector>

RangeSliderLayout::RangeSliderLayout(QWidget *parent) :
    QWidget(parent)
    //ui(new Ui::RangeSliderLayout)
{
    //ui->setupUi(this);

    QHBoxLayout *mainHLayout = new QHBoxLayout;
    rangeSlider = new RangeSlider;
    minEdit = new QLineEdit;
    maxEdit = new QLineEdit;

    minEdit->setFixedWidth(50);
    maxEdit->setFixedWidth(50);

    changedMinMaxValue();

    minEdit->setText(QString::number(rangeSlider->GetLowerValue(), 'f', 2));
    maxEdit->setText(QString::number(rangeSlider->GetUpperValue(), 'f', 2));

    mainHLayout->addWidget(minEdit, 0);
    mainHLayout->addWidget(rangeSlider, 1);
    mainHLayout->addWidget(maxEdit, 0);

    letTextToIncreaseRange = false;

    setLayout(mainHLayout);

    connect(rangeSlider, &RangeSlider::lowerValueChanged, this, &RangeSliderLayout::private_lowerValueChanged);
    connect(rangeSlider, &RangeSlider::upperValueChanged, this, &RangeSliderLayout::private_upperValueChanged);
    connect(rangeSlider, &RangeSlider::rangeChanged, this, &RangeSliderLayout::private_rangeChanged);
    connect(minEdit, &QLineEdit::textChanged, this, &RangeSliderLayout::changedTextValueMin);
    connect(maxEdit, &QLineEdit::textChanged, this, &RangeSliderLayout::changedTextValueMax);

}


void RangeSliderLayout::private_lowerValueChanged(double aLowerValue, bool isFinished)
{
    if (!fromLineEdit)
        minEdit->setText(QString::number(aLowerValue, 'f', 2));
    fromLineEdit = false;
    emit lowerValueChanged(aLowerValue, isFinished);
}

void RangeSliderLayout::private_upperValueChanged(double aUpperValue, bool isFinished)
{
    if (!fromLineEdit)
        maxEdit->setText(QString::number(aUpperValue, 'f', 2));
    fromLineEdit = false;
    emit upperValueChanged(aUpperValue, isFinished);
}

void RangeSliderLayout::private_rangeChanged(double aMin, double aMax)
{
    emit rangeChanged(aMin, aMax);
}


double RangeSliderLayout::GetMinimun() const
{
    return rangeSlider->GetMinimun();
}

void RangeSliderLayout::SetMinimum(double aMinimum)
{
    rangeSlider->setMinimum(aMinimum);
    changedMinMaxValue();
}

double RangeSliderLayout::GetMaximun() const
{
    return rangeSlider->GetMaximun();
}

void RangeSliderLayout::SetMaximum(double aMaximum)
{
    rangeSlider->setMaximum(aMaximum);
    changedMinMaxValue();
}

double RangeSliderLayout::GetLowerValue() const
{
    return rangeSlider->GetLowerValue();
}

void RangeSliderLayout::SetLowerValue(double aLowerValue)
{
    rangeSlider->setLowerValue(aLowerValue);
//    minEdit->setText(QString::number(aLowerValue, 'f', 2));
}

double RangeSliderLayout::GetUpperValue() const
{
    return rangeSlider->GetUpperValue();
}

void RangeSliderLayout::SetUpperValue(double aUpperValue)
{
    rangeSlider->setUpperValue(aUpperValue);
//    maxEdit->setText(QString::number(aUpperValue, 'f', 2));
}

void RangeSliderLayout::SetRange(double aMinimum, double aMaximum)
{
    rangeSlider->SetRange(aMinimum, aMaximum);
    changedMinMaxValue();
}

void RangeSliderLayout::changedMinMaxValue()
{
    if (letTextToIncreaseRange)
    {
        minEdit->setValidator(new QDoubleValidator(rangeSlider->GetMinimun(), rangeSlider->GetMaximun(), 3, this));
        maxEdit->setValidator(new QDoubleValidator(rangeSlider->GetMinimun(), rangeSlider->GetMaximun(), 3, this));
    }
    else
    {
        minEdit->setValidator(new QDoubleValidator(-MAXFLOAT, MAXFLOAT, 3, this));
        maxEdit->setValidator(new QDoubleValidator(-MAXFLOAT, MAXFLOAT, 3, this));
    }
}

void RangeSliderLayout::changedTextValueMin(const QString & ss)
{
    bool ok(false);
    double value = ss.toDouble(&ok);

    if (ok)
        if ((value >= rangeSlider->GetMinimun() && value <= rangeSlider->GetMaximun()) || letTextToIncreaseRange)
        {
            fromLineEdit = true;
            SetLowerValue(value);
        }
}

void RangeSliderLayout::changedTextValueMax(const QString & ss)
{
    bool ok(false);
    double value = ss.toDouble(&ok);

    if (ok)
        if ((value >= rangeSlider->GetMinimun() && value <= rangeSlider->GetMaximun()) || letTextToIncreaseRange)
        {
            fromLineEdit = true;
            SetUpperValue(value);
        }
}

bool RangeSliderLayout::getLetTextToIncreaseRange() const
{
    return letTextToIncreaseRange;
}

void RangeSliderLayout::setLetTextToIncreaseRange(bool value)
{
    letTextToIncreaseRange = value;
}

RangeSliderLayout::~RangeSliderLayout()
{
    //delete ui;
}
