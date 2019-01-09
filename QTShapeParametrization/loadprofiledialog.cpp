#include "loadprofiledialog.h"
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
    table = new QTableWidget(2, numcolumns);
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
    verticalResizeTableViewToContents(table);


    // choice of x and y
    QHBoxLayout *chartLayout = new QHBoxLayout;
    QLineSeries *series = new QLineSeries();
    QChart *chart = new QChart();
       chart->legend()->hide();
       chart->addSeries(series);
       chart->createDefaultAxes();
       chart->setTitle("Simple line chart example");
    chartView = new QChartView(chart);
        chartView->setRenderHint(QPainter::Antialiasing);
    QGridLayout *gridColsChoice = new QGridLayout;
    comboXcol = new QComboBox();
        comboXcol->addItems(getColumnComboBox(numcolumns));
        comboXcol->setCurrentIndex(0);
    comboYcol = new QComboBox();
        comboYcol->addItems(getColumnComboBox(numcolumns));
        comboYcol->setCurrentIndex(1);
    QLabel *colsXLabel = new QLabel("x:");
    QLabel *colsYLabel = new QLabel("y:");
    QGroupBox *coordinateGroupBox = new QGroupBox(tr("Frame of reference"));
    radioCartesian = new QRadioButton(tr("&Cartesian(x, y, z)"));
    radioCylindrical = new QRadioButton(tr("C&ylindrical(r, r*theta, z)"));
    QWidget* empty = new QWidget();
    empty->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);
    QVBoxLayout *vbox = new QVBoxLayout;
        vbox->addWidget(radioCartesian);
        vbox->addWidget(radioCylindrical);
        vbox->addStretch(1);
        coordinateGroupBox->setLayout(vbox);

    gridColsChoice->addItem(separator(), 0, 0, 2, 2);
    gridColsChoice->addWidget(colsXLabel, 0, 0, 1, 1); //, Qt::AlignTop);
    gridColsChoice->addWidget(colsYLabel, 1, 0, 1, 1); //, Qt::AlignCenter);
    gridColsChoice->addWidget(comboXcol, 0, 1, 1, 1); //, Qt::AlignTop);
    gridColsChoice->addWidget(comboYcol, 1, 1, 1, 1); //, Qt::AlignTop);
    gridColsChoice->addWidget(coordinateGroupBox, 2, 0, 2, 2); //, Qt::AlignTop);
    gridColsChoice->addWidget(empty, 4, 0, 2, 2);

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
    connect(radioCartesian,&QAbstractButton::clicked,this, &LoadProfileDialog::RadioCartesianSelected);
    connect(radioCylindrical,&QAbstractButton::clicked,this, &LoadProfileDialog::RadioCylindricalSelected);
    connect(okButton, &QAbstractButton::clicked, this, &QDialog::accept);
    connect(cancelButton, &QAbstractButton::clicked, this, &QDialog::reject);



    mainLayout->addLayout(loadLayout, Qt::AlignTop);
    mainLayout->addItem(separator());
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
            comboXcol->setCurrentIndex(data.getColumnX());
            comboYcol->setCurrentIndex(data.getColumnY());
            table->setColumnCount(numcolumns);
            table->update();
            for (int i = 0; i < numcolumns; i++)
            {
                table->setItem(0, i, new QTableWidgetItem(QString::number(data.Value(0, i))));
                table->setItem(1, i, new QTableWidgetItem(QString::number(data.Value(1, i))));
            }
        }
        QLineSeries *series = new QLineSeries();
        Points points = data.getPoints();
        for (unsigned int i = 0; i < points.size(); i++)
        {
            series->append(points[i].getx(), points[i].gety());
        }
        chartView->chart()->removeAllSeries();
        chartView->chart()->addSeries(series);
    }
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


void LoadProfileDialog::RadioSelected(FrameOfReference type)
{
    if (type != this->data.getFrameOfReference())
    {
        data.setFrameOfReference(type);
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

QLayoutItem* LoadProfileDialog::separator()
{
    QSpacerItem* item = new QSpacerItem(10, 10, QSizePolicy::Minimum, QSizePolicy::Expanding);
    return item;
//    QFrame* line = new QFrame();
//    line->setFrameShape(QFrame::HLine);
//    line->setFrameShadow(QFrame::Sunken);
//    return line;
}

void LoadProfileDialog::verticalResizeTableViewToContents(QTableView *tableView)
{
    int rowTotalHeightMin=0;
    int rowTotalHeightMax=0;

    // Rows height
    int count=tableView->verticalHeader()->count();
    for (int i = 0; i < count; ++i) {
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
