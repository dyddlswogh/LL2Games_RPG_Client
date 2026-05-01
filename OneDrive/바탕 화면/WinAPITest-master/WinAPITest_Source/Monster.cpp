#include "Monster.h"
#include "stbResourceManager.h"
#include "MonsterDataManager.h"
#include "stbTexture.h"
#include "Util.h"

#define M_RESOURCEMANAGER stb::SingletonBase<stb::ResourceManager>::getInstance()
#define M_MONSTERDATAMANAGER stb::SingletonBase<MonsterDataManager>::getInstance()

void Monster::Initialize()
{
	GameObject::Initialize();

	m_transform = AddComponent<stb::Transform>();
	m_animator = AddComponent<stb::Animator>();
	m_collider = AddComponent<stb::BoxCollider2D>();
	m_script = AddComponent<MonsterScript>();

	m_script->SetOwner(this);
}


void Monster::InitFromSpawn(const MonsterSpawnInfo& info)
{
	m_instanceId = info.instanceId;
	m_monsterId = info.monsterId;

	ResetFromSpawnInfo(info);
	SetAnimation();

	SetState(MonsterState::E_Move);
}

void Monster::Update(float deltaTime)
{
	GameObject::Update();

	stb::math::Vector2 diff = m_targetPos - m_pos;
	float dist = diff.length();

	//DebugMsg = "dist : " + std::to_string(dist) + "\n";
	//OutputDebugStringA(DebugMsg.c_str());

	if (dist > 1.0f)
	{
		float correctionSpeed = m_moveSpeed;
		//float correctionSpeed = m_moveSpeed * 2.0f;
		float moveDist = correctionSpeed * deltaTime;

		if (moveDist >= dist)
			m_pos = m_targetPos;
		else
			m_pos += diff.normalize() * moveDist;

		m_transform->SetPosition(m_pos);
	}
}

void Monster::Render(stbD2DRenderer& renderer)
{
	if (m_state == MonsterState::E_Die && m_isDeathAnimationFinished)
		return;


	GameObject::Render(renderer);
}

void Monster::SetState(MonsterState state)
{
	if (m_state == state)
		return;

	switch (state)
	{
		case MonsterState::E_Idle : 
			m_currentAnimation = L"idle";
			break;
		case MonsterState::E_Move:
			m_currentAnimation = L"move";
			break;
		case MonsterState::E_Hit:
			m_currentAnimation = L"hit";
			break;
		case MonsterState::E_Die:
			m_currentAnimation = L"die";
			break;
	}

	stb::Animator* animator = GetComponent<stb::Animator>();
	if (animator != nullptr)
	{
		DebugMsg ="current State : " + std::to_string(static_cast<int>(m_state)) + "\n";
		OutputDebugStringA(DebugMsg.c_str());
		animator->PlayAnimation(m_currentAnimation, true);
		OutputDebugStringA("Monster PlayAnimation \n");
	}


}
void Monster::SetPosition(float x, float y)
{

}

void Monster::SetAnimation()
{
	const MonsterData* data = M_MONSTERDATAMANAGER->FindMonsterData(m_monsterId);
	if (data == nullptr) 
	{
		std::string DebugMsg = "MonsterData is nullptr \n";
		OutputDebugStringA(DebugMsg.c_str());
		return;
	}
		

	stb::Animator* animator = GetComponent<stb::Animator>();
	if (animator == nullptr)
		return;

	for (const AnimationInfo& info : data->animations)
	{
		std::vector<stb::Texture*> frames;

		for (int i = 0; i < info.frame_count; ++i)
		{
			std::wstring key =
				utils::StringToWString(info.path + "/" + info.frame_prefix + std::to_string(i));
			
			std::wstring DebugMsg = key + L"\n";
			OutputDebugStringW(DebugMsg.c_str());

			stb::Texture* tex = M_RESOURCEMANAGER->Find<stb::Texture>(key);
			if (tex != nullptr)
				frames.emplace_back(tex);
		}
		std::string DebugMsg = info.anim_name + "\n";
		OutputDebugStringA(DebugMsg.c_str());

		DebugMsg = "frame size :" + std::to_string(frames.size()) + "\n";
		OutputDebugStringA(DebugMsg.c_str());

		animator->CreateFrameAnimation(
			utils::StringToWString(info.anim_name),
			frames,
			data->renderInfo.origin,
			0.2f
		);
	}
}
void Monster::OnDamaged(int damage, int curHp, bool dead)
{

}
void Monster::OnMove(float x, float y, int dir)
{
	
}

void Monster::ApplyServerUpdate(const MonsterUpdateInfo& info)
{
	m_targetPos = info.pos;


	m_dir = info.dir;
	//m_moveSpeed = info.moveSpeed;
	m_curHp = info.curHp;
	m_maxHp = info.maxHp;

	SetState(info.state);
}

void Monster::ApplyAttackResult(const AttackResult& result)
{
	m_curHp = result.cur_hp;
	m_maxHp = result.max_hp;

	if (result.isDead)
	{
		SetState(MonsterState::E_Die);
		return;
	}

	SetState(MonsterState::E_Hit);
}

void Monster::ResetFromSpawnInfo(const MonsterSpawnInfo& info)
{
	
	m_pos = info.pos;
	std::string DebugMsg = "Monster Pos X : " + std::to_string(m_pos.x) + "\n" +
		"Monster Pos Y : " + std::to_string(m_pos.y) + "\n";

	OutputDebugStringA(DebugMsg.c_str());
	m_dir = info.dir;
	m_moveSpeed = info.moveSpeed;
	m_curHp = info.curHp;
	m_maxHp = info.maxHp;
	m_state = info.state;

	m_isDead = false;

	m_state = MonsterState::E_NONE; // 있으면 추천
	m_isDead = false;

	if (m_transform != nullptr)
		m_transform->SetPosition(m_pos);
}