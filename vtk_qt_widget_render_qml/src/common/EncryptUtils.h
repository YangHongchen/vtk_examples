#ifndef ENCRYPTUTILS_H
#define ENCRYPTUTILS_H
#include <QString>

class EncryptUtils {
  public:
    // 禁用构造函数和拷贝操作
    EncryptUtils() = delete;

    //
    EncryptUtils (const EncryptUtils &) = delete;

    //
    EncryptUtils &operator = (const EncryptUtils &) = delete;

    // 哈希算法组
    static QString passwordEncrypt (const QString password);

};

#endif // ENCRYPTUTILS_H
