#include "src/mesure/MesureRecord.h"

QX_REGISTER_CPP_MESURE_RECORD (MesureRecord)

namespace qx {
template <> void register_class (QxClass<MesureRecord> &t)
{
    t.id (&MesureRecord::id, "id");
    t.data (&MesureRecord::patientId, "patient_id");
    t.data (&MesureRecord::doctor, "doctor");
    t.data (&MesureRecord::status, "status");
    t.data (&MesureRecord::comments, "comments");
    t.data (&MesureRecord::reportUrl, "report_url");
    // common fields
    t.data (&MesureRecord::createTime, "create_time");
    t.data (&MesureRecord::updateTime, "update_time");
    t.data (&MesureRecord::deleted, "deleted");
}
}
