#include "hw_spec_loader.h"
#include <QJsonDocument>

bool HWSpecLoader::loadDeviceSpec(const QString &mcu) {
    QFile specFile(QString(":/specs/%1.json").arg(mcu));
    if (!specFile.open(QIODevice::ReadOnly)) return false;

    QJsonObject root = QJsonDocument::fromJson(specFile.readAll()).object();
    m_spec = {
        .flashSize = root["flash"].toInt(),
        .ramSize = root["ram"].toInt(),
        .cpuClock = root["clock"].toInt(),
        .peripherals = parsePeripherals(root["peripherals"].toArray())
    };
    return true;
}

QVector<PeripheralSpec> HWSpecLoader::parsePeripherals(const QJsonArray &arr) {
    QVector<PeripheralSpec> result;
    for (const auto &item : arr) {
        QJsonObject obj = item.toObject();
        result.append({
            .name = obj["name"].toString(),
            .baseAddr = obj["base"].toString().toUInt(nullptr, 16),
            .irqNumber = obj["irq"].toInt()
        });
    }
    return result;
}
