#include "wireless_manager.h"
#include <ESP8266WiFi.h>
#include <BluetoothSerial.h>

WirelessManager::WirelessManager(QObject *parent)
    : QObject(parent), 
      m_wifiClient(new WiFiClient()),
      m_btSerial(new BluetoothSerial()) {}

void WirelessManager::connectToWiFi(const QString &ssid, const QString &pass) {
    WiFi.begin(ssid.toUtf8(), pass.toUtf8());
    m_connectTimeout.start(15000); // Таймаут 15 секунд
    connect(&m_connectTimeout, &QTimer::timeout, [this]() {
        emit connectionFailed("WiFi Timeout");
    });
}

void WirelessManager::enableBluetooth(const QString &deviceName) {
    if (!m_btSerial->begin(deviceName.toUtf8())) {
        emit error("Bluetooth Init Failed");
    } else {
        connect(m_btSerial, &BluetoothSerial::dataReceived,
                this, &WirelessManager::processBTData);
    }
}

void WirelessManager::sendOverMQTT(const QString &topic, const QByteArray &data) {
    if (m_wifiClient->connected()) {
        m_mqttClient.publish(topic.toUtf8(), data);
    } else {
        emit error("WiFi Not Connected");
    }
}

void WirelessManager::updateFirmwareOTA() {
    WiFiClientSecure client;
    client.setInsecure(); // Для упрощения (в продакшене использовать сертификаты)
    HTTPClient http;
    http.begin(client, m_fwUpdateUrl);
    
    if (http.GET() == HTTP_CODE_OK) {
        int len = http.getSize();
        WiFiUpdate.onProgress([this](size_t prog, size_t total) {
            emit otaProgress(prog * 100 / total);
        });
        WiFiUpdate.update(client, http.getStream());
    }
}
