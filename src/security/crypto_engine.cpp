#include "crypto_engine.h"
#include <mbedtls/aes.h>
#include <mbedtls/sha256.h>

CryptoEngine::CryptoEngine(QObject *parent)
    : QObject(parent), m_aesContext(new mbedtls_aes_context) {
    mbedtls_aes_init(m_aesContext);
}

QByteArray CryptoEngine::encryptAES(const QByteArray &data, const QByteArray &key) {
    unsigned char iv[16] = {0};
    QByteArray encrypted(data.size(), 0);
    
    mbedtls_aes_setkey_enc(m_aesContext, 
        reinterpret_cast<const unsigned char*>(key.constData()), 
        key.size() * 8);
        
    mbedtls_aes_crypt_cbc(m_aesContext, MBEDTLS_AES_ENCRYPT,
        data.size(), iv,
        reinterpret_cast<const unsigned char*>(data.constData()),
        reinterpret_cast<unsigned char*>(encrypted.data()));
        
    return encrypted;
}

QByteArray CryptoEngine::calculateSHA256(const QByteArray &data) {
    unsigned char hash[32];
    mbedtls_sha256_context ctx;
    
    mbedtls_sha256_init(&ctx);
    mbedtls_sha256_starts(&ctx, 0);
    mbedtls_sha256_update(&ctx, 
        reinterpret_cast<const unsigned char*>(data.constData()), 
        data.size());
    mbedtls_sha256_finish(&ctx, hash);
    
    return QByteArray(reinterpret_cast<char*>(hash), 32);
}

void CryptoEngine::generateHMAC(const QByteArray &key, const QByteArray &data) {
    // Реализация HMAC-SHA256
    // ...
}
