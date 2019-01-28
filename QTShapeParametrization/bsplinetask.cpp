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
        case INTERPOLATION_CP:
            return executeInterpolationCP(task);
        case EVALUATION_POINTS:
            return executeEvaluationPoints(task);
        case EVALUATION_POINTS_COMPARE:
            return executeEvaluationPointsCompare(task);
        case EVALUATION_TE:
            return executeEvaluationTE(task);
        case EVALUATION_MIN:
            return executeEvaluationMin(task);
        case EVALUATION_MAX:
            return executeEvaluationMax(task);
        case EVALUATION_NORMALS:
            return executeEvaluationNormals(task);
        case EVALUATION_TANGENTS:
            return executeEvaluationTangents(task);
        case EVALUATION_ERRORS_APPROX:
            return executeErrorApprox(task);
        case EVALUATION_ERRORS_PRECISE:
            return executeErrorPrecise(task);
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

BsplineTask BsplineTaskManager::executeEvaluationPointsCompare(BsplineTask &task)
{
    doubles dd = Utils::centripetal(task.data->getPoints(), 1);
    Points points = task.bspline->evaluate(dd);//task.numPoints);
    task.points = points;
    emit evaluationPointsCompareFinished(task, points);
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

BsplineTask BsplineTaskManager::executeErrorApprox(BsplineTask &task)
{
    Points original = task.data->getPoints();
    doubles us = Utils::centripetal(original, 1);
    doubles error = task.bspline->evaluateError(original, true);
    emit evaluationErrorApproxFinished(task, us, error);
    return task;
}

BsplineTask BsplineTaskManager::executeErrorPrecise(BsplineTask &task)
{
    Points original = task.data->getPoints();
    doubles us = Utils::centripetal(original, 1);
    doubles error = task.bspline->evaluateError(original, false);
    emit evaluationErrorPreciseFinished(task, us, error);
    return task;
}


