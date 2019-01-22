#ifndef BSPLINETASK_H
#define BSPLINETASK_H

#include "profiledata.h"
#include <Bspline.h>
#include <memory>

#include <QMutex>
#include <QObject>
#include <QQueue>
#include <QRunnable>
#include <QMetaType>


enum BsplineTaskType
{
    OPTIMIZATION_KNOTS,
    INTERPOLATION_CP,
    EVALUATION_POINTS,
    EVALUATION_TE,
    EVALUATION_MIN,
    EVALUATION_MAX,
    EVALUATION_NORMALS,
    EVALUATION_TANGENTS
};

struct BsplineTask
{
    BsplineTask() {}

    BsplineTask(BsplineTaskType type, QString message):
        type(type), messange(message)
    {
        this->knotSequence = KnotSequences();
    }

    //mutable QMutex mutex;
    BsplineTaskType type;
    QString messange;
    bool background = true;
    bool importBSplineByPrevious = false;
    Bspline *bspline = NULL;
    ProfileData *data = NULL;
    int n = 0;
    int numCP = 0;
    KnotSequences knotSequence;
    doubles minparams;
    doubles maxparams;
    Points points;
    int TEnumPoints = 0;
    int numPoints = 0;
    string TEshape;
};

Q_DECLARE_METATYPE(BsplineTask)
//qRegisterMetaType<QVector<int> >("QVector");
//Q_DECLARE_METATYPE(BsplineTask::BsplineTask)

class BsplineTaskManager : public QObject, public QRunnable
{
    Q_OBJECT

public:

    BsplineTaskManager();
    void addTask(BsplineTask &task);
    void removeTask(BsplineTask &task);
    void removeLastTask();

    void run();

signals:
    void taskStarted(const BsplineTask &task);
    void taskFinished(const BsplineTask &task);
    void allTasksFinished();

    void optimizationKnotsFinished(const BsplineTask &task,const  Bspline &bspline);
    void interpolationCPFinished(const BsplineTask &task,const  Bspline &bspline);
    void evaluationPointsFinished(const BsplineTask &task,const  Points &points);
    void evaluationTEFinished(const BsplineTask &task,const  Points &points);
    void evaluationMinFinished(const BsplineTask &task,const  Points &points);
    void evaluationMaxFinished(const BsplineTask &task,const  Points &points);
    void evaluationNormalsFinished(const BsplineTask &task,const  Points &normals);
    void evaluationTangentsFinished(const BsplineTask &task,const  Points &tangents);

private:
    QQueue<BsplineTask> queuetasks;

    BsplineTask executeTask(BsplineTask &task);

    BsplineTask executeOptimizationKnots(BsplineTask &task);
    BsplineTask executeInterpolationCP(BsplineTask &task);
    BsplineTask executeEvaluationPoints(BsplineTask &task);
    BsplineTask executeEvaluationTE(BsplineTask &task);
    BsplineTask executeEvaluationMin(BsplineTask &task);
    BsplineTask executeEvaluationMax(BsplineTask &task);
    BsplineTask executeEvaluationNormals(BsplineTask &task);
    BsplineTask executeEvaluationTangents(BsplineTask &task);

};


#endif // BSPLINETASK_H
