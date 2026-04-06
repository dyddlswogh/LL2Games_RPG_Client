#pragma once
#include "CommonInclude.h"
#include "stbGameObject.h"


class stbD2DRenderer;

namespace stb
{
    class OtherPlayer : public GameObject
    {
    public:
        OtherPlayer();
        virtual ~OtherPlayer();

        void Initialize() override;
        void Update() override;
        void LateUpdate() override;
        void Render(HDC hdc) override;
        void Render(stbD2DRenderer& renderer) override;

        void SetCharacterId(const std::string& id) { mCharacterId = id; }
        std::string GetCharacterId() const { return mCharacterId; }

        void UpdatePosition(float x, float y);
        void SetTargetPosition(float x, float y, float speed);

        void AddFollower(GameObject* obj, Vector2 offset)
        {
            mFollowers.push_back({ obj, offset });
        }

    private:
        void SyncFollowers(Vector2 pos);

        std::string mCharacterId;
        Vector2 mTargetPosition;
        float mTargetSpeed;
        bool mHasTarget;
        float mInterpolationSpeed;

        struct Follower { GameObject* obj; Vector2 offset; };
        std::vector<Follower> mFollowers;
    };
}
