#include "iot_platforms.h"
#include <QMqttClient>

AWSIoTIntegration::AWSIoTIntegration(QObject *parent)
    : IoTPlatform("AWS IoT Core", parent),
      m_client(new QMqttClient(this)) {

    m_client->setHostname("a3qtx9dpz4abcd-ats.iot.us-east-1.amazonaws.com");
    m_client->setPort(8883);
    connect(m_client, &QMqttClient::connected, 
            [this]() { emit connectionEstablished(); });
}

void AWSIoTIntegration::publishTelemetry(const QJsonObject &data) {
    QMqttTopicName topic("stm32/telemetry");
    m_client->publish(topic, QJsonDocument(data).toJson());
}

void AWSIoTIntegration::subscribeToCommands() {
    m_client->subscribe("stm32/commands")->setHandler(
        [this](QMqttSubscription::SubscribeResult result) {
            if (result == QMqttSubscription::Subscribed)
                emit commandSubscriptionActive();
        }
    );
}
