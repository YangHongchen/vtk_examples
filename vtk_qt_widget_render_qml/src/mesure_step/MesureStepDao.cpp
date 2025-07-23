#include "src/mesure_step/MesureStepDao.h"
#include "src/mesure_step/MesureStepStatusEnum.h"


QList<std::shared_ptr<MesureStep> > MesureStepDao::findAllStepByMesureId (long mesureId)
{
    qx::QxSqlQuery query;
    query.where ("mesure_id").isEqualTo (QVariant::fromValue (mesureId))
         .orderAsc ("priority");  // 正序
    return findAll (query);
}

bool MesureStepDao::initSteps (long mesureId, long patientId)
{
    qx::QxSession session;
    try
    {
        // TODO: 考虑国际化，翻译
        QList<QString> steps =
        {
            "左髁突位",
            "右髁突位",
            "上颌切牙位",
            "息止颌位",
            "牙尖交错",
            "后退接触",
            "最大前伸",
            "左侧方",
            "右侧方",
            "最大张口",
            "咀嚼动作"
        };
        int priority = 0;
        for (auto it = steps.begin(); it != steps.end(); ++it)
        {
            auto stepName = *it;
            MesureStep step;
            step.stepName = stepName;
            step.mesureId = mesureId;
            step.status = priority == 0 ? 1 : 0;
            step.priority = ++priority;
            step.patientId = patientId;
            step.schematicUrl = QString ("qrc:/detect/svg/detect/example/0%1.jpg").arg (priority);
            step.createTime = QDateTime::currentDateTime();
            qDebug() << "初始化step: " << stepName ;
            save (step);
        }
    }
    catch (const std::exception& e)
    {
        // 处理异常，例如打印错误信息
        qDebug() << "Save failed:" << e.what();
        return false;
    }
    return true;
}

bool MesureStepDao::checkStepExits (long MesureId)
{
    if (MesureId < 1) return false;
    qx::QxSqlQuery query;
    query.where ("mesure_id").isEqualTo (QVariant::fromValue (MesureId));
    long total = count (query);
    return total > 0;
}

std::optional<MesureStep> MesureStepDao::findOneByMesureIdActive (long MesureId)
{
    qx::QxSqlQuery query;
    query.where ("mesure_id").isEqualTo (QVariant::fromValue (MesureId))
         .and_ ("status").isEqualTo (static_cast<int> (MesureStepStatusEnum::Testing));
    return findOne (query);
}

bool MesureStepDao::updateStepNext (long MesureId, int currentStepIndex)
{
    // 更新当前步骤（状态 => 完成）
    qx::QxSqlQuery query1;
    query1.where ("mesure_id").isEqualTo (QVariant::fromValue (MesureId))
          .and_ ("priority").isEqualTo (QVariant::fromValue (currentStepIndex));

    std::optional<MesureStep> currentStep = findOne (query1);
    bool updateCurrentRet = false;
    if (currentStep)
    {
        currentStep->status = 2;
        updateCurrentRet = update (currentStep.value());
    }
    if (updateCurrentRet)
    {
        int nextStepPriority = currentStep->priority + 1;
        qx::QxSqlQuery query2;
        query2.where ("mesure_id").isEqualTo (QVariant::fromValue (MesureId))
              .and_ ("priority").isEqualTo (QVariant::fromValue (nextStepPriority));
        std::optional<MesureStep> nextStep = findOne (query2);
        bool updateNextRet = false;
        if (nextStep)
        {
            nextStep->status = 1;
            updateNextRet = update (nextStep.value());
            if (updateNextRet)
            {
                qDebug() << "更新下一步：成功";
            }
            else
            {
                qDebug() << "更新下一步：失败1";
            }
        }
        else
        {
            throw std::runtime_error ("未检测到下一步");
            return false;
        }
    }
    else
    {
        qDebug() << "更新上一步：失败！";
        return false;
    }
    return true;
}

bool MesureStepDao::updateStatus (long mesureId, int stepIndex, int status)
{
    bool updateRet = false;
    qx::QxSqlQuery queryUpdate;
    queryUpdate
    .where ("mesure_id").isEqualTo (QVariant::fromValue (mesureId))
    .and_ ("priority").isEqualTo (QVariant::fromValue (stepIndex));

    std::optional<MesureStep> step = findOne (queryUpdate);
    if (step && step->status != status)
    {
        step->status = status;
        updateRet = update (step.value());
    }
    else if (step && step->status == status)
    {
        qCritical() << "status already updated, MesureId=" << mesureId << ", step=" << stepIndex;
        updateRet = true;
    }
    else
    {
        qCritical() << "not exists testing step, MesureId=" << mesureId << ", step=" << stepIndex;
    }
    return updateRet;
}

std::optional<MesureStep> MesureStepDao::getStepByMesureIdAndPriority (long MesureId, int priority)
{
    qx::QxSqlQuery query;
    query.where ("mesure_id").isEqualTo (QVariant::fromValue (MesureId))
         .and_ ("priority").isEqualTo (priority);
    return findOne (query);
}

int MesureStepDao::checkMesureStepComplete (long MesureId)
{
    qx::QxSqlQuery query;
    query.where ("mesure_id").isEqualTo (QVariant::fromValue (MesureId));
    auto steps = findAll (query);
    if (steps.isEmpty())
    {
        qCritical() << "未查询到检测步骤";
        return -1;
    }
    // 任意一个步骤取消，则整个测试定义未取消
    if (std::any_of (steps.begin(), steps.end(), [] (const auto & step)
{
    return step->status == static_cast<int> (MesureStepStatusEnum::Cancelled);
    }))
    {
        qWarning() << "测量已取消";
        return 3;
    }

    int completeStepCount = std::count_if (steps.begin(), steps.end(), [] (const auto & step)
    {
        return step->status == static_cast<int> (MesureStepStatusEnum::Completed);
    });

    if (completeStepCount >= steps.size())
    {
        return 2;
    }
    return 1;
}

long MesureStepDao::countByMesureId (long mesureId)
{
    if (mesureId < 1)
    {
        qCritical() << "检测记录id为空";
        return 0;
    }
    qx::QxSqlQuery query;
    query.where ("mesure_id").isEqualTo (QVariant::fromValue (mesureId));
    return count (query);
}
