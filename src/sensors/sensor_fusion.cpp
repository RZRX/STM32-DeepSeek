#include "sensor_fusion.h"
#include <QtMath>

SensorFusion::SensorFusion(QObject *parent)
    : QObject(parent), 
      m_accelFilter(0.2), 
      m_gyroFilter(0.1) {}

void SensorFusion::updateIMUData(const IMUData &data) {
    // Фильтр Калмана для акселерометра
    m_accelFilter.update({
        data.accelX,
        data.accelY,
        data.accelZ
    });
    
    // Комплементарный фильтр для гироскопа
    m_gyroFilter.update({
        data.gyroX * 0.98 + m_accelFilter.x() * 0.02,
        data.gyroY * 0.98 + m_accelFilter.y() * 0.02,
        data.gyroZ * 0.98 + m_accelFilter.z() * 0.02
    });
    
    // Расчет углов
    m_roll = qRadiansToDegrees(qAtan2(m_gyroFilter.y(), m_gyroFilter.z()));
    m_pitch = qRadiansToDegrees(qAtan2(-m_gyroFilter.x(), 
        qSqrt(m_gyroFilter.y() * m_gyroFilter.y() + m_gyroFilter.z() * m_gyroFilter.z())));
    
    emit anglesUpdated(m_roll, m_pitch);
}

void SensorFusion::calibrate() {
    // Калибровка смещений
    // ...
}
