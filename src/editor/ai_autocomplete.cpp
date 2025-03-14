#include "ai_autocomplete.h"
#include <QNetworkReply>

AIAutocomplete::AIAutocomplete(QObject *parent)
    : QObject(parent), m_network(new QNetworkAccessManager(this)) {}

void AIAutocomplete::requestSuggestions(const QString &context) {
    QNetworkRequest request(QUrl("https://api.stm32ide.com/ai/completions"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    
    QJsonObject body;
    body["code"] = context;
    body["lang"] = "c";
    
    QNetworkReply *reply = m_network->post(request, QJsonDocument(body).toJson());
    connect(reply, &QNetworkReply::finished, [this, reply]() {
        if (reply->error() == QNetworkReply::NoError) {
            QJsonArray suggestions = QJsonDocument::fromJson(reply->readAll()).array();
            emit suggestionsReady(parseSuggestions(suggestions));
        }
    });
}

QStringList AIAutocomplete::parseSuggestions(const QJsonArray &suggestions) {
    QStringList result;
    for (const auto &item : suggestions) {
        result << item.toObject()["text"].toString();
    }
    return result;
}
