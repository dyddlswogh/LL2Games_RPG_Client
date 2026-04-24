#pragma once


namespace stb::enums
{
	enum class eComponentType
	{
		Transform,
		Collider,
		Rigidbody,
		Script,
		SpriteRenderer,
		Animator,
		Camera,
		AudioListener,
		AudioSource,
		EnumsEnd
	};

	enum class eLayerType
	{
		None,
		BackGround,
		Tile,
		Animal,
		Player,
		Floor,
		Particle,
		Max = 16,

	};

	enum class eResourceType
	{
		Texture,
		AudioClip,
		Animation,
		Prefab,
		EnumsEnd
	};

	enum class eColliderType
	{
		Circle2D,
		Rect2D,
		EnumsEnd
	};


}