/*********************************************************************
 * @file   component.h
 * @author Tomomi Murakami
 * @date   2021/12/04 22:18
 * @brief  コンポーネント
 ********************************************************************/
#pragma once

#define WX2_DECLARE_COMPONENT_IMPL(name)					\
private:													\
	static constexpr std::string_view GetTypeName() 		\
	{														\
		return #name;										\
	}														\
	static std::size_t GetTypeHash() 						\
	{														\
		return StringHash{}(#name);							\
	}														\
	friend IComponent

namespace wx2
{
	using ComponentType = std::uint8_t;

	class IComponent
	{
	public:
		IComponent() = default;
		virtual ~IComponent() = default;

		WX2_DISALLOW_COPY(IComponent);
		WX2_DISALLOW_MOVE(IComponent);

		virtual void Update() {};
		virtual void Draw() {};

		virtual void OnCreate() {};
		virtual void OnDestroy() {};
	};
}