/*
 * SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
 *
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef LINGLONG_UTILS_SERIALIZE_JSON_H
#define LINGLONG_UTILS_SERIALIZE_JSON_H

#include "linglong/api/types/v1/Generators.hpp"
#include "linglong/utils/error/error.h"

#include <nlohmann/json.hpp>

#include <QJsonArray>
#include <QJsonDocument>

namespace linglong::utils::serialize {

template<typename T>
QJsonDocument toQJsonDocument(const T &x) noexcept
{
    nlohmann::json json = nlohmann::to_json(x);
    QJsonDocument doc;
    return QJsonDocument::fromJson(json.dump().data());
}

template<typename T>
QVariantMap toQVariantMap(const T &x) noexcept
{
    QJsonDocument doc = toQJsonDocument(x);
    Q_ASSERT(doc.isObject());
    return doc.object().toVariantMap();
}

template<typename T, typename Source>
error::Result<T> LoadJSON(const Source& content) noexcept
{
    LINGLONG_TRACE("load json");

    try {
        auto json = nlohmann::json::parse(content);
        return json.template get<T>();
    } catch (const std::exception &e) {
        return LINGLONG_ERR(e);
    }
}

template<typename T>
error::Result<T> fromQJsonDocument(const QJsonDocument &doc) noexcept
{
    auto x = LoadJSON<T>(doc.toJson().constData());

    if (!x) {
        return x;
    }
    return x;
}

template<typename T>
error::Result<T> fromQJsonObject(const QJsonObject &obj) noexcept
{
    return fromQJsonDocument<T>(QJsonDocument(obj));
}

template<typename T>
error::Result<T> fromQJsonValue(const QJsonValue &v) noexcept
{
    if (v.isArray()) {
        return fromQJsonDocument<T>(QJsonDocument(v.toArray()));
    }
    if (v.isObject()) {
        return fromQJsonObject<T>(v.toObject());
    }

    Q_ASSERT("QJsonValue should be object or array.");
    abort();
}

template<typename T>
error::Result<T> fromQVariantMap(const QVariantMap &vmap)
{
    return fromQJsonObject<T>(QJsonObject::fromVariantMap(vmap));
}

} // namespace linglong::utils::serialize

#endif
