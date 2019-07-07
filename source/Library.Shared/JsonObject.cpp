#include "pch.h"
#include "JsonObject.h"

using namespace std;

namespace Library
{
	RTTI_DEFINITIONS(JsonObject)

	JsonObject::JsonObject(Json::Value&& value) :
		mValue(move(value))
	{
	}

	const Json::Value & JsonObject::Value() const
	{
		return mValue;
	}	
}