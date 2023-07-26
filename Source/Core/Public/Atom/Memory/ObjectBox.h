#pragma once
#include "Atom/Core.h"
#include "Atom/Exceptions.h"
#include "Atom/TTI.h"
#include "Atom/Invokable/InvokablePtr.h"

#include "DefaultMemAllocator.h"

namespace Atom
{
    namespace Internal
    {
        /// ----------------------------------------------------------------------------------------
        /// {ObjectBoxIdentifier} is used to check if the type is same as or derived from 
        /// {ObjectBox} template.
        /// ----------------------------------------------------------------------------------------
        class ObjectBoxIdentifier { };
    }

    /// --------------------------------------------------------------------------------------------
    /// Stores object inside using type erasure.
    /// 
    /// @TPARAM Copyable Should the {ObjectBox} be {CopyConstructible} and {CopyAssignable}.
    /// @TPARAM Movable Should the {ObjectBox} be {MoveConstructible} and {MoveAssignable}.
    /// @TPARAM AllowNonMovableObject Should the {Object} be {MoveConstructible} when {Movable} is true.
    ///     {Object} can be allocated on heap if it's not movable.
    /// @TPARAM StackSize Size of stack to store object, to avoid heap allocation.
    /// @TPARAM TMemAllocator {MemAllocator} to allocate memory to store object.
    /// --------------------------------------------------------------------------------------------
    template <bool Copyable, bool Movable, bool AllowNonMovableObject, usize StackSize,
        tname TMemAllocator = DefaultMemAllocator>
    class ObjectBox: public Internal::ObjectBoxIdentifier
    {
        template <bool OtherCopyable, bool OtherMovable, bool OtherAllowNonMovableObject,
            usize OtherStackSize, tname TOtherMemAllocator>
        friend class ObjectBox;

        /// --------------------------------------------------------------------------------------------
        /// Stores data for object. Like, {CopyConstructor}, {MoveConstructor} and {ObjectSize}.
        /// --------------------------------------------------------------------------------------------
        class ObjectData
        {
            pub ATOM_CONDITIONAL_FIELD(Copyable,
                InvokablePtr<void(void*, const void*)>) copy;

            pub ATOM_CONDITIONAL_FIELD(Movable,
                InvokablePtr<void(void*, void*)>) move;

            pub InvokablePtr<void(void* obj)> dtor_;

            pub usize size;
            pub void* obj;
            pub const TypeInfo* type;
        };

        /// ----------------------------------------------------------------------------------------
        /// Requirements for {Object} accepted by this {ObjectBox}.
        /// ----------------------------------------------------------------------------------------
        template <tname T>
        static cexpr bool RObject = requires
        {
            // {ObjectBox} variants are not stored inside {ObjectBox} variants.
            requires RNotDerivedFrom<T, Internal::ObjectBoxIdentifier>;

            // If Box is {Copyable} then the object should also be {CopyConstructible}.
            requires !Copyable || RCopyConstructible<T>;

            // If Box is {Movable} then the object should also be {MoveConstructible} unless
            // {NonMovableObject} is allowed.
            requires !Movable || (RMoveConstructible<T> || AllowNonMovableObject);
        };

        /// ----------------------------------------------------------------------------------------
        /// Requirements for other {ObjectBox} accepted by this {ObjectBox}.
        /// For example, {CopyConstructor} and {MoveConstructor}.
        /// ----------------------------------------------------------------------------------------
        template <bool OtherCopyable, bool OtherMovable, bool OtherAllowNonMovableObject>
        static cexpr bool ROtherBox = requires
        {
            // If this {Box} is {Copyable} the {OtherBox} should also be {Copyable}.
            requires !Copyable || OtherCopyable;

            // If this {Box} is {Movable} the {OtherBox} should also be {Movable} unless
            // {NonMovableObject} is allowed.
            requires !Movable || (OtherMovable || AllowNonMovableObject);

            // If this {Box} does not allow {NonMovableObject} then {OtherBox} should also not 
            // allow {NonMovableObject}.
            requires !Movable || (AllowNonMovableObject || !OtherAllowNonMovableObject);
        };

    //// -------------------------------------------------------------------------------------------
    //// Constructors and Operators.
    //// -------------------------------------------------------------------------------------------

    public:
        /// ----------------------------------------------------------------------------------------
        /// DefaultConstructor.
        /// ----------------------------------------------------------------------------------------
        cexpr ctor ObjectBox() noex:
            _object(), _heapMem(nullptr), _heapMemSize(0), _memAllocator() { }

        /// ----------------------------------------------------------------------------------------
        /// NullConstructor.
        /// ----------------------------------------------------------------------------------------
        cexpr ctor ObjectBox(NullType null) noex:
            ObjectBox() { }

        /// ----------------------------------------------------------------------------------------
        /// NullAssignmentOperator.
        /// ----------------------------------------------------------------------------------------
        fn op=(NullType null) -> ObjectBox&
        {
            _DisposeObject();
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// NullEqualityOperator.
        /// ----------------------------------------------------------------------------------------
        fn op==(NullType null) const noex -> bool
        {
            return _HasObject();
        }

        /// ----------------------------------------------------------------------------------------
        /// Constructor. Initializes with object.
        /// ----------------------------------------------------------------------------------------
        template <tname T>
        requires RObject<T>
        ctor ObjectBox(T&& obj): ObjectBox()
        {
            _InitObject(fwd(obj));
        }

        /// ----------------------------------------------------------------------------------------
        /// AssignmentOperator. Assigns new object.
        /// ----------------------------------------------------------------------------------------
        template <tname T>
        requires RObject<T>
        fn op=(T&& object) -> ObjectBox&
        {
            _SetObject(fwd(object));
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// CopyConstructor.
        /// ----------------------------------------------------------------------------------------
        ctor ObjectBox(const ObjectBox& other)
            requires Copyable : ObjectBox()
        {
            _CopyBox(other);
        }

        /// ----------------------------------------------------------------------------------------
        /// CopyConstructorTemplate.
        /// ----------------------------------------------------------------------------------------
        template <bool OtherMovable, bool OtherAllowNonMovableObject,
            usize OtherStackSize, tname TOtherMemAllocator>
        requires Copyable && ROtherBox<Copyable, OtherMovable, OtherAllowNonMovableObject>
        ctor ObjectBox(const ObjectBox<Copyable, OtherMovable, OtherAllowNonMovableObject, 
            OtherStackSize, TOtherMemAllocator>& other): ObjectBox()
        {
            _CopyBox(other);
        }

        /// ----------------------------------------------------------------------------------------
        /// CopyAssignmentOperator.
        /// ----------------------------------------------------------------------------------------
        fn op=(const ObjectBox& other) -> ObjectBox&
            requires Copyable
        {
            _CopyBox(other);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// CopyAssignmentOperatorTemplate.
        /// ----------------------------------------------------------------------------------------
        template <bool OtherMovable, bool OtherAllowNonMovableObject, 
            usize OtherStackSize, tname TOtherMemAllocator>
        requires Copyable && ROtherBox<Copyable, OtherMovable, OtherAllowNonMovableObject>
        fn op=(const ObjectBox<Copyable, OtherMovable, OtherAllowNonMovableObject,
            OtherStackSize, TOtherMemAllocator>& other) -> ObjectBox&
        {
            _CopyBox(other);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// MoveConstructor.
        /// ----------------------------------------------------------------------------------------
        ctor ObjectBox(ObjectBox&& other)
            requires Movable : ObjectBox()
        {
            _MoveBox(mov(other));
        }

        /// ----------------------------------------------------------------------------------------
        /// MoveConstructorTemplate.
        /// ----------------------------------------------------------------------------------------
        template <bool OtherCopyable, bool OtherMovable, bool OtherAllowNonMovableObject,
            usize OtherStackSize, tname TOtherMemAllocator>
        requires Movable && ROtherBox<OtherCopyable, OtherMovable, OtherAllowNonMovableObject>
        ctor ObjectBox(ObjectBox<OtherCopyable, OtherMovable, OtherAllowNonMovableObject,
            OtherStackSize, TOtherMemAllocator>&& other): ObjectBox()
        {
            _MoveBox(mov(other));
        }

        /// ----------------------------------------------------------------------------------------
        /// MoveAssignmentOperator.
        /// ----------------------------------------------------------------------------------------
        fn op=(ObjectBox&& other) -> ObjectBox&
            requires Movable
        {
            _MoveBox(mov(other));
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// MoveAssignmentOperatorTemplate.
        /// ----------------------------------------------------------------------------------------
        template <bool OtherCopyable, bool OtherMovable, bool OtherAllowNonMovableObject,
            usize OtherStackSize, tname TOtherMemAllocator>
        requires Movable && ROtherBox<OtherCopyable, OtherMovable, OtherAllowNonMovableObject>
        fn op=(ObjectBox<OtherCopyable, OtherMovable, OtherAllowNonMovableObject,
            OtherStackSize, TOtherMemAllocator>&& other) -> ObjectBox&
        {
            _MoveBox(mov(other));
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// Destructor.
        /// ----------------------------------------------------------------------------------------
        dtor ObjectBox()
        {
            _DisposeBox();
        }

    //// -------------------------------------------------------------------------------------------
    //// Functions
    //// -------------------------------------------------------------------------------------------

    public:
        /// ----------------------------------------------------------------------------------------
        /// Sets the new object.
        /// ----------------------------------------------------------------------------------------
        template <tname T>
        requires RObject<T>
        fn SetObject(T&& obj)
        {
            _SetObject(fwd(obj));
        }

        /// ----------------------------------------------------------------------------------------
        /// Get the object.
        /// ----------------------------------------------------------------------------------------
        template <tname T>
        fn GetObject() noex -> T&
        {
            return *_GetObject<T>();
        }

        /// ----------------------------------------------------------------------------------------
        /// Get the const object.
        /// ----------------------------------------------------------------------------------------
        template <tname T>
        fn GetObject() const noex -> const T&
        {
            return *_GetObject<T>();
        }

    //// -------------------------------------------------------------------------------------------
    //// Box Manipulation Functions
    //// -------------------------------------------------------------------------------------------

    protected:
        /// ----------------------------------------------------------------------------------------
        /// Copies {other} {ObjectBox} into {this} {ObjectBox}.
        /// ----------------------------------------------------------------------------------------
        template <bool OtherMovable, bool OtherAllowNonMovableObject,
            usize OtherStackSize, tname TOtherMemAllocator>
        requires Copyable && ROtherBox<Copyable, OtherMovable, OtherAllowNonMovableObject>
        fn _CopyBox(const ObjectBox<Copyable, OtherMovable, OtherAllowNonMovableObject,
            OtherStackSize, TOtherMemAllocator>& other)
        {
            _CopyObject(other);
        }

        /// ----------------------------------------------------------------------------------------
        /// Moves {other} {ObjectBox} into {this} {ObjectBox}.
        /// ----------------------------------------------------------------------------------------
        template <bool OtherCopyable, bool OtherMovable, bool OtherAllowNonMovableObject,
            usize OtherStackSize, tname TOtherMemAllocator>
        requires Movable && ROtherBox<OtherCopyable, OtherMovable, OtherAllowNonMovableObject>
        fn _MoveBox(ObjectBox<OtherCopyable, OtherMovable, OtherAllowNonMovableObject,
            OtherStackSize, TOtherMemAllocator>&& other)
        {
            // When allocator type is different, we cannot handle heap memory.
            // So we only move the object.
            if cexpr (!RSameAs<TMemAllocator, TOtherMemAllocator>)
            {
                _MoveObject(other);
                other._DisposeBox();
                return;
            }

            _DisposeObject();

            const usize otherObjSize = other._object.size;
            const bool otherIsUsingStackMem = other._IsUsingStackMem();
            if (otherIsUsingStackMem && otherObjSize > StackSize && _heapMemSize >= otherObjSize && 
                other._heapMemSize < otherObjSize)
            {
                // We cannot deallocate our memory in the above scenario.
                other._ReleaseMem();
            }
            else
            {
                _ReleaseMem();

                _heapMem = mov(other._heapMem);
                _heapMemSize = mov(other._heapMemSize);
                _memAllocator = mov(other._memAllocator);
            }

            if (otherIsUsingStackMem)
            {
                _MoveObject(mov(other));
            }
            else
            {
                _CopyObjectData(other);
            }
        }

        /// ----------------------------------------------------------------------------------------
        /// Destroy stored object and releases any allocated memory.
        /// ----------------------------------------------------------------------------------------
        fn _DisposeBox()
        {
            _DisposeObject();
            _ReleaseMem();
        }

    //// -------------------------------------------------------------------------------------------
    //// Object Manipulation Functions
    //// -------------------------------------------------------------------------------------------

    protected:
        /// ----------------------------------------------------------------------------------------
        /// Stores the object.
        /// 
        /// @TPARAM T Type of object to store.
        /// 
        /// @PARAM[IN] obj Object to store.
        /// @PARAM[IN] forceHeap (default = false) Force store on heap.
        /// 
        /// @EXPECTS Previous object is not set.
        /// ----------------------------------------------------------------------------------------
        template <tname T>
        requires RObject<T>
        fn _InitObject(T&& obj, bool forceHeap = false)
        {
            _object.size = sizeof(T);
            _object.type = &typeid(T);

            _object.dtor_ = [](void* obj)
            {
                reinterpret_cast<T*>(obj)->T::~T();
            };

            if cexpr (Copyable)
            {
                _object.copy = [](void* obj, const void* other)
                {
                    new (obj) T(*reinterpret_cast<const T*>(other));
                };
            }

            if cexpr (Movable)
            {
                if cexpr (RMoveConstructible<T>)
                {
                    _object.move = [](void* obj, void* other)
                    {
                        new (obj) T(mov(*reinterpret_cast<T*>(other)));
                    };
                }
                else
                {
                    _object.move = nullptr;
                }
            }

            // If the object is not movable but AllowNonMovableObject is allowed,
            // we allocate it on heap to avoid object's move constructor.
            if cexpr (Movable && AllowNonMovableObject && !RMoveConstructible<T>)
            {
                forceHeap = true;
            }

            _object.obj = _AllocMem(_object.size, forceHeap);
            new (_object.obj) T(fwd(obj));
        }

        /// ----------------------------------------------------------------------------------------
        /// Destroys previous object if any and stores new object.
        /// 
        /// @TPARAM T Type of object to store.
        /// 
        /// @PARAM[IN] obj Object to store.
        /// @PARAM[IN] forceHeap (default = false) Force store on heap.
        /// ----------------------------------------------------------------------------------------
        template <tname T>
        requires RObject<T>
        fn _SetObject(T&& obj, bool forceHeap = false)
        {
            _DisposeObject();
            _InitObject(fwd(obj));
        }

        /// ----------------------------------------------------------------------------------------
        /// Get pointer to stored object.
        /// 
        /// @TPARAM T Type as which to get the object.
        /// ----------------------------------------------------------------------------------------
        template <tname T = void>
        fn _GetObject() noex -> T*
        {
            return reinterpret_cast<T*>(_object.obj);
        }

        /// ----------------------------------------------------------------------------------------
        /// Get {TypeInfo} or stored object.
        /// ----------------------------------------------------------------------------------------
        fn _GetObjectType() const noex -> const TypeInfo&
        {
            return *_object.type;
        }

        /// ----------------------------------------------------------------------------------------
        /// Checks if object is not {null}.
        /// ----------------------------------------------------------------------------------------
        fn _HasObject() const noex -> bool
        {
            return _object.obj != nullptr;
        }

        /// ----------------------------------------------------------------------------------------
        /// Copies the object from {other} {ObjectBox} into {this} {ObjectBox}.
        /// 
        /// @PARAM[IN] other {ObjectBox} of which to copy object.
        /// @PARAM[IN] forceHeap (default = false) Force allocate object on heap.
        /// ----------------------------------------------------------------------------------------
        template <bool OtherMovable, bool OtherAllowNonMovableObject,
            usize OtherStackSize, tname TOtherMemAllocator>
        requires Copyable && ROtherBox<Copyable, OtherMovable, OtherAllowNonMovableObject>
        fn _CopyObject(const ObjectBox<Copyable, OtherMovable, OtherAllowNonMovableObject,    
            OtherStackSize, TOtherMemAllocator>& other, bool forceHeap = false)
        {
            _DisposeObject();

            _CopyObjectData(other);

            if cexpr (Movable)
            {
                forceHeap = forceHeap || _object.move == nullptr;
            }
            else
            {
                forceHeap = true;
            }

            _object.obj = _AllocMem(_object.size, forceHeap);
            _object.copy(_object.obj, other._object.obj);
        }

        /// ----------------------------------------------------------------------------------------
        /// Moves the object from {other} {ObjectBox} into {this} {ObjectBox}.
        /// 
        /// @PARAM[IN] other {ObjectBox} of which to move object.
        /// @PARAM[IN] forceHeap (default = false) Force allocate object on heap.
        /// 
        /// @NOTE This doesn't moves the memory from {other} {ObjectBox}.
        /// ----------------------------------------------------------------------------------------
        template <bool OtherCopyable, bool OtherMovable, bool OtherAllowNoneMovableObject,
            usize OtherStackSize, tname TOtherMemAllocator>
        requires Movable && ROtherBox<OtherCopyable, OtherMovable, OtherAllowNoneMovableObject>
        fn _MoveObject(ObjectBox<OtherCopyable, OtherMovable, OtherAllowNoneMovableObject,
            OtherStackSize, TOtherMemAllocator>&& other, bool forceHeap = false)
        {
            _DisposeObject();

            _CopyObjectData(other);
            forceHeap = forceHeap || _object.move == nullptr;

            _object.obj = _AllocMem(_object.size, forceHeap);
            _object.move(_object.obj, other._object.obj);
        }

        /// ----------------------------------------------------------------------------------------
        /// Disposes current object by calling its destructor.
        /// 
        /// @NOTE This does'n deallocates memory.
        /// 
        /// @SEE _ReleaseMem().
        /// ----------------------------------------------------------------------------------------
        fn _DisposeObject()
        {
            if (_object.obj != nullptr)
            {
                _object.dtor_(_object.obj);
                _object = {};
            }
        }

    private:
        /// ----------------------------------------------------------------------------------------
        /// Copies {ObjectData} from {otherBox}.
        /// 
        /// @PARAM[IN] otherBox {ObjectBox} of which to copy {ObjectData}.
        /// ----------------------------------------------------------------------------------------
        template <bool OtherCopyable, bool OtherMovable, bool OtherAllowNonMovableObject,
            usize OtherStackSize, tname TOtherMemAllocator>
        fn _CopyObjectData(const ObjectBox<OtherCopyable, OtherMovable,
            OtherAllowNonMovableObject, OtherStackSize, TOtherMemAllocator>& otherBox)
        {
            auto& other = otherBox._object;
            
            _object.obj = other.obj;
            _object.size = other.size;
            _object.type = other.type;
            _object.dtor_ = other.dtor_;

            if cexpr (Copyable)
            {
                if cexpr (Movable)
                {
                    _object.copy = other.copy;
                }
                else
                {
                    _object.copy = nullptr;
                }
            }

            if cexpr (Movable)
            {
                if cexpr (OtherMovable)
                {
                    _object.move = other.move;
                }
                else
                {
                    _object.move = nullptr;
                }
            }
        }

    //// -------------------------------------------------------------------------------------------
    //// Memory Manipulation Functions
    //// -------------------------------------------------------------------------------------------

    protected:
        /// ----------------------------------------------------------------------------------------
        /// Allocates enough memory of size {size}. Uses stack memory if it is big enough.
        /// 
        /// @PARAM[IN] size Size of memory to allocate.
        /// @PARAM[IN] forceHeap If {true}, allocates memory from {TMemAllocator}.
        /// 
        /// @RETURNS Pointer to the allocated memory.
        /// ----------------------------------------------------------------------------------------
        fn _AllocMem(usize size, bool forceHeap = false) -> void*
        {
            if cexpr (StackSize > 0)
            {
                // Check if stack memory is big enough.
                if (!forceHeap && size <= StackSize)
                {
                    return _stackMem;
                }
            }

            // If we have previously allocated memory.
            if (_heapMem != nullptr)
            {
                if (_heapMemSize < size)
                {
                    _heapMem = _memAllocator.Realloc(_heapMem, size);
                    _heapMemSize = size;
                }
            }
            // We need to allocate heap memory.
            else
            {
                _heapMem = _memAllocator.Alloc(size);
                _heapMemSize = size;
            }

            return _heapMem;
        }

        /// ----------------------------------------------------------------------------------------
        /// Deallocates any allocated memory.
        /// ----------------------------------------------------------------------------------------
        fn _ReleaseMem()
        {
            if (_heapMem != nullptr)
            {
                _memAllocator.Dealloc(_heapMem);
                _heapMem = nullptr;
                _heapMemSize = 0;
            }
        }

        /// ----------------------------------------------------------------------------------------
        /// Is object is stored in stack memory.
        /// ----------------------------------------------------------------------------------------
        fn _IsUsingStackMem() const noex -> bool
        {
            if cexpr (StackSize > 0)
            {
                return _object.obj == _stackMem;
            }

            return false;
        }

    //// -------------------------------------------------------------------------------------------
    //// Fields
    //// -------------------------------------------------------------------------------------------

    protected:
        /// ----------------------------------------------------------------------------------------
        /// Stack Memory.
        /// ----------------------------------------------------------------------------------------
        ATOM_CONDITIONAL_FIELD(StackSize > 0,
            byte[StackSize]) _stackMem;

        /// ----------------------------------------------------------------------------------------
        /// Memory Allocator.
        /// ----------------------------------------------------------------------------------------
        TMemAllocator _memAllocator;

        /// ----------------------------------------------------------------------------------------
        /// Heap memory allocated.
        /// ----------------------------------------------------------------------------------------
        void* _heapMem;

        /// ----------------------------------------------------------------------------------------
        /// Size of heap memory allocated.
        /// ----------------------------------------------------------------------------------------
        usize _heapMemSize;

        /// ----------------------------------------------------------------------------------------
        /// Object data.
        /// ----------------------------------------------------------------------------------------
        ObjectData _object;
    };
}