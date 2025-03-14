#include "jenkins_integration.h"
#include <QNetworkReply>

JenkinsIntegration::JenkinsIntegration(QObject *parent)
    : QObject(parent), m_jenkinsUrl("http://jenkins:8080") {}

void JenkinsIntegration::triggerBuild(const QString &jobName, const QString &gitBranch) {
    QUrl url(m_jenkinsUrl + "/job/" + jobName + "/buildWithParameters");
    QUrlQuery query;
    query.addQueryItem("token", "STM32_IDE");
    query.addQueryItem("branch", gitBranch);
    url.setQuery(query);

    QNetworkRequest request(url);
    request.setRawHeader("Authorization", "Basic " + QByteArray("user:pass").toBase64());

    QNetworkReply *reply = m_nam.get(request);
    connect(reply, &QNetworkReply::finished, [this, reply]() {
        if (reply->error() == QNetworkReply::NoError) {
            emit buildTriggered(reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt());
        }
    });
}

void JenkinsIntegration::parseBuildStatus(const QJsonObject &json) {
    JenkinsBuildStatus status;
    status.duration = json["duration"].toInt();
    status.result = json["result"].toString();
    emit buildStatusUpdated(status);
}
