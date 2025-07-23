#ifndef PATIENTOBJECT_H
#define PATIENTOBJECT_H

#include <QObject>
#include <QString>
#include <QDateTime>
#include "src/patient/Patient.h"

/**
 * @brief 病例数据模型封装类 PatientObject
 * （主要用于映射表数据类，供QML访问属性）
 */
class PatientObject : public QObject {
    Q_OBJECT

    // 基本信息
    Q_PROPERTY (long id READ id WRITE setId NOTIFY idChanged)
    Q_PROPERTY (QString firstName READ firstName WRITE setFirstName NOTIFY firstNameChanged)
    Q_PROPERTY (QString lastName READ lastName WRITE setLastName NOTIFY lastNameChanged)
    Q_PROPERTY (QString fullName READ fullName WRITE setFullName NOTIFY fullNameChanged)
    Q_PROPERTY (int gender READ gender WRITE setGender NOTIFY genderChanged)
    Q_PROPERTY (QString code READ code WRITE setCode NOTIFY codeChanged)
    Q_PROPERTY (QString birthDay READ birthDay WRITE setBirthDay NOTIFY birthDayChanged)
    Q_PROPERTY (int age READ age  NOTIFY ageChanged)
    Q_PROPERTY (QString technician READ technician WRITE setTechnician NOTIFY technicianChanged)
    Q_PROPERTY (QString email READ email WRITE setEmail NOTIFY emailChanged)
    Q_PROPERTY (QString phone READ phone WRITE setPhone NOTIFY phoneChanged)

    // 地址信息
    Q_PROPERTY (QString country READ country WRITE setCountry NOTIFY countryChanged)
    Q_PROPERTY (QString province READ province WRITE setProvince NOTIFY provinceChanged)
    Q_PROPERTY (QString city READ city WRITE setCity NOTIFY cityChanged)
    Q_PROPERTY (QString street READ street WRITE setStreet NOTIFY streetChanged)
    Q_PROPERTY (QString address READ address WRITE setAddress NOTIFY addressChanged)

    // 其他信息
    Q_PROPERTY (QString labels READ labels WRITE setLabels NOTIFY labelsChanged)
    Q_PROPERTY (QString comments READ comments WRITE setComments NOTIFY commentsChanged)
    Q_PROPERTY (QString avatar READ avatar WRITE setAvatar NOTIFY avatarChanged)
    Q_PROPERTY (QString createTime READ createTime WRITE setCreateTime NOTIFY createTimeChanged)
    Q_PROPERTY (QString updateTime READ updateTime WRITE setUpdateTime NOTIFY updateTimeChanged)
    Q_PROPERTY (QString lastTestingTime READ lastTestingTime WRITE setLastTestingTime NOTIFY lastTestingTimeChanged)
    Q_PROPERTY (int deleted READ deleted WRITE setDeleted NOTIFY deletedChanged)
    Q_PROPERTY (int status READ status WRITE setStatus NOTIFY statusChanged)
    Q_PROPERTY (QString statusCN READ statusCN NOTIFY statusChanged)
    // STL文件信息
    Q_PROPERTY (QString maxillaStlUrl READ maxillaStlUrl WRITE setMaxillaStlUrl NOTIFY maxillaStlUrlChanged)
    Q_PROPERTY (QString maxillaStlThumbnailUrl READ maxillaStlThumbnailUrl WRITE setMaxillaStlThumbnailUrl NOTIFY
                maxillaStlThumbnailUrlChanged)
    Q_PROPERTY (QString mandibleStlUrl READ mandibleStlUrl WRITE setMandibleStlUrl NOTIFY mandibleStlUrlChanged)
    Q_PROPERTY (QString mandibleStlThumbnailUrl READ mandibleStlThumbnailUrl WRITE setMandibleStlThumbnailUrl NOTIFY
                mandibleStlThumbnailUrlChanged)
    Q_PROPERTY (QString upperDentitionStlUrl READ upperDentitionStlUrl WRITE setUpperDentitionStlUrl NOTIFY
                upperDentitionStlUrlChanged)
    Q_PROPERTY (QString upperDentitionStlThumbnailUrl READ upperDentitionStlThumbnailUrl WRITE
                setUpperDentitionStlThumbnailUrl NOTIFY upperDentitionStlThumbnailUrlChanged)
    Q_PROPERTY (QString lowerDentitionStlUrl READ lowerDentitionStlUrl WRITE setLowerDentitionStlUrl NOTIFY
                lowerDentitionStlUrlChanged)
    Q_PROPERTY (QString lowerDentitionStlThumbnailUrl READ lowerDentitionStlThumbnailUrl WRITE
                setLowerDentitionStlThumbnailUrl NOTIFY lowerDentitionStlThumbnailUrlChanged)


  public:
    explicit PatientObject (QObject *parent = nullptr) : QObject (parent) {}
    // getter/setter + signal 模板，请参考示范实现

    long id() const { return m_id; }
    void setId (long v)
    {
        if (m_id == v) return;
        m_id = v; emit idChanged();
    }

    QString firstName() const { return m_firstName; }
    void setFirstName (const QString &v)
    {
        if (m_firstName == v) return;
        m_firstName = v; emit firstNameChanged();
    }

    QString lastName() const { return m_lastName; }
    void setLastName (const QString &v)
    {
        if (m_lastName == v) return;
        m_lastName = v; emit lastNameChanged();
    }

    QString fullName() const { return m_fullName; }
    void setFullName (const QString &v)
    {
        if (m_fullName == v) return;
        m_fullName = v; emit fullNameChanged();
    }

    int gender() const { return m_gender; }
    void setGender (int v)
    {
        if (m_gender == v) return;
        m_gender = v; emit genderChanged();
    }

    QString code() const { return m_code; }
    void setCode (const QString &v)
    {
        if (m_code == v) return;
        m_code = v; emit codeChanged();
    }

    QString birthDay() const { return m_birthDay; }
    void setBirthDay (const QString &v)
    {
        if (m_birthDay == v) return;
        m_birthDay = v; emit birthDayChanged();
    }

    int age()
    {
        int age = 0;
        // 1. 转换字符串为QDate
        QDate birthDate = QDate::fromString (m_birthDay, "yyyy-MM-dd"); // 根据实际格式调整
        // 验证日期有效性
        if (!birthDate.isValid())
        {
            qWarning() << "Invalid birth date:" << m_birthDay;
            return age; // 返回错误码
        }
        // 2. 获取当前日期
        QDate currentDate = QDate::currentDate();
        // 3. 计算基础年龄
        age = currentDate.year()  - birthDate.year();
        // 4. 精确调整
        if (currentDate.month()  < birthDate.month()  || (currentDate.month()  == birthDate.month()  &&
                currentDate.day()  < birthDate.day()))
        {
            age--; // 未过生日减1岁
        }
        return age;
    }

    QString technician() const { return m_technician; }
    void setTechnician (const QString &v)
    {
        if (m_technician == v) return;
        m_technician = v; emit technicianChanged();
    }

    QString email() const { return m_email; }
    void setEmail (const QString &v)
    {
        if (m_email == v) return;
        m_email = v; emit emailChanged();
    }

    QString phone() const { return m_phone; }
    void setPhone (const QString &v)
    {
        if (m_phone == v) return;
        m_phone = v; emit phoneChanged();
    }

    QString country() const { return m_country; }
    void setCountry (const QString &v)
    {
        if (m_country == v) return;
        m_country = v; emit countryChanged();
    }

    QString province() const { return m_province; }
    void setProvince (const QString &v)
    {
        if (m_province == v) return;
        m_province = v; emit provinceChanged();
    }

    QString city() const { return m_city; }
    void setCity (const QString &v)
    {
        if (m_city == v) return;
        m_city = v; emit cityChanged();
    }

    QString street() const { return m_street; }
    void setStreet (const QString &v)
    {
        if (m_street == v) return;
        m_street = v; emit streetChanged();
    }

    QString address() const { return m_address; }
    void setAddress (const QString &v)
    {
        if (m_address == v) return;
        m_address = v; emit addressChanged();
    }

    QString labels() const { return m_labels; }
    void setLabels (const QString &v)
    {
        if (m_labels == v) return;
        m_labels = v; emit labelsChanged();
    }

    QString comments() const { return m_comments; }
    void setComments (const QString &v)
    {
        if (m_comments == v) return;
        m_comments = v; emit commentsChanged();
    }

    QString avatar() const { return m_avatar; }
    void setAvatar (const QString &v)
    {
        if (m_avatar == v) return;
        m_avatar = v; emit avatarChanged();
    }

    QString createTime() const { return m_createTime; }
    void setCreateTime (const QString &v)
    {
        if (m_createTime == v) return;
        m_createTime = v; emit createTimeChanged();
    }

    QString updateTime() const { return m_updateTime; }
    void setUpdateTime (const QString &v)
    {
        if (m_updateTime == v) return;
        m_updateTime = v; emit updateTimeChanged();
    }

    QString lastTestingTime() const { return m_lastTestingTime; }
    void setLastTestingTime (const QString &v)
    {
        if (m_lastTestingTime == v) return;
        m_lastTestingTime = v; emit lastTestingTimeChanged();
    }

    int deleted() const { return m_deleted; }
    void setDeleted (int v)
    {
        if (m_deleted == v) return;
        m_deleted = v; emit deletedChanged();
    }

    int status() const { return m_status; }
    void setStatus (int v)
    {
        if (m_status == v) return;
        m_status = v; emit statusChanged();
    }

    QString statusCN()
    {
        QString _status = "未知";
        switch (m_status)
        {
        case 1: _status = "指导治疗前"; break;
        case 2: _status = "治疗中"; break;
        case 3: _status = "治疗完成"; break;

        }
        return _status;
    }

    QString maxillaStlUrl() const { return m_maxillaStlUrl; }
    void setMaxillaStlUrl (const QString &v)
    {
        if (m_maxillaStlUrl == v) return;
        m_maxillaStlUrl = v; emit maxillaStlUrlChanged();
    }

    QString maxillaStlThumbnailUrl() const { return m_maxillaStlThumbnailUrl; }
    void setMaxillaStlThumbnailUrl (const QString &v)
    {
        if (m_maxillaStlThumbnailUrl == v) return;
        m_maxillaStlThumbnailUrl = v; emit maxillaStlThumbnailUrlChanged();
    }

    QString mandibleStlUrl() const { return m_mandibleStlUrl; }
    void setMandibleStlUrl (const QString &v)
    {
        if (m_mandibleStlUrl == v) return;
        m_mandibleStlUrl = v; emit mandibleStlUrlChanged();
    }

    QString mandibleStlThumbnailUrl() const { return m_mandibleStlThumbnailUrl; }
    void setMandibleStlThumbnailUrl (const QString &v)
    {
        if (m_mandibleStlThumbnailUrl == v) return;
        m_mandibleStlThumbnailUrl = v; emit mandibleStlThumbnailUrlChanged();
    }

    QString upperDentitionStlUrl() const { return m_upperDentitionStlUrl; }
    void setUpperDentitionStlUrl (const QString &v)
    {
        if (m_upperDentitionStlUrl == v) return;
        m_upperDentitionStlUrl = v; emit upperDentitionStlUrlChanged();
    }

    QString upperDentitionStlThumbnailUrl() const { return m_upperDentitionStlThumbnailUrl; }
    void setUpperDentitionStlThumbnailUrl (const QString &v)
    {
        if (m_upperDentitionStlThumbnailUrl == v) return;
        m_upperDentitionStlThumbnailUrl = v; emit upperDentitionStlThumbnailUrlChanged();
    }

    QString lowerDentitionStlUrl() const { return m_lowerDentitionStlUrl; }
    void setLowerDentitionStlUrl (const QString &v)
    {
        if (m_lowerDentitionStlUrl == v) return;
        m_lowerDentitionStlUrl = v; emit lowerDentitionStlUrlChanged();
    }

    QString lowerDentitionStlThumbnailUrl() const { return m_lowerDentitionStlThumbnailUrl; }
    void setLowerDentitionStlThumbnailUrl (const QString &v)
    {
        if (m_lowerDentitionStlThumbnailUrl == v) return;
        m_lowerDentitionStlThumbnailUrl = v; emit lowerDentitionStlThumbnailUrlChanged();
    }


  signals:
    void idChanged();
    void firstNameChanged();
    void lastNameChanged();
    void fullNameChanged();
    void genderChanged();
    void codeChanged();
    void birthDayChanged();
    void ageChanged();
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

  private:
    long m_id = 0;
    QString m_firstName;
    QString m_lastName;
    QString m_fullName;
    int m_gender = 0;
    QString m_code;
    QString m_birthDay;
    QString m_technician;
    QString m_email;
    QString m_phone;

    QString m_country;
    QString m_province;
    QString m_city;
    QString m_street;
    QString m_address;

    QString m_labels;
    QString m_comments;
    QString m_avatar;
    QString m_createTime;
    QString m_updateTime;
    QString m_lastTestingTime;
    int m_deleted = 0;
    int m_status = 0;

    QString m_maxillaStlUrl;
    QString m_maxillaStlThumbnailUrl;
    QString m_mandibleStlUrl;
    QString m_mandibleStlThumbnailUrl;
    QString m_upperDentitionStlUrl;
    QString m_upperDentitionStlThumbnailUrl;
    QString m_lowerDentitionStlUrl;
    QString m_lowerDentitionStlThumbnailUrl;

  public:
    // 工具方法，更新当前PateintObject，基于查询的Patient
    static  PatientObject *toPatientObject (const Patient &patient, QObject *parent = nullptr)
    {
        auto obj = new PatientObject (parent);
        obj->m_id = patient.id;
        obj->m_firstName = patient.firstName;
        obj->m_lastName = patient.lastName;
        obj->m_fullName = patient.fullName;
        obj->m_gender = patient.gender;
        obj->m_code = patient.code;
        obj->m_birthDay = patient.birthday.date().toString (Qt::ISODate);
        obj->m_technician = patient.technician;
        obj->m_email = patient.email;
        obj->m_phone = patient.phone;

        obj->m_country = patient.country;
        obj->m_province = patient.province;
        obj->m_city = patient.city;
        obj->m_street = patient.street;
        obj->m_address = patient.address;

        obj->m_labels = patient.labels;
        obj->m_comments = patient.comments;
        obj->m_avatar = patient.avatar;
        obj->m_createTime = patient.createTime.date().toString (Qt::ISODate);
        obj->m_updateTime = patient.updateTime.date().toString (Qt::ISODate);
        obj->m_lastTestingTime = patient.lastTestingTime.date().toString (Qt::ISODate);
        obj->m_deleted = patient.deleted;
        obj->m_status = patient.status;

        obj->m_maxillaStlUrl = patient.maxillaStlUrl;
        obj->m_maxillaStlThumbnailUrl = patient.maxillaStlThumbnailUrl;
        obj->m_mandibleStlUrl = patient.mandibleStlUrl;
        obj->m_mandibleStlThumbnailUrl = patient.mandibleStlThumbnailUrl;
        obj->m_upperDentitionStlUrl = patient.upperDentitionStlUrl;
        obj->m_upperDentitionStlThumbnailUrl = patient.upperDentitionStlThumbnailUrl;
        obj->m_lowerDentitionStlUrl = patient.lowerDentitionStlUrl;
        obj->m_lowerDentitionStlThumbnailUrl = patient.lowerDentitionStlThumbnailUrl;
        return obj;
    }
};

#endif // PATIENTOBJECT_H
