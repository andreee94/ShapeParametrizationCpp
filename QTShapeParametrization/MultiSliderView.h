#pragma once

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <vector>

typedef std::vector<double> doubles;

class MultiSliderView : public QWidget
{
    Q_OBJECT

public:
    MultiSliderView(QWidget* aParent = Q_NULLPTR);

    QSize minimumSizeHint() const override;

    double GetMinimun() const;
    void SetMinimum(double aMinimum);

    double GetMaximun() const;
    void SetMaximum(double a);

    doubles GetValues() const;
    void SetValues(const doubles &values);

    void SetRange(double aMinimum, double aMaximum);

protected:
    void paintEvent(QPaintEvent* aEvent) override;
//    void mousePressEvent(QMouseEvent* aEvent) override;
//    void mouseMoveEvent(QMouseEvent* aEvent) override;
//    void mouseReleaseEvent(QMouseEvent* aEvent) override;
    void changeEvent(QEvent* aEvent) override;

//    QRectF firstHandleRect() const;
//    QRectF secondHandleRect() const;
//    QRectF handleRect(int aValue) const;

//signals:
//    void lowerValueChanged(double aLowerValue);
//    void upperValueChanged(double aUpperValue);
//    void rangeChanged(double aMin, double aMax);

public slots:
//    void setLowerValue(double aLowerValue);
//    void setUpperValue(double aUpperValue);
    void setMinimum(double aMinimum);
    void setMaximum(double aMaximum);

private:
    Q_DISABLE_COPY(MultiSliderView)
    float currentPercentage();
    int validWidth() const;

    double percentageFromValue(double value) const;
    QRectF rectFromValue(double value) const;

    double mMinimum;
    double mMaximum;
    std::vector<double> mValues;
    double mInterval;
    //double mDelta;
    QColor mBackgroudColorEnabled;
    QColor mBackgroudColorDisabled;
    QColor mBackgroudColor;
};

