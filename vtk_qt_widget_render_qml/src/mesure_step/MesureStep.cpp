#include "src/mesure_step/MesureStep.h"

QX_REGISTER_CPP_MESURE_STEP (MesureStep)

namespace qx {
template <> void register_class (QxClass<MesureStep> &t)
{
    t.id (&MesureStep::id, "id");
    t.data (&MesureStep::patientId, "patient_id");
    t.data (&MesureStep::mesuireId, "mesure_id");
    t.data (&MesureStep::stepName, "step_name");
    t.data (&MesureStep::schematicUrl, "schematic_url");
    t.data (&MesureStep::status, "status");
    t.data (&MesureStep::priority, "priority");
    // common fields
    t.data (&MesureStep::createTime, "create_time");
    t.data (&MesureStep::updateTime, "update_time");
    t.data (&MesureStep::deleted, "deleted");
}
}
