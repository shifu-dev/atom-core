#pragma once
#include "RangeReq.h"
#include "RangeReqMock.h"

namespace Atom
{
    /// --------------------------------------------------------------------------------------------
    /// {Insertable} represents a type that allows inserting objs of type {Element} into it.
    /// 
    /// {FrontInsertable} represents a type that allows inserting objs of type {Element} 
    /// into front of container.
    /// 
    /// {FrontInsertable} represents a type that allows inserting objs of type {Element} 
    /// into back of container.
    /// 
    /// {KeyInsertable} represents a type that allows inserting objs of type {Element} 
    /// at specified index of container.
    /// 
    /// {IndexInsertable} represents a type that allows inserting objs of type {Element} 
    /// at specified index of container.
    /// --------------------------------------------------------------------------------------------

    /// --------------------------------------------------------------------------------------------
    /// Ensures {TInsertable} is {Insertable} for type {T}.
    /// --------------------------------------------------------------------------------------------
    template <typename TInsertable, typename T>
    concept RInsertable = requires(TInsertable insertable, T el, RangeReqMock<T> range)
    {
        insertable.Insert(el);
        insertable.Insert(range);
    };

    /// --------------------------------------------------------------------------------------------
    /// Ensures {TInsertable} is {FrontInsertable} for type {T}.
    /// --------------------------------------------------------------------------------------------
    template <typename TFrontInsertable, typename T>
    concept RFrontInsertable = requires(TFrontInsertable insertable, T el, RangeReqMock<T> range)
    {
        insertable.InsertFront(el);
        insertable.InsertFront(range);
    };

    /// --------------------------------------------------------------------------------------------
    /// Ensures {TInsertable} is {BackInsertable} for type {T}.
    /// --------------------------------------------------------------------------------------------
    template <typename TInsertable, typename T>
    concept RBackInsertable = requires(TInsertable insertable, T el, RangeReqMock<T> range)
    {
        insertable.InsertBack(el);
        insertable.InsertBack(range);
    };

    /// --------------------------------------------------------------------------------------------
    /// Ensures {TInsertable} is {KeyInsertable} for type {T}.
    /// --------------------------------------------------------------------------------------------
    template <typename TInsertable, typename TKey, typename T>
    concept RKeyInsertable = requires(TInsertable insertable, TKey key, T el, RangeReqMock<T> range)
    {
        insertable.Insert(key, el);
        insertable.Insert(key, range);
    };

    /// --------------------------------------------------------------------------------------------
    /// Ensures {TInsertable} is {IndexInsertable} for type {T}.
    /// --------------------------------------------------------------------------------------------
    template <typename TInsertable, typename T>
    concept RIndexInsertable = RKeyInsertable<TInsertable, usize, T>;
}