#ifndef PATIENT_H
#define PATIENT_H
#include "src/database/export.h"
// #include <QObject>

class PATIENT_DLL_EXPORT Patient {
    // Q_OBJECT

    // // 使用 MEMBER 自动绑定属性
    // Q_PROPERTY(long id MEMBER id NOTIFY idChanged)
    // Q_PROPERTY(QString firstName MEMBER firstName NOTIFY firstNameChanged)
    // Q_PROPERTY(QString lastName MEMBER lastName NOTIFY lastNameChanged)
    // Q_PROPERTY(QString fullName MEMBER fullName NOTIFY fullNameChanged)
    // Q_PROPERTY(int gender MEMBER gender NOTIFY genderChanged)
    // Q_PROPERTY(QString code MEMBER code NOTIFY codeChanged)
    // Q_PROPERTY(QDateTime birthday MEMBER birthday NOTIFY birthdayChanged)
    // Q_PROPERTY(QString technician MEMBER technician NOTIFY technicianChanged)
    // Q_PROPERTY(QString email MEMBER email NOTIFY emailChanged)
    // Q_PROPERTY(QString phone MEMBER phone NOTIFY phoneChanged)
    // Q_PROPERTY(QString country MEMBER country NOTIFY countryChanged)
    // Q_PROPERTY(QString province MEMBER province NOTIFY provinceChanged)
    // Q_PROPERTY(QString city MEMBER city NOTIFY cityChanged)
    // Q_PROPERTY(QString street MEMBER street NOTIFY streetChanged)
    // Q_PROPERTY(QString address MEMBER address NOTIFY addressChanged)
    // Q_PROPERTY(QString labels MEMBER labels NOTIFY labelsChanged)
    // Q_PROPERTY(QString comments MEMBER comments NOTIFY commentsChanged)
    // Q_PROPERTY(QString avatar MEMBER avatar NOTIFY avatarChanged)
    // Q_PROPERTY(QDateTime createTime MEMBER createTime NOTIFY createTimeChanged)
    // Q_PROPERTY(QDateTime updateTime MEMBER updateTime NOTIFY updateTimeChanged)
    // Q_PROPERTY(QDateTime lastTestingTime MEMBER lastTestingTime NOTIFY lastTestingTimeChanged)
    // Q_PROPERTY(int deleted MEMBER deleted NOTIFY deletedChanged)
    // Q_PROPERTY(int status MEMBER status NOTIFY statusChanged)
    // Q_PROPERTY(QString maxillaStlUrl MEMBER maxillaStlUrl NOTIFY maxillaStlUrlChanged)
    // Q_PROPERTY(QString maxillaStlThumbnailUrl MEMBER maxillaStlThumbnailUrl NOTIFY maxillaStlThumbnailUrlChanged)
    // Q_PROPERTY(QString mandibleStlUrl MEMBER mandibleStlUrl NOTIFY mandibleStlUrlChanged)
    // Q_PROPERTY(QString mandibleStlThumbnailUrl MEMBER mandibleStlThumbnailUrl NOTIFY mandibleStlThumbnailUrlChanged)
    // Q_PROPERTY(QString upperDentitionStlUrl MEMBER upperDentitionStlUrl NOTIFY upperDentitionStlUrlChanged)
    // Q_PROPERTY(QString upperDentitionStlThumbnailUrl MEMBER upperDentitionStlThumbnailUrl NOTIFY upperDentitionStlThumbnailUrlChanged)
    // Q_PROPERTY(QString lowerDentitionStlUrl MEMBER lowerDentitionStlUrl NOTIFY lowerDentitionStlUrlChanged)
    // Q_PROPERTY(QString lowerDentitionStlThumbnailUrl MEMBER lowerDentitionStlThumbnailUrl NOTIFY lowerDentitionStlThumbnailUrlChanged)

  signals:
    // 所有属性的变更信号
    void idChanged();
    void firstNameChanged();
    void lastNameChanged();
    void fullNameChanged();
    void genderChanged();
    void codeChanged();
    void birthdayChanged();
    void technicianChanged();
    void emailChanged();
    void phoneChanged();
    void countryChanged();
    void provinceChanged();
    void cityChanged();
    void streetChanged();
    void addressChanged();
    void labelsChanged();
    void commentsChanged();
    void avatarChanged();
    void createTimeChanged();
    void updateTimeChanged();
    void lastTestingTimeChanged();
    void deletedChanged();
    void statusChanged();
    void maxillaStlUrlChanged();
    void maxillaStlThumbnailUrlChanged();
    void mandibleStlUrlChanged();
    void mandibleStlThumbnailUrlChanged();
    void upperDentitionStlUrlChanged();
    void upperDentitionStlThumbnailUrlChanged();
    void lowerDentitionStlUrlChanged();
    void lowerDentitionStlThumbnailUrlChanged();

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
