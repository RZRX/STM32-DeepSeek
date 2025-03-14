#include "crypto_accelerator.h"
#include <stm32h7xx_hal.h>

CryptographicAccelerator::CryptographicAccelerator(QObject *parent)
    : QObject(parent) {
    
    HAL_CRYP_Init(&m_hcryp);
    HAL_CRYP_SetMode(&m_hcryp, CRYP_AES_ECB);
}

QByteArray CryptographicAccelerator::encryptAES(const QByteArray &data, const QByteArray &key) {
    HAL_StatusTypeDef status;
    QByteArray encrypted(data.size(), 0);
    
    HAL_CRYP_AESECB_Encrypt(&m_hcryp,
        (uint8_t*)data.constData(),
        data.size(),
        (uint8_t*)encrypted.data(),
        HAL_MAX_DELAY);
    
    if (status != HAL_OK) {
        emit error("AES encryption failed");
    }
    return encrypted;
}

void CryptographicAccelerator::enableDMA(bool enable) {
    if (enable) {
        __HAL_CRYP_ENABLE_IT(&m_hcryp, CRYP_IT_DIN);
    } else {
        __HAL_CRYP_DISABLE_IT(&m_hcryp, CRYP_IT_DIN);
    }
}
