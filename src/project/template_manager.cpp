#include "template_manager.h"
#include <QJsonDocument>
#include <QtConcurrent>

TemplateManager::TemplateManager(QObject *parent)
    : QObject(parent), m_network(new QNetworkAccessManager(this)) {}

void TemplateManager::fetchTemplatesFromRepo(const QString &repoUrl) {
    QNetworkRequest request(QUrl(repoUrl + "/templates.json"));
    QNetworkReply *reply = m_network->get(request);
    
    connect(reply, &QNetworkReply::finished, [this, reply]() {
        if (reply->error() == QNetworkReply::NoError) {
            QJsonArray templates = QJsonDocument::fromJson(reply->readAll()).array();
            emit templatesFetched(parseTemplateList(templates));
        }
    });
}

QVector<ProjectTemplate> TemplateManager::parseTemplateList(const QJsonArray &arr) {
    QVector<ProjectTemplate> result;
    for (const auto &item : arr) {
        QJsonObject obj = item.toObject();
        result.append({
            .name = obj["name"].toString(),
            .category = obj["category"].toString(),
            .mcuFamily = obj["mcu"].toString(),
            .gitUrl = obj["repo"].toString()
        });
    }
    return result;
}

void TemplateManager::createFromTemplate(const QString &gitUrl, const QString &targetPath) {
    QtConcurrent::run([=]() {
        QProcess git;
        git.setWorkingDirectory(targetPath);
        git.start("git", {"clone", gitUrl, "."});
        git.waitForFinished();
        emit templateCreated(targetPath);
    });
}
