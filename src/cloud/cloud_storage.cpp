#include "cloud_storage.h"
#include <QOAuth2AuthorizationCodeFlow>

CloudStorage::CloudStorage(QObject *parent)
    : QObject(parent),
      m_oauth2(new QOAuth2AuthorizationCodeFlow(this)) {
    
    m_oauth2->setAuthorizationUrl(QUrl("https://api.stm32ide.com/oauth"));
    m_oauth2->setAccessTokenUrl(QUrl("https://api.stm32ide.com/token"));
    m_oauth2->setClientIdentifier("IDE_CLIENT_ID");
    
    connect(m_oauth2, &QOAuth2AuthorizationCodeFlow::authorizeWithBrowser,
            this, &CloudStorage::openAuthBrowser);
}

void CloudStorage::uploadProject(const QString &projectPath) {
    if (m_oauth2->status() != QOAuth2AuthorizationCodeFlow::Status::Granted) {
        emit error("Not authenticated");
        return;
    }

    QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);
    QFile *file = new QFile(projectPath, multiPart);
    
    if (file->open(QIODevice::ReadOnly)) {
        QHttpPart filePart;
        filePart.setBodyDevice(file);
        filePart.setHeader(QNetworkRequest::ContentTypeHeader, "application/zip");
        multiPart->append(filePart);
        
        QNetworkReply *reply = m_oauth2->post(QUrl("https://api.stm32ide.com/projects"), multiPart);
        connect(reply, &QNetworkReply::uploadProgress, [this](qint64 sent, qint64 total) {
            emit uploadProgress(sent * 100 / total);
        });
    }
}

void CloudStorage::syncProjectList() {
    QNetworkReply *reply = m_oauth2->get(QUrl("https://api.stm32ide.com/projects"));
    connect(reply, &QNetworkReply::finished, [this, reply]() {
        QJsonArray projects = QJsonDocument::fromJson(reply->readAll()).array();
        emit projectListUpdated(parseProjectList(projects));
    });
}
