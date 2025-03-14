#include "secure_boot.h"
#include <mbedtls/sha256.h>
#include <mbedtls/pk.h>

SecureBoot::SecureBoot(QObject *parent) 
    : QObject(parent), 
      m_pubKeyPem("-----BEGIN PUBLIC KEY-----\n...") {}

bool SecureBoot::verifyFirmware(const QByteArray &fwData, const QByteArray &signature) {
    mbedtls_pk_context pk;
    mbedtls_pk_init(&pk);
    
    // Загрузка публичного ключа
    if (mbedtls_pk_parse_public_key(&pk, 
        reinterpret_cast<const unsigned char*>(m_pubKeyPem.data()),
        m_pubKeyPem.size() + 1) != 0) {
        return false;
    }

    // Проверка подписи SHA256
    unsigned char hash[32];
    mbedtls_sha256(reinterpret_cast<const unsigned char*>(fwData.data()),
        fwData.size(), hash, 0);
    
    return mbedtls_pk_verify(&pk, MBEDTLS_MD_SHA256,
        hash, sizeof(hash),
        reinterpret_cast<const unsigned char*>(signature.data()),
        signature.size()) == 0;
}

void SecureBoot::lockFlashMemory() {
    // Блокировка регистра опций
    FLASH_OBProgramInitTypeDef ob;
    HAL_FLASHEx_OBGetConfig(&ob);
    ob.OptionBytes.SecureAreaConfig = OB_SECURE_AREA_CONFIG_SECURE;
    HAL_FLASHEx_OBProgram(&ob);
}
