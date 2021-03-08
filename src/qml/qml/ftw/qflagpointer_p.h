/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtQml module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or (at your option) the GNU General
** Public license version 3 or any later version approved by the KDE Free
** Qt Foundation. The licenses are as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL2 and LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-2.0.html and
** https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QFLAGPOINTER_P_H
#define QFLAGPOINTER_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <QtCore/qglobal.h>
#ifdef __CHERI_PURE_CAPABILITY__
#include <cheri.h>
#include <assert.h>
#endif

QT_BEGIN_NAMESPACE

namespace QtPrivate {
template <typename T> struct QFlagPointerAlignment
{
    enum : size_t { Value = Q_ALIGNOF(T) };
};
template <> struct QFlagPointerAlignment<void>
{
    enum : size_t { Value = ~size_t(0) };
};
}

template<typename T>
class QFlagPointer {
public:
    inline QFlagPointer();
    inline QFlagPointer(T *);
    inline QFlagPointer(const QFlagPointer<T> &o);

    inline bool isNull() const;

    inline bool flag() const;
    inline void setFlag();
    inline void clearFlag();
    inline void setFlagValue(bool);

    inline bool flag2() const;
    inline void setFlag2();
    inline void clearFlag2();
    inline void setFlag2Value(bool);

    inline QFlagPointer<T> &operator=(const QFlagPointer &o);
    inline QFlagPointer<T> &operator=(T *);

    inline T *operator->() const;
    inline T *operator*() const;

    inline T *data() const;

    inline explicit operator bool() const;

private:
    quintptr ptr_value = 0;

    static const quintptr FlagBit = 0x1;
    static const quintptr Flag2Bit = 0x2;
    static const quintptr FlagsMask = FlagBit | Flag2Bit;
};

template<typename T, typename T2>
class QBiPointer {
public:
    inline QBiPointer();
    inline QBiPointer(T *);
    inline QBiPointer(T2 *);
    inline QBiPointer(const QBiPointer<T, T2> &o);

    inline bool isNull() const;
    inline bool isT1() const;
    inline bool isT2() const;

    inline bool flag() const;
    inline void setFlag();
    inline void clearFlag();
    inline void setFlagValue(bool);

    inline QBiPointer<T, T2> &operator=(const QBiPointer<T, T2> &o);
    inline QBiPointer<T, T2> &operator=(T *);
    inline QBiPointer<T, T2> &operator=(T2 *);

    inline T *asT1() const;
    inline T2 *asT2() const;

private:
    quintptr ptr_value = 0;

    static const quintptr FlagBit = 0x1;
    static const quintptr Flag2Bit = 0x2;
    static const quintptr FlagsMask = FlagBit | Flag2Bit;
};

template<typename T>
QFlagPointer<T>::QFlagPointer()
{
}

template<typename T>
QFlagPointer<T>::QFlagPointer(T *v)
: ptr_value(quintptr(v))
{
    Q_STATIC_ASSERT_X(Q_ALIGNOF(T) >= 4, "Type T does not have sufficient alignment");
#ifdef __CHERI_PURE_CAPABILITY__
	Q_ASSERT(cheri_low_bits_get(ptr_value, FlagsMask) == 0);
#else
    Q_ASSERT((ptr_value & FlagsMask) == 0);
#endif
}

template<typename T>
QFlagPointer<T>::QFlagPointer(const QFlagPointer<T> &o)
: ptr_value(o.ptr_value)
{
}

template<typename T>
bool QFlagPointer<T>::isNull() const
{
#ifdef __CHERI_PURE_CAPABILITY__
    return 0 == cheri_low_bits_clear(ptr_value, FlagsMask);
#else
    return 0 == (ptr_value & (~FlagsMask));
#endif
}

template<typename T>
bool QFlagPointer<T>::flag() const
{
#ifdef __CHERI_PURE_CAPABILITY__
    return cheri_low_bits_get(ptr_value, FlagBit);
#else
    return ptr_value & FlagBit;
#endif
}

template<typename T>
void QFlagPointer<T>::setFlag()
{
#ifdef __CHERI_PURE_CAPABILITY__
    ptr_value = cheri_low_bits_or(ptr_value, FlagBit);
#else
    ptr_value |= FlagBit;
#endif
}

template<typename T>
void QFlagPointer<T>::clearFlag()
{
#ifdef __CHERI_PURE_CAPABILITY__
    ptr_value = cheri_low_bits_clear(ptr_value, FlagBit);
#else
    ptr_value &= ~FlagBit;
#endif
}

template<typename T>
void QFlagPointer<T>::setFlagValue(bool v)
{
    if (v) setFlag();
    else clearFlag();
}

template<typename T>
bool QFlagPointer<T>::flag2() const
{
#ifdef __CHERI_PURE_CAPABILITY__
    return cheri_low_bits_get(ptr_value, FlagsMask) & Flag2Bit;
#else
    return ptr_value & Flag2Bit;
#endif
}

template<typename T>
void QFlagPointer<T>::setFlag2()
{
#ifdef __CHERI_PURE_CAPABILITY__
    ptr_value = cheri_low_bits_or(ptr_value, Flag2Bit);
#else
    ptr_value |= Flag2Bit;
#endif
}

template<typename T>
void QFlagPointer<T>::clearFlag2()
{
#ifdef __CHERI_PURE_CAPABILITY__
    ptr_value = cheri_low_bits_set(ptr_value, FlagsMask, cheri_low_bits_get(ptr_value, FlagBit));
#else
    ptr_value &= ~Flag2Bit;
#endif
}

template<typename T>
void QFlagPointer<T>::setFlag2Value(bool v)
{
    if (v) setFlag2();
    else clearFlag2();
}

template<typename T>
QFlagPointer<T> &QFlagPointer<T>::operator=(const QFlagPointer &o)
{
    ptr_value = o.ptr_value;
    return *this;
}

template<typename T>
QFlagPointer<T> &QFlagPointer<T>::operator=(T *o)
{
#ifdef __CHERI_PURE_CAPABILITY__
	Q_ASSERT(cheri_low_bits_get(quintptr(o), FlagsMask) == 0);
#else
    Q_ASSERT((quintptr(o) & FlagsMask) == 0);
#endif

#ifdef __CHERI_PURE_CAPABILITY__
    ptr_value = cheri_low_bits_set(quintptr(o), FlagsMask, cheri_low_bits_get(ptr_value, FlagsMask));
#else
    ptr_value = quintptr(o) | (ptr_value & FlagsMask);
#endif
    return *this;
}

template<typename T>
T *QFlagPointer<T>::operator->() const
{
#ifdef __CHERI_PURE_CAPABILITY__
    return (T *)(cheri_low_bits_clear(ptr_value, FlagsMask));
#else
    return (T *)(ptr_value & ~FlagsMask);
#endif
}

template<typename T>
T *QFlagPointer<T>::operator*() const
{
#ifdef __CHERI_PURE_CAPABILITY__
    return (T *)(cheri_low_bits_clear(ptr_value, FlagsMask));
#else
    return (T *)(ptr_value & ~FlagsMask);
#endif
}

template<typename T>
T *QFlagPointer<T>::data() const
{
#ifdef __CHERI_PURE_CAPABILITY__
    return (T *)(cheri_low_bits_clear(ptr_value, FlagsMask));
#else
    return (T *)(ptr_value & ~FlagsMask);
#endif
}

template<typename T>
QFlagPointer<T>::operator bool() const
{
    return data() != nullptr;
}

template<typename T, typename T2>
QBiPointer<T, T2>::QBiPointer()
{
}

template<typename T, typename T2>
QBiPointer<T, T2>::QBiPointer(T *v)
: ptr_value(quintptr(v))
{
    Q_STATIC_ASSERT_X(QtPrivate::QFlagPointerAlignment<T>::Value >= 4,
                      "Type T does not have sufficient alignment");
#ifdef __CHERI_PURE_CAPABILITY__
	Q_ASSERT(cheri_low_bits_get(quintptr(v), FlagsMask) == 0);
#else
    Q_ASSERT((quintptr(v) & FlagsMask) == 0);
#endif
}

template<typename T, typename T2>
QBiPointer<T, T2>::QBiPointer(T2 *v)
#ifdef __CHERI_PURE_CAPABILITY__
: ptr_value(cheri_low_bits_or(quintptr(v), Flag2Bit))
#else
: ptr_value(quintptr(v) | Flag2Bit)
#endif
{
    Q_STATIC_ASSERT_X(QtPrivate::QFlagPointerAlignment<T2>::Value >= 4,
                      "Type T2 does not have sufficient alignment");
#ifdef __CHERI_PURE_CAPABILITY__
	Q_ASSERT(cheri_low_bits_get(quintptr(v), FlagsMask) == 0);
#else
    Q_ASSERT((quintptr(v) & FlagsMask) == 0);
#endif
}

template<typename T, typename T2>
QBiPointer<T, T2>::QBiPointer(const QBiPointer<T, T2> &o)
: ptr_value(o.ptr_value)
{
}

template<typename T, typename T2>
bool QBiPointer<T, T2>::isNull() const
{
#ifdef __CHERI_PURE_CAPABILITY__
    return 0 == cheri_low_bits_clear(ptr_value, FlagsMask);
#else
    return 0 == (ptr_value & (~FlagsMask));
#endif
}

template<typename T, typename T2>
bool QBiPointer<T, T2>::isT1() const
{
#ifdef __CHERI_PURE_CAPABILITY__
    return !(cheri_low_bits_get(ptr_value, FlagsMask) & (size_t) Flag2Bit);
#else
    return !(ptr_value & Flag2Bit);
#endif
}

template<typename T, typename T2>
bool QBiPointer<T, T2>::isT2() const
{
#ifdef __CHERI_PURE_CAPABILITY__
    return cheri_low_bits_get(ptr_value, FlagsMask) & (size_t) Flag2Bit;
#else
    return ptr_value & Flag2Bit;
#endif
}

template<typename T, typename T2>
bool QBiPointer<T, T2>::flag() const
{
#ifdef __CHERI_PURE_CAPABILITY__
    return cheri_low_bits_get(ptr_value, FlagBit);
#else
    return ptr_value & FlagBit;
#endif
}

template<typename T, typename T2>
void QBiPointer<T, T2>::setFlag()
{
#ifdef __CHERI_PURE_CAPABILITY__
    ptr_value = cheri_low_bits_or(ptr_value, FlagBit);
#else
    ptr_value |= FlagBit;
#endif
}

template<typename T, typename T2>
void QBiPointer<T, T2>::clearFlag()
{
#ifdef __CHERI_PURE_CAPABILITY__
    ptr_value = cheri_low_bits_clear(ptr_value, FlagBit);
#else
    ptr_value &= ~FlagBit;
#endif
}

template<typename T, typename T2>
void QBiPointer<T, T2>::setFlagValue(bool v)
{
    if (v) setFlag();
    else clearFlag();
}

template<typename T, typename T2>
QBiPointer<T, T2> &QBiPointer<T, T2>::operator=(const QBiPointer<T, T2> &o)
{
    ptr_value = o.ptr_value;
    return *this;
}

template<typename T, typename T2>
QBiPointer<T, T2> &QBiPointer<T, T2>::operator=(T *o)
{
#ifdef __CHERI_PURE_CAPABILITY__
	Q_ASSERT(cheri_low_bits_get(quintptr(o), FlagsMask) == 0);
#else
    Q_ASSERT((quintptr(o) & FlagsMask) == 0);
#endif

#ifdef __CHERI_PURE_CAPABILITY__
    ptr_value = cheri_low_bits_set(quintptr(o), FlagBit, cheri_low_bits_get(ptr_value, FlagBit));
#else
    ptr_value = quintptr(o) | (ptr_value & FlagBit);
#endif
    return *this;
}

template<typename T, typename T2>
QBiPointer<T, T2> &QBiPointer<T, T2>::operator=(T2 *o)
{
#ifdef __CHERI_PURE_CAPABILITY__
	Q_ASSERT(cheri_low_bits_get(quintptr(o), FlagsMask) == 0);
#else
    Q_ASSERT((quintptr(o) & FlagsMask) == 0);
#endif

#ifdef __CHERI_PURE_CAPABILITY__
    ptr_value = cheri_low_bits_set(quintptr(o), FlagsMask, (cheri_low_bits_get(ptr_value, FlagBit) | (size_t) Flag2Bit));
#else
    ptr_value = quintptr(o) | (ptr_value & FlagBit) | Flag2Bit;
#endif
    return *this;
}

template<typename T, typename T2>
T *QBiPointer<T, T2>::asT1() const
{
    Q_ASSERT(isT1());
#ifdef __CHERI_PURE_CAPABILITY__
    return (T *)(cheri_low_bits_clear(ptr_value, FlagsMask));
#else
    return (T *)(ptr_value & ~FlagsMask);
#endif
}

template<typename T, typename T2>
T2 *QBiPointer<T, T2>::asT2() const
{
    Q_ASSERT(isT2());
#ifdef __CHERI_PURE_CAPABILITY__
    return (T2 *)(cheri_low_bits_clear(ptr_value, FlagsMask));
#else
    return (T2 *)(ptr_value & ~FlagsMask);
#endif
}

QT_END_NAMESPACE

#endif // QFLAGPOINTER_P_H
