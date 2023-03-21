#pragma once
#include "Atom/Logging/Logger.h"
#include "Atom/Logging/LogTarget.h"
#include "Atom/Logging/LogTargets/MultiLogTarget.h"
#include <functional>

namespace Atom::Logging::Internal
{
    /// --------------------------------------------------------------------------------------------
    /// --- DOC_TEMPLATE 
    /// --------------------------------------------------------------------------------------------
    /// Basic Logger object.
    /// 
    /// @THREAD_SAFETY SAFE
    /// --------------------------------------------------------------------------------------------
    template <bool ST>
    class SimpleLoggerTemplate: public Logger
    {
        using MultiLogTarget = TTI::TConditional<ST, MultiLogTargetST, MultiLogTargetMT>;
        using AtomicLogLevel = TTI::TConditional<ST, ELogLevel, Atomic<ELogLevel>>;

    public:
        /// ----------------------------------------------------------------------------------------
        /// DefaultConstructor.
        /// 
        /// @PARAM[IN] name Name of this logger.
        /// ----------------------------------------------------------------------------------------
        explicit SimpleLoggerTemplate(String name) noexcept:
            _name(MOVE(name)), targets() { }

        /// ----------------------------------------------------------------------------------------
        /// Constructs and adds LogTarget object.
        /// Constructs {this->targets(target)}.
        /// 
        /// @PARAM[IN] name Name of this logger.
        /// @PARAM[IN] target LogTarget object to add.
        /// ----------------------------------------------------------------------------------------
        SimpleLoggerTemplate(String name, LogTargetPtr target) noexcept:
            _name(MOVE(name)), targets(target) { }

        /// ----------------------------------------------------------------------------------------
        /// Constructs and adds LogTarget objects.
        /// Constructs {this->targets(targets)}.
        /// 
        /// @PARAM[IN] name Name of this logger.
        /// @PARAM[IN] targets LogTarget objects to add.
        /// ----------------------------------------------------------------------------------------
        SimpleLoggerTemplate(String name, InitializerList<LogTargetPtr> targets) noexcept:
            _name(MOVE(name)), targets(targets) { }

        /// ----------------------------------------------------------------------------------------
        /// Constructs and adds LogTarget objects.
        /// Constructs {this->targets(targets)}.
        /// 
        /// @PARAM[IN] name Name of this logger.
        /// @PARAM[IN] targets LogTarget objects to add.
        /// ----------------------------------------------------------------------------------------
        SimpleLoggerTemplate(String name, const RConstIterable<LogTargetPtr> auto& targets)
            noexcept: _name(MOVE(name)), targets(targets) { }

        /// ----------------------------------------------------------------------------------------
        /// Constructs and adds LogTarget objects.
        /// Constructs {this->targets(begin, end)}.
        /// 
        /// @PARAM[IN] name Name of this logger.
        /// @PARAM[IN] begin RConstIterator to beginning of range to add.
        /// @PARAM[IN] end RConstIterator to end of range to add.
        /// ----------------------------------------------------------------------------------------
        SimpleLoggerTemplate(String name, RConstIterator<LogTargetPtr> auto begin,
            RConstIterator<LogTargetPtr> auto end) noexcept:
            _name(MOVE(name)), targets(begin, end) { }

    public:
        /// ----------------------------------------------------------------------------------------
        /// Get the name of this Logger.
        /// 
        /// @THREAD_SAFETY SAFE
        /// ----------------------------------------------------------------------------------------
        StringView Name() const noexcept override final
        {
            return _name;
        }

        /// ----------------------------------------------------------------------------------------
        /// Writes to all LogTarget objects.
        /// 
        /// @THREAD_SAFETY SAFE
        /// ----------------------------------------------------------------------------------------
        void Log(LogMsg& logMsg) override
        {
            targets.Write(logMsg);
        }

        /// ----------------------------------------------------------------------------------------
        /// Flushes all LogTarget objects.
        /// 
        /// @THREAD_SAFETY SAFE
        /// ----------------------------------------------------------------------------------------
        void Flush() override
        {
            targets.Flush();
        }

        /// ----------------------------------------------------------------------------------------
        /// Set log level.
        /// 
        /// @THREAD_SAFETY SAFE
        /// ----------------------------------------------------------------------------------------
        void SetLogLevel(ELogLevel lvl) noexcept
        {
            _logLevel = lvl;
        }

        /// ----------------------------------------------------------------------------------------
        /// Get log level.
        /// 
        /// @THREAD_SAFETY SAFE
        /// ----------------------------------------------------------------------------------------
        ELogLevel GetLogLevel(ELogLevel lvl) noexcept
        {
            return _logLevel;
        }

        /// ----------------------------------------------------------------------------------------
        /// Checks if Log of level {lvl} should be logged.
        /// 
        /// @THREAD_SAFETY SAFE
        /// ----------------------------------------------------------------------------------------
        bool CheckLogLevel(ELogLevel lvl) const noexcept override final
        {
            return lvl != ELogLevel::OFF && lvl >= _logLevel;
        }

        /// ----------------------------------------------------------------------------------------
        /// Set flush level.
        /// 
        /// @THREAD_SAFETY SAFE
        /// ----------------------------------------------------------------------------------------
        void SetFlushLevel(ELogLevel lvl) noexcept
        {
            _flushLevel = lvl;
        }

        /// ----------------------------------------------------------------------------------------
        /// Get flush level.
        /// 
        /// @THREAD_SAFETY SAFE
        /// ----------------------------------------------------------------------------------------
        ELogLevel GetFlushLevel() const noexcept
        {
            return _flushLevel;
        }

        /// ----------------------------------------------------------------------------------------
        /// Checks if Log of level {lvl} should be flushed.
        /// 
        /// @THREAD_SAFETY SAFE
        /// ----------------------------------------------------------------------------------------
        bool CheckFlushLevel(ELogLevel lvl) const noexcept
        {
            return lvl != ELogLevel::OFF && lvl >= _flushLevel;
        }

    public:
        /// ----------------------------------------------------------------------------------------
        /// The object used to manage LogTarget objects. Accessing it is thread safe.
        /// 
        /// @THREAD_SAFETY SAFE
        /// ----------------------------------------------------------------------------------------
        MultiLogTarget targets;

    protected:
        /// ----------------------------------------------------------------------------------------
        /// Name of this logger.
        /// ----------------------------------------------------------------------------------------
        const String _name;

        /// ----------------------------------------------------------------------------------------
        /// ELogLevel used to filter logs.
        /// ----------------------------------------------------------------------------------------
        AtomicLogLevel _logLevel;

        /// ----------------------------------------------------------------------------------------
        /// ELogLevel used to filter flush after logs.
        /// ----------------------------------------------------------------------------------------
        AtomicLogLevel _flushLevel;
    };
}

namespace Atom::Logging
{
    using SimpleLoggerST = Internal::SimpleLoggerTemplate<true>;
    using SimpleLoggerMT = Internal::SimpleLoggerTemplate<false>;
}