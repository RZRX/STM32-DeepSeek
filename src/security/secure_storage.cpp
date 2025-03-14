#include "secure_storage.h"
#include <QSysInfo>

SecureStorage::SecureStorage(QObject *parent)
    : QObject(parent), m_useHSM(false) {}

void SecureStorage::init() {
#if defined(Q_OS_WIN)
    m_useHSM = checkTPMAvailability();
#elif defined(Q_OS_LINUX)
    m_useHSM = access("/dev/tpm0", F_OK) != -1;
#endif
}

bool SecureStorage::storeKey(const QString &keyId, const QByteArray &key) {
    if (m_useHSM) {
        return storeInHSM(keyId, key);
    } else {
        return storeInSoftware(keyId, key);
    }
}

bool SecureStorage::storeInHSM(const QString &keyId, const QByteArray &key) {
    // Интеграция с аппаратным модулем безопасности
    // ...
    return true;
}

bool SecureStorage::storeInSoftware(const QString &keyId, const QByteArray &key) {
    QSettings secureConfig("stm32ide.ini", QSettings::IniFormat);
    secureConfig.setValue("keys/" + keyId, 
        QByteArray(key.toBase64()).toHex());
    return true;
}
