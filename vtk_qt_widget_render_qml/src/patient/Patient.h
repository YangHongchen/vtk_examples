#ifndef PATIENT_H
#define PATIENT_H
#include "src/database/export.h"

/**
 * @brief 病例数据库表模型类
 * （QxORM写法，不建议集成QObject以及其他复杂的逻辑）
 */
class PATIENT_DLL_EXPORT Patient {
  public:
    Patient() : id (0) { }
    virtual ~Patient() { }

    long id;                    // ID主键
    QString firstName;          // 名
    QString lastName;           // 姓
    QString fullName;           // 姓名
    int gender = 0;             // 性别: 默认未知
    QString code;               // 编码（自定义）
    QDateTime birthday;         // 生日
    QString technician;         // 技术员
    QString email;              // 邮箱
    QString phone;              // 电话
    QString country;            // 国家
    QString province;           // 省份
    QString city;               // 城市
    QString street;             // 街道
    QString address;            // 详细地址
    QString labels;             // 标签
    QString comments;           // 备注
    QString avatar;             // 上传头像文件URL
    QDateTime createTime;       // 创建时间
    QDateTime updateTime;       // 更新时间
    QDateTime lastTestingTime;  // 最近检查时间
    int deleted = 0;            // 软删除标识：0未删除，1删除
    int status  = 0;            // 状态：0未删除，1删除

    QString maxillaStlUrl;                  // 上颌骨 STL文件URL
    QString maxillaStlThumbnailUrl;         // 上颌骨 STL文件缩略图URL
    QString mandibleStlUrl;                 // 下颌骨 STL文件URL
    QString mandibleStlThumbnailUrl;        // 下颌骨 STL文件缩略图URL
    QString upperDentitionStlUrl;           // 上牙列 STL文件UR
    QString upperDentitionStlThumbnailUrl;  // 上牙列 STL文件缩略图URL
    QString lowerDentitionStlUrl;           // 下牙列 STL文件URL
    QString lowerDentitionStlThumbnailUrl;  // 下牙列 STL文件缩略图URL
};

QX_REGISTER_HPP_PATIENT (Patient, qx::trait::no_base_class_defined, 1)


#endif // PATIENT_H
