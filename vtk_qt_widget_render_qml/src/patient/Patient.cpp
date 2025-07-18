#include "src/patient/Patient.h"

// 将[Patient]类注册到 QxOrm 的上下文
QX_REGISTER_CPP_PATIENT (Patient)

namespace qx {
template <> void register_class (QxClass<Patient> &t)
{
    // 注册 User::id <=> 数据库中的主键
    t.id (&Patient::id, "id");
    t.data (&Patient::firstName, "first_name");
    t.data (&Patient::lastName, "last_name");
    t.data (&Patient::fullName, "full_name");
    t.data (&Patient::gender, "gender");
    t.data (&Patient::code, "code");
    t.data (&Patient::birthday, "birthday");
    t.data (&Patient::technician, "technician");
    t.data (&Patient::email, "email");
    t.data (&Patient::phone, "phone");
    t.data (&Patient::country, "country");
    t.data (&Patient::province, "province");
    t.data (&Patient::city, "city");
    t.data (&Patient::street, "street");
    t.data (&Patient::address, "address");
    t.data (&Patient::labels, "labels");
    t.data (&Patient::comments, "comments");
    t.data (&Patient::avatar, "avatar");
    t.data (&Patient::createTime, "create_time");
    t.data (&Patient::updateTime, "update_time");
    t.data (&Patient::deleted, "deleted");
    t.data (&Patient::lastTestingTime, "last_testing_time");
    t.data (&Patient::status, "status");

    t.data (&Patient::maxillaStlUrl, "maxilla_stl_url");
    t.data (&Patient::maxillaStlThumbnailUrl, "maxilla_stl_thumbnail_url");
    t.data (&Patient::mandibleStlUrl, "mandible_stl_url");
    t.data (&Patient::mandibleStlThumbnailUrl, "mandible_stl_thumbnail_url");
    t.data (&Patient::upperDentitionStlUrl, "upper_dentition_stl_url");
    t.data (&Patient::upperDentitionStlThumbnailUrl, "upper_dentition_stl_thumbnail_url");
    t.data (&Patient::lowerDentitionStlUrl, "lower_dentition_stl_url");
    t.data (&Patient::lowerDentitionStlThumbnailUrl, "lower_dentition_stl_thumbnail_url");

}
}
