/*
 * SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
 *
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include "linglong/cli/printer.h"

#include "linglong/package/reference.h"

#include <QJsonArray>

#include <iomanip>
#include <iostream>

namespace linglong::cli {
namespace {

int getUnicodeNum(const QString &name)
{
    int num = 0;
    int count = name.count();
    for (int i = 0; i < count; i++) {
        QChar ch = name.at(i);
        ushort decode = ch.unicode();
        if (decode >= 0x4E00 && decode <= 0x9FA5) {
            num++;
        }
    }
    return num;
}
} // namespace

void Printer::printErr(const utils::error::Error &err)
{
    std::cout << "Error: CODE=" << err.code() << std::endl
              << err.message().toStdString() << std::endl;
}

void Printer::printPackageInfos(const std::vector<api::types::v1::PackageInfo> &list)
{
    std::cout << "\033[38;5;214m" << std::left << std::setw(32) << qUtf8Printable("appId")
              << std::setw(32) << qUtf8Printable("name") << std::setw(16)
              << qUtf8Printable("version") << std::setw(12) << qUtf8Printable("arch")
              << std::setw(16) << qUtf8Printable("channel") << std::setw(12)
              << qUtf8Printable("module") << qUtf8Printable("description") << "\033[0m"
              << std::endl;

    for (const auto &info : list) {
        auto simpleDescription = QString::fromStdString(info.description.value_or("")).trimmed();
        if (simpleDescription.length() > 56) {
            simpleDescription = simpleDescription.left(53) + "...";
        }

        auto appId = QString::fromStdString(info.id).trimmed();

        auto name = QString::fromStdString(info.name).trimmed();
        if (name.length() > 32) {
            name = name.left(29) + "...";
        }
        if (appId.length() > 32) {
            name.push_front(" ");
        }
        int count = getUnicodeNum(name);
        int length = simpleDescription.length() < 56 ? simpleDescription.length() : 56;
        std::cout << std::setw(32) << appId.toStdString() << std::setw(32) << name.toStdString()
                  << std::setw(16) << info.version << std::setw(12) << info.architecture
                  << std::setw(16) << info.channel << std::setw(12) << info.packageInfoModule
                  << std::setw(length) << simpleDescription.toStdString() << std::endl;
    }
}

void Printer::printContainers(const std::vector<api::types::v1::CliContainer> &list)
{
    std::cout << "\033[38;5;214m" << std::left << std::setw(48) << qUtf8Printable("App")
              << std::setw(36) << qUtf8Printable("ContainerID") << std::setw(8)
              << qUtf8Printable("Pid") << qUtf8Printable("Path") << "\033[0m" << std::endl;

    for (auto const &container : list) {
        std::cout << std::setw(48) << container.package << std::setw(36) << container.id
                  << std::setw(8) << container.pid << std::endl;
    }
}

void Printer::printCommonResult(const api::types::v1::CommonResult &reply)
{
    std::cout << "code: " << reply.code << std::endl;
    std::cout << "message:" << std::endl << reply.message << std::endl;
}

void printRepoInfos(const api::types::v1::PackageManager1GetRepoInfoResultRepoInfo &repoInfo)
{
    std::cout << "Default: " << repoInfo.defaultRepo << std::endl;
    std::cout << std::left << std::setw(11) << "Name";
    std::cout << "Url" << std::endl;
    for (const auto &repo : repoInfo.repos) {
        std::cout << std::left << std::setw(10) << repo.first << " " << repo.second << std::endl;
    }
}

void Printer::printLayerInfo(const api::types::v1::LayerInfo &info)
{
    std::cout << info.info.dump(4) << std::endl;
}

void Printer::printMessage(const QString &text, const int num)
{
    QByteArray blank;
    blank.fill(' ', num);

    std::cout << blank.toStdString() << text.toStdString() << std::endl;
}

void Printer::printReplacedText(const QString &text, const int num)
{
    QByteArray blank;
    blank.fill(' ', num);

    std::cout << "\33[2K\r" << blank.toStdString() << text.toStdString() << std::flush;
}

} // namespace linglong::cli
