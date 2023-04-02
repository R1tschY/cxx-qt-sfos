// clang-format off
// SPDX-FileCopyrightText: 2022 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
// clang-format on
// SPDX-FileContributor: Andrew Hayzen <andrew.hayzen@kdab.com>
//
// SPDX-License-Identifier: MIT OR Apache-2.0
#pragma once

#include <optional>
#include <memory>
#include <mutex>
#include <shared_mutex>
#include <stdexcept>

#include <QtCore/QTimer>
#include <QtCore/QDebug>

#include "rust/cxx.h"

namespace rust {
namespace cxxqtlib1 {

template<typename T>
class CxxQtGuardedPointer
{
public:
  explicit CxxQtGuardedPointer(T* ptr)
    : ptr(ptr)
  {
  }

  T* ptr;
  ::std::shared_mutex mutex;
};

template<typename T>
class CopyableLambda {
public:
    CopyableLambda(T t): t(::std::move(t)) { }

    CopyableLambda(const CopyableLambda& other)
        : t({})
    { }

    CopyableLambda& operator=(const CopyableLambda& other) {
        this->t = ::std::move(other.t);
        return *this;
    }

    CopyableLambda(CopyableLambda&& other)
        : t(::std::move(other.t))
    { }

    CopyableLambda& operator=(CopyableLambda&& other) {
        this->t = ::std::move(other.t);
        return *this;
    }

    void operator()() {
        this->t.value()();
    }

    std::optional<T> t;
};

template<typename T>
CopyableLambda<T> make_copyable_lambda(T t) {
    return { ::std::move(t) };
}

template<typename T>
class CxxQtThread
{
public:
  CxxQtThread(::std::shared_ptr<CxxQtGuardedPointer<T>> obj,
              ::std::shared_ptr<::std::recursive_mutex> rustObjMutex)
    : m_obj(obj)
    , m_rustObjMutex(rustObjMutex)
  {
  }

  template<typename A>
  void queue(::rust::Fn<void(T& self, ::rust::Box<A> arg)> func,
             ::rust::Box<A> arg) const
  {
    // Ensure that we can read the pointer and it's not being written to
    const auto guard = ::std::shared_lock(m_obj->mutex);
    if (!m_obj->ptr) {
      throw ::std::runtime_error(
        "Cannot queue function pointer as object has been destroyed");
      return;
    }

    // Construct the lambda
    auto obj = m_obj;
    auto rustObjMutex = m_rustObjMutex;
    auto lambda = [obj = ::std::move(obj),
                   rustObjMutex = ::std::move(rustObjMutex),
                   func = ::std::move(func),
                   arg = ::std::move(arg)]() mutable {
      // Ensure that we can read the pointer and it's not being written to
      const auto guard = ::std::shared_lock(obj->mutex);
      if (obj->ptr) {
        // Ensure that the rustObj is locked
        const ::std::lock_guard<::std::recursive_mutex> guardRustObj(
          *rustObjMutex);
        func(*obj->ptr, ::std::move(arg));
      } else {
        qWarning()
          << "Could not call the function pointer as object has been destroyed";
      }
    };

    // Add the lambda to the queue
    QTimer::singleShot(0, m_obj->ptr, make_copyable_lambda(::std::move(lambda)));
  }

private:
  ::std::shared_ptr<CxxQtGuardedPointer<T>> m_obj;
  ::std::shared_ptr<::std::recursive_mutex> m_rustObjMutex;
};

} // namespace cxxqtlib1
} // namespace rust
