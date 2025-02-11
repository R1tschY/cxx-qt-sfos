// clang-format off
// SPDX-FileCopyrightText: 2022 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
// clang-format on
// SPDX-FileContributor: Andrew Hayzen <andrew.hayzen@kdab.com>
// SPDX-FileContributor: Leon Matthes <leon.matthes@kdab.com>
//
// SPDX-License-Identifier: MIT OR Apache-2.0
#pragma once

#include <cinttypes>
#include <memory>

#include <QtCore/QDateTime>
#include <QtCore/QTimeZone>

#include "rust/cxx.h"

template<>
struct rust::IsRelocatable<QDateTime> : ::std::true_type
{
};

namespace rust {
namespace cxxqtlib1 {

QDateTime
qdatetimeAddDays(const QDateTime& datetime, ::std::int64_t ndays);
QDateTime
qdatetimeAddMSecs(const QDateTime& datetime, ::std::int64_t msecs);
QDateTime
qdatetimeAddSecs(const QDateTime& datetime, ::std::int64_t secs);
QDateTime
qdatetimeCurrentDateTime();
QDateTime
qdatetimeCurrentDateTimeUtc();
::std::int64_t
qdatetimeCurrentMSecsSinceEpoch();
::std::int64_t
qdatetimeDaysTo(const QDateTime& datetime, const QDateTime& other);
QDateTime
qdatetimeFromMSecsSinceEpoch(::std::int64_t msecs, const QTimeZone& timeZone);
::std::int64_t
qdatetimeMSecsTo(const QDateTime& datetime, const QDateTime& other);
::std::int64_t
qdatetimeSecsTo(const QDateTime& datetime, const QDateTime& other);
void
qdatetimeSetDate(QDateTime& datetime, QDate date);
void
qdatetimeSetMSecsSinceEpoch(QDateTime& datetime, ::std::int64_t msecs);
void
qdatetimeSetTime(QDateTime& datetime, QTime time);
::std::unique_ptr<QTimeZone>
qdatetimeTimeZone(const QDateTime& datetime);
::std::int64_t
qdatetimeToMSecsSinceEpoch(const QDateTime& datetime);

}
}
