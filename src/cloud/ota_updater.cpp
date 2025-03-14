#include "ota_updater.h"
#include <QCryptographicHash>

OTAUpdater::OTAUpdater(QObject *parent)
    : QObject(parent), m_network(new QNetworkAccessManager(this)) {}

void OTAUpdater::checkForUpdates(const QString &currentVersion) {
    QNetworkRequest request(QUrl("https://firmware.stm32ide.com/update"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    
    QJsonObject query;
    query["device_id"] = m_deviceId;
    query["current_fw"] = currentVersion;
    
    QNetworkReply *reply = m_network->post(request, QJsonDocument(query).toJson());
    connect(reply, &QNetworkReply::finished, [this, reply]() {
        if (reply->error() == QNetworkReply::NoError) {
            QJsonObject response = QJsonDocument::fromJson(reply->readAll()).object();
            processUpdateResponse(response);
        }
    });
}

void OTAUpdater::processUpdateResponse(const QJsonObject &response) {
    if (response["update_available"].toBool()) {
        m_latestVersion = response["latest_version"].toString();
        m_downloadUrl = QUrl(response["download_url"].toString());
        m_fileHash = response["sha256"].toString();
        emit updateAvailable(m_latestVersion);
    }
}

void OTAUpdater::downloadUpdate() {
    QNetworkRequest request(m_downloadUrl);
    QNetworkReply *reply = m_network->get(request);
    
    connect(reply, &QNetworkReply::downloadProgress, [this](qint64 bytes, qint64 total) {
        emit downloadProgress(bytes * 100 / total);
    });
    
    connect(reply, &QNetworkReply::finished, [this, reply]() {
        QByteArray data = reply->readAll();
        if (verifyHash(data)) {
            saveFirmware(data);
            emit downloadCompleted(true);
        }
    });
}

bool OTAUpdater::verifyHash(const QByteArray &data) {
    return QCryptographicHash::hash(data, QCryptographicHash::Sha256).toHex() == m_fileHash;
}
