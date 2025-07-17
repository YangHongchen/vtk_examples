#ifndef BASEDAO_H
#define BASEDAO_H
#include <QxOrm.h>
#include <QDebug>

#include "src/database/DBManager.h"
#include "src/database/PaginationResult.h"

template <typename T>

class BaseDao
{
  public:
      explicit BaseDao() : m_db (DBManager::instance().getDatabase()) {}

      /**
         * @brief 保存数据（插入）
         * @param entity
         * @return
      */
      bool save (T& entity)
      {
          QSqlError sqlError = qx::dao::insert (entity, &m_db);
          if (sqlError.type() != QSqlError::NoError)
          {
              qDebug() << "[DB_SAVE_ERROR]: " << sqlError.text();
              return false;
          }
          return true;
      }

      bool saveBatch (const std::vector<T> &entities)
      {
          for (auto t : entities)
          {
              QSqlError sqlError = qx::dao::insert (t, &m_db);
              if (sqlError.type() != QSqlError::NoError)
              {
                  qDebug() << "[DB_SAVE_BATCH_ERROR]: " << sqlError.text();
                  return false;
              }
          }
          return true;
      }

      /**
     * @brief 更新数据
     * @param entity
     * @return
     */
      bool update (T& entity)
      {
          QSqlError sqlError = qx::dao::update (entity, &m_db);
          if (sqlError.type() != QSqlError::NoError)
          {
              qDebug() << "[DB_UPDATE_ERROR]: " << m_db.lastError().text();
              return false;
          }
          return true;
      }

      /**
     * @brief 通过查询条件更新
     * @param query
     * @param t
     * @return
     */
      bool updateByQuery (qx::QxSqlQuery &query,  T &t, const QStringList &columns)
      {
          for (const QString &ss : columns)
          {
              qDebug() << "[通过查询条件更新]: " << ss;
          }

          QSqlError sqlError = qx::dao::update_by_query (query, t, &m_db, columns);
          if (sqlError.type() != QSqlError::NoError)
          {
              qCritical() << "[DB_UPDATE_BY_QUERY_ERROR]" << sqlError.text();
              return false;
          }
          return true;
      }

      /**
     * @brief 通过id删除数据
     * @param id
     * @return
     */
      bool deleteById (long int id)
      {
          T t;
          t.id = id;
          QSqlError sqlError = qx::dao::delete_by_id<T> (t, &m_db);
          if (sqlError.type() != QSqlError::NoError)
          {
              qDebug() << "[DB_DELETE_BY_ID_ERROR]: " << m_db.lastError().text();
              return false;
          }
          return true;
      }

      /**
     * @brief 通过id获取数据
     * @param id
     * @return
     */
      std::shared_ptr<T> findById (long int id)
      {
          T t; // 创建T类型的对象
          t.id = id; // 假设T有setId方法
          QSqlError sqlError = qx::dao::fetch_by_id<T> (t, &m_db);
          if (sqlError.type() != QSqlError::NoError)
          {
              qDebug() << "[DB_FIND_BY_ID_ERROR]: " << m_db.lastError().text();
          }
          return std::make_shared<T> (t);
      }

      /**
     * 查询全部
     * @brief findAll
     * @return
     */
      QList<std::shared_ptr<T>> findAll (const qx::QxSqlQuery &query = qx::QxSqlQuery())
      {
          qx::QxCollection<long, T> collection;
          QSqlError sqlError = qx::dao::fetch_by_query (query, collection, &m_db);
          if (sqlError.type() != QSqlError::NoError)
          {
              qCritical() << "[DB_FETCH_ALL_ERROR]" << m_db.lastError().text();
              return QList<std::shared_ptr<T>>();
          }
          // 使用 get_list() 访问数据
          QList<std::shared_ptr<T>> result;
          for (auto it = collection.begin(); it != collection.end(); ++it)
          {
              result.append (std::make_shared<T> (it->second));
          }
          return result;
      }

      /**
     * @brief findByQuery
     * @param query
     * @param pageIndex
     * @param pageSize
     * @return
     */
      PaginationResult<T> findAllPagination (qx::QxSqlQuery& query, int pageIndex = 1, int pageSize = 10)
      {
          pageIndex = qMax (1, pageIndex);
          pageSize = qMax (1, pageSize);
          // 获取总记录数
          const qx::QxSqlQuery countQuery = qx::QxSqlQuery (query);
          long total = count (query);
          // 设置分页limit条件
          qx::QxSqlQuery paginatedQuery (query);
          int offset = (pageIndex  - 1) * pageSize;
          paginatedQuery.limit (pageSize, offset);
          // 执行查询
          qx::QxCollection<long, T> collection;
          QSqlError sqlError = qx::dao::fetch_by_query (paginatedQuery, collection, &m_db);
          // 错误处理
          if (sqlError.type() != QSqlError::NoError)
          {
              qCritical() << "[DB_PAGINATION_ERROR]" << sqlError.text();
              return PaginationResult<T>();
          }
          // 转换数据结构
          QList<std::shared_ptr<T>> dataList;
          for (auto it = collection.begin();  it != collection.end();  ++it)
          {
              dataList.append (std::make_shared<T> (it->second));
          }
          return PaginationResult<T> (total, pageSize, pageIndex, dataList);
      }

      // 模板函数直接调用 QxOrm 实现的 count 函数
      long count (const qx::QxSqlQuery &query = qx::QxSqlQuery())
      {
          return qx::dao::detail::QxDao_Count<T>::count (query, &m_db);
      }

      // 模板函数直接调用 QxOrm 实现的 count 函数
      std::optional<T> findOne (const qx::QxSqlQuery &query = qx::QxSqlQuery())
      {
          // 执行查询
          qx::QxCollection<long, T> collection;
          QSqlError sqlError = qx::dao::fetch_by_query (query, collection, &m_db);
          if (sqlError.type() != QSqlError::NoError)
          {
              qCritical() << "[DB_FINDONE_ERROR]" << sqlError.text();
              return std::nullopt; // 返回空值，避免返回未初始化对象
          }
          if (collection.size() < 1)
          {
              qWarning() << "[DB_FINDONE_WARNING] 查询结果为空";
              return std::nullopt;
          }
          qDebug() << "findOne result：" << collection.size() << " records";
          return std::move (collection.getFirst()); // 使用 std::move() 避免拷贝
      }

      // 通过查询条件，删除数据
      bool deleteByQuery (const qx::QxSqlQuery & query = qx::QxSqlQuery())
      {
          QSqlError sqlError = qx::dao::delete_by_query<T> (query, &m_db);
          if (sqlError.type() != QSqlError::NoError)
          {
              qDebug() << "[DB_DELETE_BY_QUERY_ERROR]: " << m_db.lastError().text();
              return false;
          }
          return true;
      }
  protected:
      QSqlDatabase m_db;
};

#endif // BASEDAO_H
