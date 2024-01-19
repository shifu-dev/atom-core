#pragma once
#include "Atom/Core/TypeList.h"
#include "_VariantImpl.h"

namespace Atom
{
    /// --------------------------------------------------------------------------------------------
    /// # To Do
    /// - Check Requirements for assignments.
    /// - Check if requirements using TypeList functionality can be made concepts.
    /// --------------------------------------------------------------------------------------------
    template <typename... Ts>
    class Variant
    {
        static_assert(TypeList<Ts...>::AreUnique, "Every type in Ts... should be unique.");
        static_assert(TypeList<Ts...>::Count > 0, "At least one type needs to be specified.");

    private:
        using This = Variant<Ts...>;
        using _TImpl = _VariantImpl<Ts...>;

        template <typename... TOthers>
        friend class Variant;

    public:
        /// ----------------------------------------------------------------------------------------
        /// TypeList of this variant.
        /// ----------------------------------------------------------------------------------------
        using Types = TypeList<Ts...>;

    public:
        /// ----------------------------------------------------------------------------------------
        /// Check if this variant supports type `T`.
        /// ----------------------------------------------------------------------------------------
        template <typename T>
        static consteval auto Has() -> bool
        {
            return _TImpl::template HasType<T>();
        }

        /// ----------------------------------------------------------------------------------------
        /// Check if index `i` can be used to access value.
        /// ----------------------------------------------------------------------------------------
        template <usize i>
        static consteval auto Has() -> bool
        {
            return _TImpl::template HasIndex<i>();
        }

        /// ----------------------------------------------------------------------------------------
        /// Get type at index.
        /// ----------------------------------------------------------------------------------------
        template <usize i>
            requires(Has<i>())
        using TAt = typename _TImpl::template TypeAtIndex<i>;

        /// ----------------------------------------------------------------------------------------
        /// Get first type.
        /// ----------------------------------------------------------------------------------------
        using TFirst = typename _TImpl::TFirst;

        /// ----------------------------------------------------------------------------------------
        /// Get first type.
        /// ----------------------------------------------------------------------------------------
        using TLast = typename _TImpl::TLast;

        /// ----------------------------------------------------------------------------------------
        /// Index of type. This index than can be used to access value of type at that index.
        /// ----------------------------------------------------------------------------------------
        template <typename T>
        static consteval auto IndexOf() -> usize
            requires(Has<T>())
        {
            return _TImpl::template GetIndexForType<T>();
        }

        /// ----------------------------------------------------------------------------------------
        /// Count of types this variant supports.
        /// ----------------------------------------------------------------------------------------
        static consteval auto Count() -> usize
        {
            return _TImpl::GetTypeCount();
        }

    public:
        /// ----------------------------------------------------------------------------------------
        /// # Default Constructor
        /// ----------------------------------------------------------------------------------------
        constexpr Variant()
            requires(RDefaultConstructible<TFirst>) or (RIsVoid<TFirst>)
        {
            _impl.template constructValueByIndex<0>();
        }

        /// ----------------------------------------------------------------------------------------
        /// # Trivial Copy Constructor
        /// ----------------------------------------------------------------------------------------
        constexpr Variant(const Variant& that) = default;

        /// ----------------------------------------------------------------------------------------
        /// # Copy Constructor
        /// ----------------------------------------------------------------------------------------
        constexpr Variant(const Variant& that)
            requires(RCopyConstructibleAll<Ts...>) and (not RTriviallyCopyConstructibleAll<Ts...>)
        {
            _impl.constructValueFromVariant(that._impl);
        }

        /// ----------------------------------------------------------------------------------------
        /// # Template Copy Constructor
        /// ----------------------------------------------------------------------------------------
        template <typename... TOthers>
        constexpr Variant(const Variant<TOthers...>& that)
            requires(RCopyConstructibleAll<TOthers...>) and (Types::template Has<TOthers...>)
        {
            _impl.constructValueFromVariant(that._impl);
        }

        /// ----------------------------------------------------------------------------------------
        /// # Trivial Copy Operator
        /// ----------------------------------------------------------------------------------------
        constexpr Variant& operator=(const Variant& that) = default;

        /// ----------------------------------------------------------------------------------------
        /// # Copy Operator
        /// ----------------------------------------------------------------------------------------
        constexpr Variant& operator=(const Variant& that)
            requires(RCopyableAll<Ts...>) and (not RTriviallyCopyAssignableAll<Ts...>)
        {
            _impl.setValueFromVariant(that._impl);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # Template Copy Operator
        /// ----------------------------------------------------------------------------------------
        template <typename... TOthers>
        constexpr Variant& operator=(const Variant<TOthers...>& that)
            requires(RCopyableAll<TOthers...>) and (Types::template Has<TOthers...>)
        {
            _impl.setValueFromVariant(that._impl);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # Trivial Move Constructor
        /// ----------------------------------------------------------------------------------------
        constexpr Variant(Variant&& that) = default;

        /// ----------------------------------------------------------------------------------------
        /// # Move Constructor
        /// ----------------------------------------------------------------------------------------
        constexpr Variant(Variant&& that)
            requires(RMoveConstructibleAll<Ts...>) and (not RTriviallyMoveConstructibleAll<Ts...>)
        {
            _impl.constructValueFromVariant(mov(that._impl));
        }

        /// ----------------------------------------------------------------------------------------
        /// # Template Move Constructor
        /// ----------------------------------------------------------------------------------------
        template <typename... TOthers>
        constexpr Variant(Variant<TOthers...>&& that)
            requires(RMoveConstructibleAll<TOthers...>) and (Types::template Has<TOthers...>)
        {
            _impl.constructValueFromVariant(mov(that._impl));
        }

        /// ----------------------------------------------------------------------------------------
        /// # Trivial Move Operator
        /// ----------------------------------------------------------------------------------------
        constexpr Variant& operator=(Variant&& that) = default;

        /// ----------------------------------------------------------------------------------------
        /// # Move Operator
        /// ----------------------------------------------------------------------------------------
        constexpr Variant& operator=(Variant&& that)
            requires(RMoveableAll<Ts...>) and (not RTriviallyMoveAssignableAll<Ts...>)
        {
            _impl.setValueFromVariant(mov(that._impl));
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # Template Move Operator
        /// ----------------------------------------------------------------------------------------
        template <typename... TOthers>
        constexpr Variant& operator=(Variant<TOthers...>&& that)
            requires(RMoveableAll<Ts...>) and (Types::template Has<TOthers...>)
        {
            _impl.setValueFromVariant(mov(that._impl));
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # Value Copy Constructor
        ///
        /// Copy constructs variant with value of type T.
        ///
        /// # Parameters
        /// - `value`: Value to construct with.
        /// ----------------------------------------------------------------------------------------
        template <typename T>
        constexpr Variant(const T& value)
            requires(Has<T>())
        {
            _impl.template constructValueByType<T>(value);
        }

        /// ----------------------------------------------------------------------------------------
        /// # Value Move Constructor
        ///
        /// Move constructs variant with value of type T.
        ///
        /// # Parameters
        /// - `value`: Value to construct with.
        /// ----------------------------------------------------------------------------------------
        template <typename T>
        constexpr Variant(T&& value)
            requires(Has<T>())
        {
            _impl.template constructValueByType<T>(mov(value));
        }

        /// ----------------------------------------------------------------------------------------
        /// # Value Copy Operator
        ///
        /// Destroys previous value assigns new value.
        ///
        /// # Parameters
        /// - `value`: Value to assign.
        /// ----------------------------------------------------------------------------------------
        template <typename T>
        constexpr Variant& operator=(const T& value)
            requires(Has<T>())
        {
            _impl.setValue(value);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # Value Move Operator
        ///
        /// Destroys previous value assigns new value.
        ///
        /// # Parameters
        /// - `value`: Value to assign.
        /// ----------------------------------------------------------------------------------------
        template <typename T>
        constexpr Variant& operator=(T&& value)
            requires(Has<T>())
        {
            _impl.setValue(mov(value));
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # Trivial Destructor
        /// ----------------------------------------------------------------------------------------
        constexpr ~Variant() = default;

        /// ----------------------------------------------------------------------------------------
        /// # Destructor
        ///
        /// Destructs value.
        /// ----------------------------------------------------------------------------------------
        constexpr ~Variant()
            requires(not RTriviallyDestructibleAll<Ts...>)
        {
            _impl.destroyValue();
        }

    public:
        /// ----------------------------------------------------------------------------------------
        /// Constructs the type `T` and sets the value.
        ///
        /// # See Also
        /// - [`TAt`]
        /// ----------------------------------------------------------------------------------------
        template <typename T, typename... TArgs>
        constexpr auto emplace(TArgs&&... args)
            requires(Has<T>()) and (RConstructible<T, TArgs...>)
        {
            _impl.template emplaceValueByType<T>(forward<TArgs>(args)...);
        }

        /// ----------------------------------------------------------------------------------------
        /// Constructs the type for index `i` and sets the value.
        ///
        /// # See Also
        /// - [`TAt`]
        /// ----------------------------------------------------------------------------------------
        template <usize i, typename... TArgs>
        constexpr auto emplace(TArgs&&... args)
            requires(Has<i>()) and (RConstructible<TAt<i>, TArgs...>)
        {
            _impl.template emplaceValueByIndex<i>(forward<TArgs>(args)...);
        }

        /// ----------------------------------------------------------------------------------------
        /// Sets the value to `value`.
        ///
        /// # Parameters
        /// - `value`: Value to set.
        /// ----------------------------------------------------------------------------------------
        template <typename T>
        constexpr auto set(const T&& value)
            requires(Has<T>()) and (RCopyConstructible<T>)
        {
            _impl.setValue(value);
        }

        /// ----------------------------------------------------------------------------------------
        /// Sets the value to `value`.
        ///
        /// # Parameters
        /// - `value`: Value to set.
        /// ----------------------------------------------------------------------------------------
        template <typename T>
        constexpr auto set(T&& value)
            requires(Has<T>()) and (RMoveConstructible<T>)
        {
            _impl.setValue(mov(value));
        }

        /// ----------------------------------------------------------------------------------------
        /// Access the value as type `T`.
        ///
        /// # Template Parameters
        /// - `T`: Type to access variant's value as.
        /// ----------------------------------------------------------------------------------------
        template <typename T>
        constexpr auto as() const -> const T&
            requires(Has<T>()) and (not RIsVoid<T>)
        {
            Contracts::Expects(is<T>(), "Access to invalid type.");

            return _impl.template getValueByType<T>();
        }

        /// ----------------------------------------------------------------------------------------
        /// Access the value as type `T`.
        ///
        /// # Template Parameters
        /// - `T`: Type to access variant's value as.
        /// ----------------------------------------------------------------------------------------
        template <typename T>
        constexpr auto as() -> T&
            requires(Has<T>()) and (not RIsVoid<T>)
        {
            Contracts::DebugExpects(is<T>(), "Access to invalid type.");

            return _impl.template getValueByType<T>();
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        template <typename T>
        constexpr auto asCheck() const -> const T&
            requires(Has<T>()) and (not RIsVoid<T>)
        {
            Contracts::Expects(is<T>(), "Access to invalid type.");

            return _impl.template getValueByType<T>();
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        template <typename T>
        constexpr auto asCheck() -> T&
            requires(Has<T>()) and (not RIsVoid<T>)
        {
            Contracts::Expects(is<T>(), "Access to invalid type.");

            return _impl.template getValueByType<T>();
        }

        /// ----------------------------------------------------------------------------------------
        /// Access the value at index `i`.
        ///
        /// # Template Parameters
        /// - `i`: Index of type to access variants value as.
        ///
        /// # See Also
        /// - [`TAt`]
        /// ----------------------------------------------------------------------------------------
        template <usize i>
        constexpr auto at() const -> const TAt<i>&
            requires(Has<i>()) and (not RIsVoid<TAt<i>>)
        {
            Contracts::Expects(is<i>(), "Access to invalid type by index.");

            return _impl.template getValueByIndex<i>();
        }

        /// ----------------------------------------------------------------------------------------
        /// Access the value at index `i`.
        ///
        /// # Template Parameters
        /// - `i`: Index of type to access variants value as.
        ///
        /// # See Also
        /// - [`TAt`]
        /// ----------------------------------------------------------------------------------------
        template <usize i>
        constexpr auto at() -> TAt<i>&
            requires(Has<i>()) and (not RIsVoid<TAt<i>>)
        {
            Contracts::DebugExpects(is<i>(), "Access to invalid type by index.");

            return _impl.template getValueByIndex<i>();
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        template <usize i>
        constexpr auto atCheck() const -> const TAt<i>&
            requires(Has<i>()) and (not RIsVoid<TAt<i>>)
        {
            Contracts::Expects(is<i>(), "Access to invalid type by index.");

            return _impl.template getValueByIndex<i>();
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        template <usize i>
        constexpr auto atCheck() -> TAt<i>&
            requires(Has<i>()) and (not RIsVoid<TAt<i>>)
        {
            Contracts::Expects(is<i>(), "Access to invalid type by index.");

            return _impl.template getValueByIndex<i>();
        }

        /// ----------------------------------------------------------------------------------------
        /// Checks if current value is of type `T`.
        /// ----------------------------------------------------------------------------------------
        template <typename T>
        constexpr auto is() const -> bool
            requires(Has<T>())
        {
            return _impl.template isType<T>();
        }

        /// ----------------------------------------------------------------------------------------
        /// Checks if current value is of type accessed by index `i`.
        ///
        /// # See Also
        /// - [`TAt`]
        /// ----------------------------------------------------------------------------------------
        template <usize i>
        constexpr auto is() const -> bool
            requires(Has<i>())
        {
            return _impl.template isIndex<i>();
        }

        /// ----------------------------------------------------------------------------------------
        /// Get the index to current type.
        /// ----------------------------------------------------------------------------------------
        constexpr auto index() const -> usize
        {
            return _impl.getTypeIndex();
        }

    private:
        _TImpl _impl;
    };
}
