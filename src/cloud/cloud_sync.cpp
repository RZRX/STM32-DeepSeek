#include "cloud_sync.h"
#include <QNetworkRequest>

CloudSync::CloudSync(QObject *parent)
    : QObject(parent), 
      m_manager(new QNetworkAccessManager(this)),
      m_apiUrl("https://api.stm32ide.com/v1") {}

void CloudSync::uploadProject(const QString &projectId, const QString &zipPath) {
    QFile zip(zipPath);
    if (!zip.open(QIODevice::ReadOnly)) return;

    QNetworkRequest request(QUrl(m_apiUrl + "/projects/" + projectId));
    request.setRawHeader("Authorization", "Bearer " + m_apiKey);

    QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);
    QHttpPart filePart;
    filePart.setBodyDevice(&zip);
    filePart.setHeader(QNetworkRequest::ContentTypeHeader, "application/zip");
    filePart.setHeader(QNetworkRequest::ContentDispositionHeader, 
        QVariant("form-data; name=\"file\"; filename=\"project.zip\""));

    multiPart->append(filePart);

    QNetworkReply *reply = m_manager->put(request, multiPart);
    connect(reply, &QNetworkReply::uploadProgress, 
            [this](qint64 sent, qint64 total) { emit syncProgress(sent * 100 / total); });
}
