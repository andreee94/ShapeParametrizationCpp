
#include "MultiSliderView.h"
#include <QValueAxis>
#include <QtWidgets>
#include <QtCharts>
#include <vector>

namespace
{

const int scHandleSideLength = 10;
const int scSliderBarHeight = 4;
const int scMargin = 10;

}


MultiSliderView::MultiSliderView(QWidget* aParent)
    : QWidget(aParent),
      mMinimum(0),
      mMaximum(100),
//      mLowerValue(0),
//      mUpperValue(100),
//      mFirstHandlePressed(false),
//      mSecondHandlePressed(false),
      mInterval(mMaximum - mMinimum),
      mBackgroudColorEnabled(QColor(0x1E, 0x90, 0xFF)),
      mBackgroudColorDisabled(Qt::darkGray),
      mBackgroudColor(mBackgroudColorEnabled)
{
    setMouseTracking(true);
}

void MultiSliderView::paintEvent(QPaintEvent* aEvent)
{
    Q_UNUSED(aEvent);
    QPainter painter(this);

    // Background
    QRectF backgroundRect = QRectF(scMargin, (height() - scSliderBarHeight) / 2, width() - scMargin * 2, scSliderBarHeight);
    QPen pen(Qt::gray, 0.8);
    painter.setPen(pen);
    painter.setRenderHint(QPainter::Qt4CompatiblePainting);
    QBrush backgroundBrush(QColor(0xD0, 0xD0, 0xD0));
    painter.setBrush(backgroundBrush);
    painter.drawRoundedRect(backgroundRect, 1, 1);

    // First value handle rect
    pen.setColor(Qt::darkGray);
    pen.setWidth(0.5);
    painter.setPen(pen);
    painter.setRenderHint(QPainter::Antialiasing);
//    QBrush handleBrush(QColor(0xFA, 0xFA, 0xFA));
//    painter.setBrush(handleBrush);
//    QRectF leftHandleRect = firstHandleRect();
//    painter.drawRoundedRect(leftHandleRect, 2, 2);

//    // Second value handle rect
//    QRectF rightHandleRect = secondHandleRect();
//    painter.drawRoundedRect(rightHandleRect, 2, 2);

    QBrush handleBrush(mBackgroudColor); //QColor(0xFA, 0xFA, 0xFA));
    painter.setBrush(handleBrush);
    painter.setPen(QPen(Qt::black, 0.6));
    for (unsigned int i = 0; i < mValues.size(); i++)
    {
        QRectF valueRect = rectFromValue(mValues[i]);
        painter.drawEllipse(valueRect);
    }

    QValueAxis *axis = new QValueAxis;
    axis->setRange(mMinimum, mMaximum);
    axis->setTickCount(10);


    // Handles
    //painter.setRenderHint(QPainter::Antialiasing, false);
//    QRectF selectedRect(backgroundRect);
//    selectedRect.setLeft(leftHandleRect.right() + 0.5);
//    selectedRect.setRight(rightHandleRect.left() - 0.5);
//    QBrush selectedBrush(mBackgroudColor);
//    painter.setBrush(selectedBrush);
//    painter.drawRect(selectedRect);
}

double MultiSliderView::percentageFromValue(double value) const
{
    return (value - mMinimum) / mInterval;
}

QRectF MultiSliderView::rectFromValue(double value) const
{
    double perc = percentageFromValue(value);
    double left = scMargin + validWidth() * perc;
    double top = (height()-scHandleSideLength) / 2;
    return QRectF(left, top, scHandleSideLength, scHandleSideLength);
}

//void RangeSlider::mousePressEvent(QMouseEvent* aEvent)
//{
//    if(aEvent->buttons() & Qt::LeftButton)
//    {
//        mSecondHandlePressed = secondHandleRect().contains(aEvent->pos());
//        mFirstHandlePressed = !mSecondHandlePressed && firstHandleRect().contains(aEvent->pos());
//        if(mFirstHandlePressed)
//        {
//            mDelta = aEvent->pos().x() - (firstHandleRect().x() + scHandleSideLength / 2);
//        }
//        else if(mSecondHandlePressed)
//        {
//            mDelta = aEvent->pos().x() - (secondHandleRect().x() + scHandleSideLength / 2);
//        }
//        if(aEvent->pos().y() >= 2
//           && aEvent->pos().y() <= height()- 2)
//        {
//            double step = mInterval / 10; // < 1 ? 1 : mInterval / 10;
//            if(aEvent->pos().x() < firstHandleRect().x())
//            {
//                setLowerValue(mLowerValue - step);
//            }
//            else if(aEvent->pos().x() > firstHandleRect().x() + scHandleSideLength
//                    && aEvent->pos().x() < secondHandleRect().x())
//            {
//                if(aEvent->pos().x() - (firstHandleRect().x() + scHandleSideLength) <
//                   (secondHandleRect().x() - (firstHandleRect().x() + scHandleSideLength)) / 2)
//                {
//                    if(mLowerValue + step < mUpperValue)
//                    {
//                        setLowerValue(mLowerValue + step);
//                    }
//                    else
//                    {
//                        setLowerValue(mUpperValue);
//                    }
//                }
//                else
//                {
//                    if(mUpperValue - step > mLowerValue)
//                    {
//                        setUpperValue(mUpperValue - step);
//                    }
//                    else
//                    {
//                        setUpperValue(mLowerValue);
//                    }
//                }
//            }
//            else if(aEvent->pos().x() > secondHandleRect().x() + scHandleSideLength)
//            {
//                setUpperValue(mUpperValue + step);
//            }
//        }
//    }
//}

//void RangeSlider::mouseMoveEvent(QMouseEvent* aEvent)
//{
//    if(aEvent->buttons() & Qt::LeftButton)
//    {
//        if(mFirstHandlePressed)
//        {
//            if(aEvent->pos().x() - mDelta + scHandleSideLength / 2 <= secondHandleRect().x())
//            {
//                setLowerValue((aEvent->pos().x() - mDelta - scLeftRightMargin - scHandleSideLength / 2) * 1.0 / validWidth() * mInterval + mMinimum);
//            }
//            else
//            {
//                setLowerValue(mUpperValue);
//            }
//        }
//        else if(mSecondHandlePressed)
//        {
//            if(firstHandleRect().x() + scHandleSideLength * 1.5 <= aEvent->pos().x() - mDelta)
//            {
//                setUpperValue((aEvent->pos().x() - mDelta - scLeftRightMargin - scHandleSideLength / 2 - scHandleSideLength) * 1.0 / validWidth() * mInterval + mMinimum);
//            }
//            else
//            {
//                setUpperValue(mLowerValue);
//            }
//        }
//    }
//}

//void RangeSlider::mouseReleaseEvent(QMouseEvent* aEvent)
//{
//    Q_UNUSED(aEvent);

//    mFirstHandlePressed = false;
//    mSecondHandlePressed = false;
//}

void MultiSliderView::changeEvent(QEvent* aEvent)
{
    if(aEvent->type() == QEvent::EnabledChange)
    {
        if(isEnabled())
        {
            mBackgroudColor = mBackgroudColorEnabled;
        }
        else
        {
            mBackgroudColor = mBackgroudColorDisabled;
        }
        update();
    }
}

QSize MultiSliderView::minimumSizeHint() const
{
    return QSize(scHandleSideLength * 2 + scMargin * 2, scHandleSideLength + 2 * scMargin);
}

double MultiSliderView::GetMinimun() const
{
    return mMinimum;
}

void MultiSliderView::SetMinimum(double aMinimum)
{
    setMinimum(aMinimum);
}

double MultiSliderView::GetMaximun() const
{
    return mMaximum;
}

void MultiSliderView::SetMaximum(double aMaximum)
{
    setMaximum(aMaximum);
}

doubles MultiSliderView::GetValues() const
{
    return mValues;
}

void MultiSliderView::SetValues(const doubles &aValues)
{
    this->mValues = aValues;
}


void MultiSliderView::setMinimum(double aMinimum)
{
    if(aMinimum <= mMaximum)
    {
        mMinimum = aMinimum;
    }
    else
    {
        double oldMax = mMaximum;
        mMinimum = oldMax;
        mMaximum = aMinimum;
    }
    mInterval = mMaximum - mMinimum;
    update();
}

void MultiSliderView::setMaximum(double aMaximum)
{
    if(aMaximum >= mMinimum)
    {
        mMaximum = aMaximum;
    }
    else
    {
        double oldMin = mMinimum;
        mMaximum = oldMin;
        mMinimum = aMaximum;
    }
    mInterval = mMaximum - mMinimum;
    update();
}

int MultiSliderView::validWidth() const
{
    return width() - scMargin * 2 - scHandleSideLength * 2;
}

void MultiSliderView::SetRange(double aMinimum, double mMaximum)
{
    setMinimum(aMinimum);
    setMaximum(mMaximum);
}

