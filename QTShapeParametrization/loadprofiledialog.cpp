#include "loadprofiledialog.h"
#include "qtutils.h"
#include "string"
//#include "ui_loadprofiledialog.h"

#include <QtWidgets>
#include <QtCharts>
#include <QLayoutItem>

using namespace std;


LoadProfileDialog::LoadProfileDialog(QWidget *parent) :
    QDialog(parent)
    //,ui(new Ui::LoadProfileDialog)
{
    int numcolumns = 5;
    //ui->setupUi(this);
    // a big vertical layoput which contains everything
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    // horizontal layout for filename
    QHBoxLayout *loadLayout = new QHBoxLayout;
    QLabel *loadLabel = new QLabel("File:");
    loadEdit = new QLineEdit("");
    loadButton = new QPushButton("Choose");
        loadLayout->addWidget(loadLabel);
        loadLayout->addWidget(loadEdit);
        loadLayout->addWidget(loadButton);

    // table of example of columns
    table = new QTableWidget(3, numcolumns);
        table->setEditTriggers(QAbstractItemView::NoEditTriggers); // not editable
    QHeaderView* headerH = table->horizontalHeader();
    QHeaderView* headerV = table->verticalHeader();
        headerH->setSectionResizeMode(QHeaderView::Stretch); // stretch horizontally
        headerV->setSectionResizeMode(QHeaderView::Fixed); // non stretch vertically
//    for (int i = 0; i < numcolumns; i++)
//    {
//        QTableWidgetItem *item = new QTableWidgetItem("");
//        item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled|Qt::ItemFlag::ItemIsUserCheckable);
//        item->setCheckState(Qt::Checked);
//        item->setStyleSheet("margin-left:50%; margin-right:50%;");
//        table->setItem(0, i, item);
//    }
//    table->setVerticalHeaderLabels({"X", "Y", "1", "2"});
    verticalResizeTableViewToContents(table, 3);


    // choice of x and y
    QHBoxLayout *chartLayout = new QHBoxLayout;
    QLineSeries *series = new QLineSeries();
    Chart *chart = new Chart();
       chart->legend()->hide();
       chart->addSeries(series);
       chart->createDefaultAxes();
       chart->setTitle("Profile plot");
       chart->setMargins(QMargins(4,4,4,4));
    chartView = new ChartView();
        chartView->setChart(chart);
        chartView->setRenderHint(QPainter::Antialiasing);
        chartView->setDirectionZoom(ChartView::BothDirectionZoom);
        chartView->setRubberBand(new QRubberBand(QRubberBand::Shape::Rectangle));
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

    chartLayout->addWidget(chartView, 1);
    chartLayout->addLayout(gridColsChoice, 0);



    // bottom buttons
    QHBoxLayout *bottomLayout = new QHBoxLayout;
    okButton = new QPushButton("OK");
    cancelButton = new QPushButton("Cancel");
        bottomLayout->addWidget(okButton, 1, Qt::AlignRight);
        bottomLayout->addWidget(cancelButton);

    connect(loadButton, &QPushButton::clicked, this, &LoadProfileDialog::OpenFileDialog);
    connect(comboXcol, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &LoadProfileDialog::ComboXActivated);
    connect(comboYcol, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &LoadProfileDialog::ComboYActivated);
    connect(comboRcol, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &LoadProfileDialog::ComboRActivated);
    connect(radioCartesian, &QAbstractButton::clicked,this, &LoadProfileDialog::RadioCartesianSelected);
    connect(radioCylindrical, &QAbstractButton::clicked,this, &LoadProfileDialog::RadioCylindricalSelected);
    connect(checkboxReverseX,QOverload<int>::of(&QCheckBox::stateChanged),this, &LoadProfileDialog::CheckReverseXChanged);
    connect(checkboxReverseY,QOverload<int>::of(&QCheckBox::stateChanged),this, &LoadProfileDialog::CheckReverseYChanged);
    connect(okButton, &QAbstractButton::clicked, this, &QDialog::accept);
    connect(cancelButton, &QAbstractButton::clicked, this, &QDialog::reject);



    mainLayout->addLayout(loadLayout, Qt::AlignTop);
    mainLayout->addItem(QTUtils::separator());
    mainLayout->addWidget(table);
    mainLayout->addLayout(chartLayout, 1);
    //mainLayout->addStretch(1);
    mainLayout->addLayout(bottomLayout, Qt::AlignRight | Qt::AlignBottom);
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
        QLineSeries *series = new QLineSeries();
        QScatterSeries *seriesPoints = new QScatterSeries();
        Points points = data.getPoints();
        for (unsigned int i = 0; i < points.size(); i++)
        {
            series->append(points[i].getx(), points[i].gety());
            seriesPoints->append(points[i].getx(), points[i].gety());
        }
//        series->setPointsVisible(true);
//        series->setPen(QPen(Qt::darkBlue, 2));
        series->setColor(QTUtils::Black()); //series->pen().color());
        seriesPoints->setMarkerSize(6);
        seriesPoints->setColor(QTUtils::Black()); //series->pen().color());
        seriesPoints->setBorderColor(QTUtils::Black()); //series->pen().color());
        chartView->chart()->removeAllSeries();
        chartView->chart()->addSeries(series);
        chartView->chart()->addSeries(seriesPoints);
        chartView->chart()->createDefaultAxes();
    }
}

void LoadProfileDialog::setProfileData(const ProfileData &value)
{
    data = value;
    updateTableAndChart(true);
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
    }
}

void LoadProfileDialog::ComboYActivated(int index)
{
    if (index != this->data.getColumnY() && index >=0 )
    {
        data.setColumnY(index);
        this->updateTableAndChart(false);
    }
}

void LoadProfileDialog::ComboRActivated(int index)
{
    if (index != this->data.getColumnR() && index >=0 )
    {
        data.setColumnR(index);
        this->updateTableAndChart(false);
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
    }
}

void LoadProfileDialog::CheckReverseXChanged(int state)
{
    if (this->boolFromState(state) != this->data.getReverseX())
    {
        data.setReverseX(boolFromState(state));
        this->updateTableAndChart(false);
    }
}

void LoadProfileDialog::CheckReverseYChanged(int state)
{
    if (this->boolFromState(state) != this->data.getReverseY())
    {
        data.setReverseY(boolFromState(state));
        this->updateTableAndChart(false);
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

void LoadProfileDialog::OpenFileDialog()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Profile Data"), "/home/andrea", tr("Every Text Files (* *.txt)"));
    if (fileName.toUtf8().constData() != this->data.getFileName())
    {
        loadEdit->setText(fileName);
        data.setFile(fileName.toUtf8().constData());
        updateTableAndChart(true);
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
