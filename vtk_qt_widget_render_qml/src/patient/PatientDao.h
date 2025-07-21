#ifndef PATIENTDAO_H
#define PATIENTDAO_H
#include "src/database/BaseDao.h"
#include "src/patient/Patient.h"
#include <QRegularExpression>

class PatientDao : public BaseDao<Patient>
{
public:
    explicit PatientDao() = default;

    /**
     * @brief findOneById
     * @param id
     * @return
     */
    std::shared_ptr<Patient> findOneById(long id);

    /**
     * @brief findOneByPhone
     * @param id
     * @return
     */
    std::optional<Patient> findOneByPhone(const QString  &phone) ;

    /**
     * @brief savePatient
     * @param patient
     * @return
     */
    bool savePatient(Patient& patient);

    /**
     * @brief findAll
     * @return
     */
    QList<std::shared_ptr<Patient>> findAllPatients();

    /**
     * @brief findAllPagination
     * @param keyword
     * @param page
     * @param pageSize
     * @return
     */
    PaginationResult<Patient> findPatientsCondition (const QString keyword = "", int page = 1, int pageSize = 10);

    void test();


public:
    /**
     * @brief checkExists
     * @param patient
     * @return
     */
    bool checkExists(const Patient &patient) ;

private:

    /**
     * @brief validatePatient
     * @param patient
     * @return
     */
    bool validatePatient(Patient &patient);




};

#endif // PATIENTDAO_H
