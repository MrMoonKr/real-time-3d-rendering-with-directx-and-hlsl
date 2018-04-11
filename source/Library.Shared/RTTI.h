#pragma once

#include <string>
#include <cstdint>

namespace Library
{
	class RTTI
	{
	public:
		virtual ~RTTI() = default;

		virtual std::uint64_t TypeIdInstance() const = 0;

		virtual RTTI* QueryInterface(const std::uint64_t)
		{
			return nullptr;
		}

		virtual bool Is(std::uint64_t) const
		{
			return false;
		}

		virtual bool Is(const std::string&) const
		{
			return false;
		}

		template <typename T>
		T* As() const
		{
			return (Is(T::TypeIdClass()) ? reinterpret_cast<T*>(const_cast<RTTI*>(this)) : nullptr);
		}

		virtual std::string ToString() const
		{
			return "RTTI";
		}

		virtual bool Equals(const RTTI* rhs) const
		{
			return this == rhs;
		}
	};

#define RTTI_DECLARATIONS(Type, ParentType)																		\
		public:																									\
			static std::string TypeName() { return std::string(#Type); }										\
			static std::uint64_t TypeIdClass() { return sRunTimeTypeId; }										\
			virtual std::uint64_t TypeIdInstance() const override { return Type::TypeIdClass(); }				\
			virtual Library::RTTI* QueryInterface(const std::uint64_t id) override								\
            {																									\
				return (id == sRunTimeTypeId ? reinterpret_cast<RTTI*>(this) : ParentType::QueryInterface(id)); \
            }																									\
			virtual bool Is(std::uint64_t id) const override													\
			{																									\
				return (id == sRunTimeTypeId ? true : ParentType::Is(id));                                      \
			}																									\
			virtual bool Is(const std::string& name) const override												\
			{																									\
				return (name == TypeName() ? true : ParentType::Is(name));                                      \
			}																									\
			private:																							\
				static std::uint64_t sRunTimeTypeId;

#define RTTI_DEFINITIONS(Type) std::uint64_t Type::sRunTimeTypeId = reinterpret_cast<std::uint64_t>(&Type::sRunTimeTypeId);
}