#include "bsplinetask.h"
#include <vector>
#include "Utils.h"
#include "Bspline.h"

BsplineTaskManager::BsplineTaskManager()
{

}

void BsplineTaskManager::addTask(BsplineTask &task)
{
    queuetasks.enqueue(task);
}

void BsplineTaskManager::removeTask(BsplineTask &task)
{
    //queuetasks.removeOne(task);
}

void BsplineTaskManager::removeLastTask()
{
    queuetasks.dequeue();
}

void BsplineTaskManager::run()
{
    Bspline* latest = NULL;
    while(!queuetasks.isEmpty())
    {
        BsplineTask task = queuetasks.dequeue();
        if (task.importBSplineByPrevious && latest)
            task.bspline = latest;

        //QMutexLocker locker(&task->mutex);
        emit taskStarted(task);

        task = executeTask(task);
        latest = task.bspline ? new Bspline(*task.bspline) : NULL;

        emit taskFinished(task);
    }
    emit allTasksFinished();
}

BsplineTask BsplineTaskManager::executeTask(BsplineTask &task)
{
    switch (task.type)
    {
        case OPTIMIZATION_KNOTS:
            return executeOptimizationKnots(task);
            break;
        case INTERPOLATION_CP:
            return executeInterpolationCP(task);
            break;
        case EVALUATION_POINTS:
            return executeEvaluationPoints(task);
            break;
        case EVALUATION_TE:
            return executeEvaluationTE(task);
            break;
        case EVALUATION_MIN:
            return executeEvaluationMin(task);
            break;
        case EVALUATION_MAX:
            return executeEvaluationMax(task);
            break;
        case EVALUATION_NORMALS:
            return executeEvaluationNormals(task);
            break;
        case EVALUATION_TANGENTS:
            return executeEvaluationNormals(task);
            break;
    }
}

BsplineTask BsplineTaskManager::executeOptimizationKnots(BsplineTask &task)
{
    return task;
}

BsplineTask BsplineTaskManager::executeInterpolationCP(BsplineTask &task)
{
    Bspline tempbspline = Bspline::interpolate(task.data->getPoints(), task.numCP, task.n, task.knotSequence);
    task.bspline = new Bspline(tempbspline);
    emit interpolationCPFinished(task, tempbspline);
    return task;
}

BsplineTask BsplineTaskManager::executeEvaluationPoints(BsplineTask &task)
{
    Points points = task.bspline->evaluate(task.numPoints);
    task.points = points;
    emit evaluationPointsFinished(task, points);
    return task;
}

BsplineTask BsplineTaskManager::executeEvaluationTE(BsplineTask &task)
{
    Points points = task.bspline->evaluateTE(task.TEnumPoints, task.TEshape, true);
    task.points = points;
    emit evaluationTEFinished(task, points);
    return task;
}

BsplineTask BsplineTaskManager::executeEvaluationMin(BsplineTask &task)
{
    Points points = task.bspline->getExtremeMin(task.minparams).evaluateWithTE(task.numPoints, task.TEnumPoints, task.TEshape, true);
    task.points = points;
    emit evaluationMinFinished(task, points);
    return task;
}

BsplineTask BsplineTaskManager::executeEvaluationMax(BsplineTask &task)
{
    Points points = task.bspline->getExtremeMax(task.maxparams).evaluateWithTE(task.numPoints, task.TEnumPoints, task.TEshape, true);
    task.points = points;
    emit evaluationMaxFinished(task, points);
    return task;
}

BsplineTask BsplineTaskManager::executeEvaluationNormals(BsplineTask &task)
{
    Points normals = task.bspline->getNormalsInCP();
    emit evaluationNormalsFinished(task, normals);
    return task;
}

BsplineTask BsplineTaskManager::executeEvaluationTangents(BsplineTask &task)
{
    Points tangents = task.bspline->getTangentsInCP();
    emit evaluationTangentsFinished(task, tangents);
    return task;
}


