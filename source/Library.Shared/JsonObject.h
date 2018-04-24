#pragma once

#include <json/json.h>
#include "RTTI.h"

namespace Library
{
	class JsonObject final : public RTTI
	{
		RTTI_DECLARATIONS(JsonObject, RTTI)

	public:
		JsonObject(const JsonObject&) = default;
		JsonObject& operator=(const JsonObject&) = default;
		JsonObject(JsonObject&&) = default;
		JsonObject& operator=(JsonObject&&) = default;
		~JsonObject() = default;

		const Json::Value& Value() const;

	private:
		friend class JsonObjectReader;

		JsonObject(Json::Value&& value);

		Json::Value mValue;
	};
}