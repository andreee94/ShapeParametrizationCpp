#include "loadprofiledialog.h"
#include "qtutils.h"
#include "string"
//#include "ui_loadprofiledialog.h"

#include <QtWidgets>
#include <QtCharts>
#include <QLayoutItem>

using namespace std;
typedef std::vector<double> doubles;
typedef std::vector<int> ints;
typedef std::vector<bool> bools;
typedef std::vector<string> strings;
typedef std::vector<Point> Points;


LoadProfileDialog::LoadProfileDialog(QWidget *parent) :
    QDialog(parent)
    //,ui(new Ui::LoadProfileDialog)
{
    //int numcolumns = 5;
    //ui->setupUi(this);
    // a big vertical layoput which contains everything
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // choice of x and y
    QHBoxLayout *chartLayout = new QHBoxLayout;
    chartLayout->addWidget(generateChartLayout(), 1);
    chartLayout->addWidget(generateTabLayout(), 0);

    mainLayout->addLayout(generateLoadLayout(), Qt::AlignTop);
    mainLayout->addItem(QTUtils::separator());
    mainLayout->addWidget(generateTableLayout());
    mainLayout->addLayout(chartLayout, 1);
    //mainLayout->addStretch(1);
    mainLayout->addLayout(generateBottomLayout(), Qt::AlignRight | Qt::AlignBottom);
    setLayout(mainLayout);
    setWindowTitle(tr("Load Profile Data"));
    resize(640, 560);
}


void LoadProfileDialog::updateTableAndChart(bool hasUpdatedFilename)
{
    if (this->data.getFileName().size() > 0)
    {
        if (hasUpdatedFilename)
        {
            int numcolumns = this->data.getDataColumnsNum();
            comboXcol->clear();
            comboYcol->clear();
            comboXcol->addItems(getColumnComboBox(numcolumns));
            comboYcol->addItems(getColumnComboBox(numcolumns));
            comboRcol->addItems(getColumnComboBox(numcolumns));
            comboXcol->setCurrentIndex(data.getColumnX());
            comboYcol->setCurrentIndex(data.getColumnY());
            table->setColumnCount(numcolumns);
            table->setRowCount(data.getDataRowsNum());
            table->update();
            for (int i = 0; i < numcolumns; i++)
            {
                for (int j = 0; j < data.getDataRowsNum(); j++)
                {
                    table->setItem(j, i, new QTableWidgetItem(QString::number(data.Value(j, i))));
                }
//                table->setItem(0, i, new QTableWidgetItem(QString::number(data.Value(0, i))));
//                table->setItem(1, i, new QTableWidgetItem(QString::number(data.Value(1, i))));
            }
        }
        seriesLine->clear();
        seriesPoints->clear();
        seriesTE->clear();
        seriesFirst->clear();
        seriesLast->clear();
        vector<Point> points = data.getPoints(true);
        for (unsigned int i = 0; i < points.size(); i++)
        {
            seriesLine->append(points[i].getx(), points[i].gety());
            seriesPoints->append(points[i].getx(), points[i].gety());
        }
        if (! chartView->chart()->series().contains(seriesLine) )
            chartView->chart()->addSeries(seriesLine);
        if (! chartView->chart()->series().contains(seriesPoints) )
            chartView->chart()->addSeries(seriesPoints);
        if (! chartView->chart()->series().contains(seriesTE) )
            chartView->chart()->addSeries(seriesTE);
        chartView->chart()->createDefaultAxes();
    }
}

void LoadProfileDialog::updateSelectionType()
{
    QColor color;
    if (selectionType == SelectionType::Points)
        color = QTUtils::Red();
    else if (selectionType == SelectionType::First)
        color = QTUtils::Green();
    else if (selectionType == SelectionType::Last)
        color = QTUtils::Blue();
    QString styleSelected = "QPushButton {background: " + QTUtils::color2str(color) +";}";

    buttonSelectTEPoints->setStyleSheet(selectionType == SelectionType::Points ? styleSelected : "");
    buttonSelectStartPoint->setStyleSheet(selectionType == SelectionType::First ? styleSelected : "");
    buttonSelectEndPoint->setStyleSheet(selectionType == SelectionType::Last ? styleSelected : "");
}

void LoadProfileDialog::toggleSelectionType(LoadProfileDialog::SelectionType type)
{
    if (selectionType == type)
        selectionType = SelectionType::None;
    else
        selectionType = type;
    updateSelectionType();
}

void LoadProfileDialog::setProfileData(const ProfileData &value)
{
    data = value;
    updateTableAndChart(true);
    resetTEClicked();
}

ProfileData LoadProfileDialog::getProfileData() const
{
    return data;
}

void LoadProfileDialog::ComboXActivated(int index)
{
    if (index != this->data.getColumnX() && index >=0 )
    {
        data.setColumnX(index);
        this->updateTableAndChart(false);
        resetTEClicked();
    }
}

void LoadProfileDialog::ComboYActivated(int index)
{
    if (index != this->data.getColumnY() && index >=0 )
    {
        data.setColumnY(index);
        this->updateTableAndChart(false);
        resetTEClicked();
    }
}

void LoadProfileDialog::ComboRActivated(int index)
{
    if (index != this->data.getColumnR() && index >=0 )
    {
        data.setColumnR(index);
        this->updateTableAndChart(false);
        resetTEClicked();
    }
}

void LoadProfileDialog::RadioSelected(FrameOfReference type)
{
    if (type != this->data.getFrameOfReference())
    {
        data.setFrameOfReference(type);
        this->updateTableAndChart(false);
        colsRLabel->setEnabled(type == FrameOfReference::CYLINDRICAL);
        comboRcol->setEnabled(type == FrameOfReference::CYLINDRICAL);
        checkboxReverseZ->setEnabled(type == FrameOfReference::CYLINDRICAL);
        resetTEClicked();
    }
}

void LoadProfileDialog::CheckReverseXChanged(int state)
{
    if (this->boolFromState(state) != this->data.getReverseX())
    {
        data.setReverseX(boolFromState(state));
        this->updateTableAndChart(false);
        resetTEClicked();
    }
}

void LoadProfileDialog::CheckReverseYChanged(int state)
{
    if (this->boolFromState(state) != this->data.getReverseY())
    {
        data.setReverseY(boolFromState(state));
        this->updateTableAndChart(false);
        resetTEClicked();
    }
}

void LoadProfileDialog::CheckReverseZChanged(int state)
{
    if (this->boolFromState(state) != this->data.getReverseZ())
    {
        data.setReverseZ(boolFromState(state));
        this->updateTableAndChart(false);
    }
}

void LoadProfileDialog::checkAutoTEChanged(int state)
{
    buttonSelectTEPoints->setEnabled(state == Qt::CheckState::Unchecked);
    if (selectionType == SelectionType::Points)
    {
        selectionType = SelectionType::None;
        updateSelectionType();
    }
}

void LoadProfileDialog::selectPointsClicked()
{
    toggleSelectionType(SelectionType::Points);
}

void LoadProfileDialog::selectFirstClicked()
{
    toggleSelectionType(SelectionType::First);
}

void LoadProfileDialog::selectLastClicked()
{
    toggleSelectionType(SelectionType::Last);
}

void LoadProfileDialog::resetTEClicked()
{
    seriesTE->clear();
    seriesLast->clear();
    seriesFirst->clear();
    seriesPoints->clear();
    data.firstPoint.reset();
    data.lastPoint.reset();
    if (this->data.getFileName().size() > 0)
        QTUtils::appendPointsToSeries(seriesPoints, data.getPoints(true));
    selectionType = SelectionType::None;
    updateSelectionType();
}

void LoadProfileDialog::clickableEventList(QScatterSeries *series, QPointF point, int index)
{
    if (selectionType != SelectionType::None)
    {
        if (selectionType == SelectionType::Points)
        {
            if (series == seriesPoints)
            {
                seriesPoints->remove(index);
                seriesTE->append(point);
            }
            else if (series == seriesTE)
            {
                seriesTE->remove(index);
                seriesPoints->append(point);
            }
        }
        else if (selectionType == SelectionType::First)
        {
            if (series == seriesFirst)
            {
                seriesFirst->remove(index);
                seriesPoints->append(point);
                data.firstPoint.reset();
            }
            else
            {
                series->remove(index);
                if (data.firstPoint) // if a first point already available, remove it and append it to default  series
                {
                    seriesFirst->remove(data.firstPoint.value());
                    seriesPoints->append(data.firstPoint.value());
                }
                seriesFirst->append(point);
                data.firstPoint = point;
            }
        }
        else if (selectionType == SelectionType::Last)
        {
            if (series == seriesLast)
            {
                seriesLast->remove(index);
                seriesPoints->append(point);
                data.lastPoint.reset();
            }
            else
            {
                if (data.lastPoint) // if a first point already available, remove it and append it to default  series
                {
                    seriesLast->remove(data.lastPoint.value());
                    seriesPoints->append(data.lastPoint.value());
                }
                series->remove(index);
                seriesLast->append(point);
                data.lastPoint = point;
            }
        }
        // update TEpoints automatically
        if (checkBoxAutoTE->isChecked())
        {
            if (data.firstPoint && data.lastPoint) // both first and last point selected
            {
                vector<Point> curvePoints;
                vector<Point> TEPoints;
                vector<Point> points = data.getPoints(true);
                Point p1 = Point(data.firstPoint.value().x(), data.firstPoint.value().y());
                Point p2 = Point(data.lastPoint.value().x(), data.lastPoint.value().y());
                data.setFirstPointIndex(Utils::indexof(points, p1));
                data.setLastPointIndex(Utils::indexof(points, p2));
                data.computePointsAndTE(curvePoints, TEPoints);
                seriesPoints->clear();
                seriesTE->clear();
                QTUtils::appendPointsToSeries(seriesPoints, curvePoints);
                QTUtils::appendPointsToSeries(seriesTE, TEPoints);
            }
        }
    }
}

void LoadProfileDialog::OpenFileDialog()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Profile Data"), "/home/andrea", tr("Every Text Files (* *.txt)"));
    if (fileName.toUtf8().constData() != this->data.getFileName())
    {
        loadEdit->setText(fileName);
        data.setFile(fileName.toUtf8().constData());
        updateTableAndChart(true);
        resetTEClicked();
    }
}


QStringList LoadProfileDialog::getColumnComboBox(int numcolumns)
{
    QStringList list;
    for (int i = 0; i < numcolumns; i++)
    {
        list <<  QString("Column ").append(QString::number(i + 1));
    }
    return list;
}

void LoadProfileDialog::verticalResizeTableViewToContents(QTableView *tableView, int maxnumrows)
{
    int rowTotalHeightMin=0;
    int rowTotalHeightMax=0;

    // Rows height
    int count=tableView->verticalHeader()->count();
    for (int i = 0; i < min(count, maxnumrows); ++i) {
        // 2018-03 edit: only account for row if it is visible
        if (!tableView->verticalHeader()->isSectionHidden(i)) {
            rowTotalHeightMin+=tableView->verticalHeader()->sectionSize(i);
            rowTotalHeightMax+=tableView->verticalHeader()->sectionSize(i);
        }
    }

    // Check for scrollbar visibility
    if (!tableView->horizontalScrollBar()->isHidden())
    {
        rowTotalHeightMax+=tableView->horizontalScrollBar()->height() / 2;
    }

    // Check for header visibility
    if (!tableView->horizontalHeader()->isHidden())
    {
        rowTotalHeightMin+=tableView->horizontalHeader()->height();
        rowTotalHeightMax+=tableView->horizontalHeader()->height();
    }
    tableView->setMinimumHeight(rowTotalHeightMin);
    tableView->setMaximumHeight(rowTotalHeightMax);
}

LoadProfileDialog::~LoadProfileDialog()
{
    //delete ui;
}



QWidget *LoadProfileDialog::generateChartLayout()
{
    QLineSeries *series = new QLineSeries();
    Chart *chart = new Chart();
       chart->legend()->hide();
       chart->setTitle("Profile plot");
       chart->addSeries(series);
       chart->createDefaultAxes();
       chart->setMargins(QMargins(4,4,4,4));

    chartView = new ChartView();
        chartView->setChart(chart);
        chartView->setRenderHint(QPainter::Antialiasing);
        chartView->setDirectionScroll(ChartView::BothDirectionScroll);
        chartView->setDirectionZoom(ChartView::BothDirectionZoom);
        chartView->setRubberBand(new QRubberBand(QRubberBand::Shape::Rectangle));

    connect(chartView,QOverload<QScatterSeries*, QPointF, int>::of(&ChartView::clickableEventList),this, &LoadProfileDialog::clickableEventList);

    chartView->chart()->addSeries(seriesPoints);
    chartView->chart()->addSeries(seriesLine);
    chartView->chart()->addSeries(seriesTE);
    chartView->chart()->addSeries(seriesFirst);
    chartView->chart()->addSeries(seriesLast);
    chartView->chart()->createDefaultAxes();

    chartView->addClickable(seriesPoints);
    chartView->addClickable(seriesTE);
    chartView->addClickable(seriesFirst);
    chartView->addClickable(seriesLast);


    seriesLine->setColor(QTUtils::Black());

    seriesPoints->setMarkerSize(6);
    seriesPoints->setColor(QTUtils::Black());
    seriesPoints->setBorderColor(QTUtils::Black());

    seriesTE->setColor(QTUtils::Red());
    seriesTE->setBorderColor(QTUtils::Red());
    seriesTE->setMarkerSize(8);

    seriesFirst->setColor(QTUtils::Green());
    seriesFirst->setBorderColor(QTUtils::Green());
    seriesFirst->setMarkerSize(12);

    seriesLast->setColor(QTUtils::Blue());
    seriesLast->setBorderColor(QTUtils::Blue());
    seriesLast->setMarkerSize(12);

    return chartView;
}

QLayout *LoadProfileDialog::generateLoadLayout()
{
    int numcolumns = 5;
    QHBoxLayout *loadLayout = new QHBoxLayout;
    QLabel *loadLabel = new QLabel("File:");
    loadEdit = new QLineEdit("");
    loadButton = new QPushButton("Choose");

    loadLayout->addWidget(loadLabel);
    loadLayout->addWidget(loadEdit);
    loadLayout->addWidget(loadButton);

    connect(loadButton, &QPushButton::clicked, this, &LoadProfileDialog::OpenFileDialog);

    return loadLayout;
}

QWidget *LoadProfileDialog::generateTableLayout()
{
    int numcolumns = 5;

    // table of example of columns
    table = new QTableWidget(3, numcolumns);
        table->setEditTriggers(QAbstractItemView::NoEditTriggers); // not editable
    QHeaderView* headerH = table->horizontalHeader();
    QHeaderView* headerV = table->verticalHeader();
        headerH->setSectionResizeMode(QHeaderView::Stretch); // stretch horizontally
        headerV->setSectionResizeMode(QHeaderView::Fixed); // non stretch vertically
    verticalResizeTableViewToContents(table, 3);

    return table;
}

QWidget *LoadProfileDialog::generateTabLayout()
{
    QWidget *widgetPoints = new QWidget;
    widgetPoints->setLayout(generatePointsLayout());

    QWidget *widgetTE = new QWidget;
    widgetTE->setLayout(generateTELayout());

    QTabWidget *tabLayout = new QTabWidget;
    tabLayout->addTab(widgetPoints, tr("Coordinates"));
    tabLayout->addTab(widgetTE, tr("Trailing Edge"));
    return tabLayout;
}

QLayout *LoadProfileDialog::generatePointsLayout()
{
    int numcolumns = 5;
    QGridLayout *gridColsChoice = new QGridLayout;
    comboXcol = new QComboBox();
        comboXcol->addItems(getColumnComboBox(numcolumns));
        comboXcol->setCurrentIndex(0);
    comboYcol = new QComboBox();
        comboYcol->addItems(getColumnComboBox(numcolumns));
        comboYcol->setCurrentIndex(1);
    comboRcol = new QComboBox();
        comboRcol->addItems(getColumnComboBox(numcolumns));
        comboRcol->setCurrentIndex(0);
    QLabel *colsXLabel = new QLabel("x:");
    QLabel *colsYLabel = new QLabel("y:");
    colsRLabel = new QLabel("R: (max of)");
    QHBoxLayout *colsRLayout = new QHBoxLayout;
        colsRLayout->addWidget(colsRLabel);
        colsRLayout->addWidget(comboRcol);
        colsRLabel->setEnabled(false);
        comboRcol->setEnabled(false);
    QGroupBox *coordinateGroupBox = new QGroupBox(tr("Frame of reference"));
    radioCartesian = new QRadioButton(tr("&Cartesian(x, y, z)"));
        radioCartesian->setChecked(true);
    radioCylindrical = new QRadioButton(tr("C&ylindrical(r, r*theta, z)"));
    QGroupBox *reverseGroupBox = new QGroupBox(tr("Reverse x or y coordinates"));
    checkboxReverseX = new QCheckBox(tr("Reverse X Coordinate"));
    checkboxReverseY = new QCheckBox(tr("Reverse Y Coordinate"));
    checkboxReverseZ = new QCheckBox(tr("Reverse Z Coordinate"));
        checkboxReverseZ->setEnabled(false);
    QWidget* empty = new QWidget();
    empty->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);
    QVBoxLayout *vbox_framereference = new QVBoxLayout;
        vbox_framereference->addWidget(radioCartesian);
        vbox_framereference->addWidget(radioCylindrical);
        vbox_framereference->addLayout(colsRLayout);
        vbox_framereference->addStretch(1);
        coordinateGroupBox->setLayout(vbox_framereference);
    QVBoxLayout *vbox_reverse = new QVBoxLayout;
        vbox_reverse->addWidget(checkboxReverseX);
        vbox_reverse->addWidget(checkboxReverseY);
        vbox_reverse->addWidget(checkboxReverseZ);
        vbox_reverse->addStretch(1);
        reverseGroupBox->setLayout(vbox_reverse);

    gridColsChoice->addItem(QTUtils::separator(), 0, 0, 2, 2);
    gridColsChoice->addWidget(colsXLabel, 0, 0, 1, 1); //, Qt::AlignTop);
    gridColsChoice->addWidget(colsYLabel, 1, 0, 1, 1); //, Qt::AlignCenter);
    gridColsChoice->addWidget(comboXcol, 0, 1, 1, 1); //, Qt::AlignTop);
    gridColsChoice->addWidget(comboYcol, 1, 1, 1, 1); //, Qt::AlignTop);
    gridColsChoice->addWidget(coordinateGroupBox, 2, 0, 2, 2); //, Qt::AlignTop);
    gridColsChoice->addWidget(reverseGroupBox, 4, 0, 2, 2); //, Qt::AlignTop);
    gridColsChoice->addWidget(empty, 6, 0, 2, 2);

    connect(comboXcol, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &LoadProfileDialog::ComboXActivated);
    connect(comboYcol, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &LoadProfileDialog::ComboYActivated);
    connect(comboRcol, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &LoadProfileDialog::ComboRActivated);
    connect(radioCartesian, &QAbstractButton::clicked,this, &LoadProfileDialog::RadioCartesianSelected);
    connect(radioCylindrical, &QAbstractButton::clicked,this, &LoadProfileDialog::RadioCylindricalSelected);
    connect(checkboxReverseX,QOverload<int>::of(&QCheckBox::stateChanged),this, &LoadProfileDialog::CheckReverseXChanged);
    connect(checkboxReverseY,QOverload<int>::of(&QCheckBox::stateChanged),this, &LoadProfileDialog::CheckReverseYChanged);

    return gridColsChoice;
}

QLayout *LoadProfileDialog::generateTELayout()
{
    QVBoxLayout *boxLayout = new QVBoxLayout;
    checkBoxAutoTE = new QCheckBox("Automatic Points Selection");
    buttonSelectTEPoints = new QPushButton("Select TE Points");
    buttonSelectStartPoint = new QPushButton("Select first Point");
    buttonSelectEndPoint = new QPushButton("Select last Point");
    buttonResetSelection = new QPushButton("Reset");
    boxLayout->addWidget(buttonSelectStartPoint);
    boxLayout->addWidget(buttonSelectEndPoint);
    boxLayout->addItem(QTUtils::separator());
    boxLayout->addWidget(checkBoxAutoTE);
    boxLayout->addWidget(buttonSelectTEPoints);
    boxLayout->addStretch(1);
    boxLayout->addWidget(buttonResetSelection, 0, Qt::AlignRight);

    checkBoxAutoTE->setChecked(true); // default auto is checked
    checkAutoTEChanged(checkBoxAutoTE->isChecked());

    connect(checkBoxAutoTE,QOverload<int>::of(&QCheckBox::stateChanged),this, &LoadProfileDialog::checkAutoTEChanged);
    connect(buttonSelectTEPoints, &QPushButton::clicked, this, &LoadProfileDialog::selectPointsClicked);
    connect(buttonSelectStartPoint, &QPushButton::clicked, this, &LoadProfileDialog::selectFirstClicked);
    connect(buttonSelectEndPoint, &QPushButton::clicked, this, &LoadProfileDialog::selectLastClicked);
    connect(buttonResetSelection, &QPushButton::clicked, this, &LoadProfileDialog::resetTEClicked);
    return boxLayout;
}

QLayout *LoadProfileDialog::generateBottomLayout()
{
    // bottom buttons
    QHBoxLayout *bottomLayout = new QHBoxLayout;
    okButton = new QPushButton("OK");
    cancelButton = new QPushButton("Cancel");
        bottomLayout->addWidget(okButton, 1, Qt::AlignRight);
        bottomLayout->addWidget(cancelButton);
    connect(okButton, &QAbstractButton::clicked, this, &QDialog::accept);
    connect(cancelButton, &QAbstractButton::clicked, this, &QDialog::reject);
    return bottomLayout;

}
