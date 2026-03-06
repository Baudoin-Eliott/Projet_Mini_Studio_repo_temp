#pragma once

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include "Texture.h"
<<<<<<< Updated upstream
=======
#include <memory>
#include <variant>
>>>>>>> Stashed changes


namespace sf 
{
	class Shape;
    class Color;
}

class Scene;

class Entity
{
    struct Target 
    {
		sf::Vector2i position;
        float distance;
		bool isSet;
    };

protected:
	sf::Sprite mSprite;
<<<<<<< Updated upstream
    sf::CircleShape mShape;
    sf::Vector2f mDirection;
=======
	std::variant<sf::CircleShape, sf::RectangleShape> mShape;
	sf::Vector2f mDirection;
>>>>>>> Stashed changes
	Target mTarget;
    float mSpeed = 0.f;
    bool mToDestroy = false;
    int mTag = -1;
	bool mRigidBody = false;
	bool mIsSatic = false;

public:
	void SetTexture(std::shared_ptr<Texture> _texture) {
		mSprite.setTexture(*_texture->getTexture());
<<<<<<< Updated upstream
=======

		sf::Vector2f size = std::visit([](const auto& shape) -> sf::Vector2f {
			using T = std::decay_t<decltype(shape)>;
			if constexpr (std::is_same_v<T, sf::CircleShape>)
			{
				float diameter = shape.getRadius() * 2;
				return { diameter, diameter };
			}
			else
				return shape.getSize();
			}, mShape);

		float scaleX = size.x / _texture->getWidth();
		float scaleY = size.y / _texture->getHeight();
		mSprite.setScale(scaleX, scaleY);

>>>>>>> Stashed changes
	}


	bool HasTexture() {
		return mSprite.getTexture() != nullptr;
	}
	void SetStatic(bool _isStatic) { mIsSatic = _isStatic; }
	bool GoToDirection(int x, int y, float speed = -1.f);
    bool GoToPosition(int x, int y, float speed = -1.f);
    void SetPosition(float x, float y, float ratioX = 0.5f, float ratioY = 0.5f);
	void SetDirection(float x, float y, float speed = -1.f);
	void SetSpeed(float speed) { mSpeed = speed; }
	void SetTag(int tag) { mTag = tag; }
<<<<<<< Updated upstream
	float GetRadius() const { return mShape.getRadius(); }
	void SetRigidBody(bool isRigitBody) { mRigidBody = isRigitBody; }
	bool IsRigidBody() const { return mRigidBody; }

    sf::Vector2f GetPosition(float ratioX = 0.5f, float ratioY = 0.5f) const;
	sf::Shape* GetShape() { return &mShape; }
=======
	float GetRadius() const;
	void SetRigidBody(bool isRigidBody) { mRigidBody = isRigidBody; }
	bool IsRigidBody() const { return mRigidBody; }

	sf::Vector2f GetPosition(float ratioX = 0.5f, float ratioY = 0.5f) const;
	sf::Shape* GetShape() {
		return std::visit([](auto& shape) -> sf::Shape* { return &shape; }, mShape);
	}

>>>>>>> Stashed changes

	bool IsTag(int tag) const { return mTag == tag; }
    bool IsColliding(Entity* other) const;
	bool IsInside(float x, float y) const;

    void Destroy();
	bool ToDestroy() const { return mToDestroy; }
	
	template<typename T>
	T* GetScene() const;

    Scene* GetScene() const;
	float GetDeltaTime() const;

    template<typename T>
    T* CreateEntity(float radius, const sf::Color& color);

	template<typename T>
	T* CreateEntity(sf::Vector2f size, const sf::Color& color);

protected:
    Entity() = default;
    ~Entity() = default;

    virtual void OnUpdate() {};
    virtual void OnCollision(Entity* collidedWith) {};
	virtual void OnInitialize() {};
	virtual void OnDestroy() {};
	
private:
    void Update();
	void Initialize(float radius, const sf::Color& color);
	void Initialize(sf::Vector2f size, const sf::Color& color);
	void Repulse(Entity* other);

    friend class GameManager;
    friend Scene;
};

#include "Entity.inl"