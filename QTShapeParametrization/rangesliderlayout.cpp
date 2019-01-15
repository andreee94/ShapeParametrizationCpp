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

    minEdit->setValidator(new QDoubleValidator(rangeSlider->GetMinimun(), rangeSlider->GetMaximun(), 3, this));
    minEdit->setValidator(new QDoubleValidator(rangeSlider->GetMinimun(), rangeSlider->GetMaximun(), 3, this));

    mainHLayout->addWidget(minEdit, 0);
    mainHLayout->addWidget(rangeSlider, 1);
    mainHLayout->addWidget(maxEdit, 0);

    setLayout(mainHLayout);

    connect(rangeSlider, &RangeSlider::lowerValueChanged, this, &RangeSliderLayout::private_lowerValueChanged);
    connect(rangeSlider, &RangeSlider::upperValueChanged, this, &RangeSliderLayout::private_upperValueChanged);
    connect(rangeSlider, &RangeSlider::rangeChanged, this, &RangeSliderLayout::private_rangeChanged);

}


void RangeSliderLayout::private_lowerValueChanged(double aLowerValue)
{
    minEdit->setText(QString::number(aLowerValue, 'f', 2));
    emit lowerValueChanged(aLowerValue);
}

void RangeSliderLayout::private_upperValueChanged(double aUpperValue)
{
    maxEdit->setText(QString::number(aUpperValue, 'f', 2));
    emit upperValueChanged(aUpperValue);
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
}

double RangeSliderLayout::GetUpperValue() const
{
    return rangeSlider->GetUpperValue();
}

void RangeSliderLayout::SetUpperValue(double aUpperValue)
{
    rangeSlider->setUpperValue(aUpperValue);
}

void RangeSliderLayout::SetRange(double aMinimum, double aMaximum)
{
    rangeSlider->SetRange(aMinimum, aMaximum);
    changedMinMaxValue();
}

void RangeSliderLayout::changedMinMaxValue()
{
    minEdit->setValidator(new QDoubleValidator(rangeSlider->GetMinimun(), rangeSlider->GetMaximun(), 3, this));
    maxEdit->setValidator(new QDoubleValidator(rangeSlider->GetMinimun(), rangeSlider->GetMaximun(), 3, this));
}

RangeSliderLayout::~RangeSliderLayout()
{
    //delete ui;
}
