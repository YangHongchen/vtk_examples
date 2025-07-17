#ifndef PAGINATIONRESULT_H
#define PAGINATIONRESULT_H

#include <QList>
#include <QVariant>
#include <QMetaObject>
#include <QMetaProperty>
#include <QJsonObject>

template <typename T>
class PaginationResult
{
    public:
    PaginationResult(int total = 0,int pageSize = 10, int currentPage = 1, const QList<std::shared_ptr<T>>& data = {})
    : m_total(total), m_pageSize(pageSize), m_currentPage(currentPage), m_data(data) {}

// 总记录数
int total() const { return m_total; }

// 总页数
int pageCount() const {
    return m_pageSize > 0 ? (m_total + m_pageSize - 1) / m_pageSize : 0;
}

// 当前页数据
QList<std::shared_ptr<T>> data() const { return m_data; }

// 当前页码有效性
bool isValidPage() const {
    return m_currentPage >= 1 && m_currentPage <= pageCount();
}

/**
     * @brief 将数据转换为 QVariantList
     * @return
     */
QVariantList toVariantList() const {
    QVariantList result;
    for (const std::shared_ptr<T>& item : m_data) {
        if (item) {
            result.append(itemToVariant(*item));
        }
    }
    return result;
}

private:
int m_total;
int m_pageSize;
int m_currentPage;
QList<std::shared_ptr<T>> m_data; // 确保这个变量存在

/**
     * @brief itemToVariant
     * @param item
     * @return
     */
QVariant itemToVariant(const T& item) {
    QJsonObject jsonObj;
    const QMetaObject* metaObj = item.metaObject(); // 获取元对象

    for (int i = metaObj->propertyOffset(); i < metaObj->propertyCount(); ++i) {
        QMetaProperty prop = metaObj->property(i);
        jsonObj[prop.name()] = QVariant::fromValue(prop.read(&item));
    }

    return jsonObj;
}
};

#endif // PAGINATIONRESULT_H
