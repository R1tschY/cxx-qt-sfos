<!--
SPDX-FileCopyrightText: 2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
SPDX-FileContributor: Leon Matthes <leon.matthes@kdab.com>

SPDX-License-Identifier: MIT OR Apache-2.0
-->

# Inheritance

Some Qt APIs require you to override certain methods from an abstract base class, for example [QAbstractItemModel](https://doc.qt.io/qt-6/qabstractitemmodel.html).

To support creating such subclasses directly from within Rust, CXX-Qt provides you with multiple helpers.

## Accessing base class methods
To access the methods of a base class in Rust, use the `#[cxx_qt::inherit]` macro.
It can be placed in front of an `extern "C++"` block in a `#[cxx_qt::bridge]`.

```rust,ignore
{{#include ../../../examples/qml_features/rust/src/custom_base_class.rs:book_inherit_qalm}}

{{#include ../../../examples/qml_features/rust/src/custom_base_class.rs:book_inherit_qalm_impl_unsafe}}

    impl qobject::CustomBaseClass {
{{#include ../../../examples/qml_features/rust/src/custom_base_class.rs:book_inherit_clear}}
    }
```
[Full example](https://github.com/KDAB/cxx-qt/blob/main/examples/qml_features/rust/src/custom_base_class.rs)

This code implements a QAbstractListModel subclass.
For this, the `clear` method implemented in Rust needs to call `beginResetModel` and related methods from the base class, which are made accessible by using `#[cxx_qt::inherit]`.
See [the Qt docs](https://doc.qt.io/qt-6/qabstractlistmodel.html) for more details on the specific subclassing requirements.

Methods can be declared inside `#[cxx_qt::inherit]` in `extern "C++"` blocks similar to CXX, with the same restrictions regarding which types can be used.
Additionally, the `self` type must be either `self: Pin<&mut qobject::T>` or `self: &qobject::T`, where `qobject::T` must refer to a QObject marked with `#[cxx_qt::qobject]` in the `#[cxx_qt::bridge]`

The declared methods will be case-converted as in other CXX-Qt APIs.
To explicitly declare the C++ method name, use the `#[cxx_name="myFunctionName"]` attribute.

## Overriding base class methods

CXX-Qt allows invokables to be generated with the C++ modifiers necessary to implement inheritance.
This way methods can be overridden, declared as `virtual` or `final`.

| C++ keyword | CXX-Qt attribute              |
|-------------|-------------------------------|
| `override`  | `#[qinvokable(cxx_override)]` |
| `virtual`   | `#[qinvokable(cxx_virtual)]`  |
| `final`     | `#[qinvokable(cxx_final)]`    |

The below example overrides the [`data`](https://doc.qt.io/qt-6/qabstractitemmodel.html#data) method inherited from the QAbstractListModel.
```rust,ignore
{{#include ../../../examples/qml_features/rust/src/custom_base_class.rs:book_inherit_qalm}}

    impl qobject::CustomBaseClass {

{{#include ../../../examples/qml_features/rust/src/custom_base_class.rs:book_inherit_data}}
    }
```
[Full example](https://github.com/KDAB/cxx-qt/blob/main/examples/qml_features/rust/src/custom_base_class.rs)

When a method is overridden using `cxx_override`, the base class version of the method can be accessed by using `#[cxx_qt::inherit]` in combination with the `#[cxx_name]` attribute.
In this case the base class version of the function must get a different name because Rust can't have two functions with the same name on one type.

Example:
```rust,ignore
{{#include ../../../examples/qml_features/rust/src/custom_base_class.rs:book_inherit_qalm}}

{{#include ../../../examples/qml_features/rust/src/custom_base_class.rs:book_inherit_qalm_impl_safe}}

    impl qobject::CustomBaseClass {
{{#include ../../../examples/qml_features/rust/src/custom_base_class.rs:book_inherit_can_fetch_more}}
    }
```
[Full example](https://github.com/KDAB/cxx-qt/blob/main/examples/qml_features/rust/src/custom_base_class.rs)
