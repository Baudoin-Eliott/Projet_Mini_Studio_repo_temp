#include "Entity.h"

#include "GameManager.h"
#include "Utils.h"
#include "Debug.h"

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/CircleShape.hpp>

float Velocity::GetSpeed()
{
	if (!_speedIsUpdated)
	{
		_speed = sqrt(_dir.x * _dir.x + _dir.y * _dir.y);
		_speedIsUpdated = true;
	}
	return _speed;
}
void Velocity::SetSpeed(float speed)
{
	_dir /= GetSpeed();
	_speed = speed;
	_dir *= _speed;
	_speedIsUpdated = true;
}
const sf::Vector2f& Velocity::GetDir()
{
	return _dir;
}
sf::Vector2f Velocity::GetCopyDir()
{
	return _dir;
}

void Velocity::SetDir(const sf::Vector2f& dir)
{
	_dir = dir;
	_speedIsUpdated = false;
}
void Velocity::operator+=(const sf::Vector2f& addToDir)
{
	_dir += addToDir;
	_speedIsUpdated = false;
}

static void RepulseRectCircle(Entity* rectEntity, Entity* circleEntity,
	const sf::RectangleShape& rect, const sf::CircleShape& circle,
	float rectForce, float circleForce)
{
	//on trouve le point le plus proche
	sf::Vector2f rectPos = rect.getPosition();
	sf::Vector2f rectSize = rect.getSize();

	sf::Vector2f circleCenter = circle.getPosition() + sf::Vector2f(circle.getRadius(), circle.getRadius());
	float radius = circle.getRadius();

	float closestX = std::max(rectPos.x, std::min(circleCenter.x, rectPos.x + rectSize.x));
	float closestY = std::max(rectPos.y, std::min(circleCenter.y, rectPos.y + rectSize.y));

	//on regarde a quel point on est dans l'autre
	sf::Vector2f diff = circleCenter - sf::Vector2f(closestX, closestY);
	float dist = std::sqrt(diff.x * diff.x + diff.y * diff.y);

	sf::Vector2f normal;
	float penetration;

	if (dist == 0.f)
	{
		// si on est pile au centre
		float dLeft = circleCenter.x - rectPos.x;
		float dRight = rectPos.x + rectSize.x - circleCenter.x;
		float dTop = circleCenter.y - rectPos.y;
		float dBottom = rectPos.y + rectSize.y - circleCenter.y;
		float minD = std::min(std::min(dLeft, dRight), std::min(dTop, dBottom));

		if (minD == dLeft) normal = { -1.f,  0.f };
		else if (minD == dRight) normal = { 1.f,  0.f };
		else if (minD == dTop) normal = { 0.f, -1.f };
		else normal = { 0.f,  1.f };
		penetration = radius + minD;
	}
	else
	{
		normal = diff / dist;
		penetration = radius - dist;
	}

	//on pouse vers l'exterieur
	sf::Vector2f translation = normal * (penetration * 0.5f);

	sf::Vector2f centerRect = rectEntity->GetPosition(0.5f, 0.5f);
	sf::Vector2f centerCircle = circleEntity->GetPosition(0.5f, 0.5f);

	sf::Vector2f position1 = centerRect - translation * rectForce;
	sf::Vector2f position2 = centerCircle + translation * circleForce;



	rectEntity->SetPosition(position1.x, position1.y, 0.5f, 0.5f);
	circleEntity->SetPosition(position2.x, position2.y, 0.5f, 0.5f);
}

static bool RectCircleCollision(const sf::RectangleShape& rect, const sf::CircleShape& circle)
{
	//trouver le point le plus proche du cercle

	sf::Vector2f center = circle.getPosition() + sf::Vector2f(circle.getRadius(), circle.getRadius());
	sf::Vector2f rectPos = rect.getPosition();
	sf::Vector2f rectSize = rect.getSize();

	float closestX = std::max(rectPos.x, std::min(center.x, rectPos.x + rectSize.x));
	float closestY = std::max(rectPos.y, std::min(center.y, rectPos.y + rectSize.y));

	float dx = center.x - closestX;
	float dy = center.y - closestY;
	return (dx * dx + dy * dy) < (circle.getRadius() * circle.getRadius());

}




void Entity::Initialize(float radius, const sf::Color& color)
{
	std::visit([radius, color](auto& shape) {
		if constexpr (std::is_same_v<std::decay_t<decltype(shape)>, sf::CircleShape>) {
			shape.setOrigin(0.f, 0.f);
			shape.setRadius(radius);
			shape.setFillColor(color);
		}
		}, mShape);


	mTarget.isSet = false;

	OnInitialize();
}

void Entity::Initialize(sf::Vector2f size, const sf::Color& color)
{
	mShape = sf::RectangleShape{};
	std::visit([size, color](auto& shape) {
		if constexpr (std::is_same_v<std::decay_t<decltype(shape)>, sf::RectangleShape>) {
			shape.setOrigin(0.f, 0.f);
			shape.setSize(size);
			shape.setFillColor(color);
		}
		}, mShape);


	mTarget.isSet = false;

	OnInitialize();
}

void Entity::Repulse(Entity* other)
{
	std::visit([this, other](auto& shapeA, auto& shapeB) {
		using T = std::decay_t<decltype(shapeA)>;
		using G = std::decay_t<decltype(shapeB)>;

		float shapeAForce = 0.5f;
		float shapeBForce = 0.5f;

		if (this->mIsSatic) {
			shapeAForce = 0.f;
			shapeBForce *= 2;
		}
		if (other->mIsSatic) {
			shapeBForce = 0.f;
			shapeAForce *= 2;
		}

		if constexpr (std::is_same_v<T, sf::CircleShape> && std::is_same_v<G, sf::CircleShape>) {

			//cercle cercle

			sf::Vector2f distance = GetPosition(0.5f, 0.5f) - other->GetPosition(0.5f, 0.5f);

			float sqrLength = (distance.x * distance.x) + (distance.y * distance.y);
			float length = std::sqrt(sqrLength);

			float radius1 = shapeA.getRadius();
			float radius2 = shapeB.getRadius();

			float overlap = (length - (radius1 + radius2)) * 0.5f;

			sf::Vector2f normal = distance / length;

			sf::Vector2f translation = overlap * normal;

			sf::Vector2f position1 = GetPosition(0.5f, 0.5f) - translation * shapeAForce;
			sf::Vector2f position2 = other->GetPosition(0.5f, 0.5f) + translation * shapeBForce;

			SetPosition(position1.x, position1.y, 0.5f, 0.5f);
			other->SetPosition(position2.x, position2.y, 0.5f, 0.5f);


		}


		if constexpr (std::is_same_v<T, sf::RectangleShape> && std::is_same_v<G, sf::RectangleShape>)
		{
			//rect rect
			sf::Vector2f pos1 = shapeA.getPosition();
			sf::Vector2f size1 = shapeA.getSize();
			sf::Vector2f pos2 = shapeB.getPosition();
			sf::Vector2f size2 = shapeB.getSize();

			sf::Vector2f center1 = GetPosition(0.5f, 0.5f);
			sf::Vector2f center2 = other->GetPosition(0.5f, 0.5f);

			float overlapX = std::min(pos1.x + size1.x, pos2.x + size2.x) - std::max(pos1.x, pos2.x);
			float overlapY = std::min(pos1.y + size1.y, pos2.y + size2.y) - std::max(pos1.y, pos2.y);

			sf::Vector2f translation;
			if (overlapX < overlapY)
			{
				float sign = (center1.x < center2.x) ? 1.f : -1.f;
				translation = { overlapX * 0.5f * sign, 0.f };
			}
			else
			{
				float sign = (center1.y < center2.y) ? 1.f : -1.f;
				translation = { 0.f, overlapY * 0.5f * sign };
			}

			sf::Vector2f position1 = center1 - translation * shapeAForce;
			sf::Vector2f position2 = center2 + translation * shapeBForce;


			SetPosition(position1.x, position1.y, 0.5f, 0.5f);
			other->SetPosition(position2.x, position2.y, 0.5f, 0.5f);
		}

		if constexpr (std::is_same_v<T, sf::RectangleShape> && std::is_same_v<G, sf::CircleShape>)
			RepulseRectCircle(this, other, shapeA, shapeB, shapeAForce, shapeBForce);

		if constexpr (std::is_same_v<T, sf::CircleShape> && std::is_same_v<G, sf::RectangleShape>)
			RepulseRectCircle(other, this, shapeB, shapeA, shapeBForce, shapeAForce);
		}, mShape, other->mShape);
}

bool Entity::IsColliding(Entity* other) const
{

	return std::visit([other, this](const auto& shapeA, const auto& shapeB) -> bool {
		using T = std::decay_t<decltype(shapeA)>;
		using G = std::decay_t<decltype(shapeB)>;

		if constexpr (std::is_same_v<T, sf::CircleShape> && std::is_same_v<G, sf::CircleShape>) {

			//cercle cercle
			sf::Vector2f distance = GetPosition(0.5f, 0.5f) - other->GetPosition(0.5f, 0.5f);

			float sqrLength = (distance.x * distance.x) + (distance.y * distance.y);

			float radius1 = GetRadius();
			float radius2 = other->GetRadius();

			float sqrRadius = (radius1 + radius2) * (radius1 + radius2);

			return sqrLength < sqrRadius;
		}

		if constexpr (std::is_same_v<T, sf::RectangleShape> && std::is_same_v<G, sf::RectangleShape>) {

			//rect rect
			sf::Vector2f size1 = shapeA.getSize();
			sf::Vector2f pos1 = shapeA.getPosition();


			sf::Vector2f size2 = shapeB.getSize();
			sf::Vector2f pos2 = shapeB.getPosition();


			return pos1.x < pos2.x + size2.x &&
				pos2.x < pos1.x + size1.x &&
				pos1.y < pos2.y + size2.y &&
				pos2.y < pos1.y + size1.y;
		}
		if constexpr (std::is_same_v<T, sf::RectangleShape> && std::is_same_v<G, sf::CircleShape>)
			return RectCircleCollision(shapeA, shapeB);
		if constexpr (std::is_same_v<T, sf::CircleShape> && std::is_same_v<G, sf::RectangleShape>)
			return RectCircleCollision(shapeB, shapeA);


		}, mShape, other->mShape);
}

bool Entity::IsInside(float x, float y) const
{
	return std::visit([x, y, this](auto& shape) -> bool {
		using T = std::decay_t<decltype(shape)>;

		if constexpr (std::is_same_v<T, sf::CircleShape>) {
			sf::Vector2f position = GetPosition(0.5f, 0.5f);

			float dx = x - position.x;
			float dy = y - position.y;

			float radius = GetRadius();

			return (dx * dx + dy * dy) < (radius * radius);
		}
		else {
			sf::Vector2f size = shape.getSize();
			sf::Vector2f position = shape.getPosition();

			return x > position.x && y > position.y && x < position.x + size.x && y < position.y + size.y;
		}
		}, mShape);
}

void Entity::Destroy()
{
	mToDestroy = true;

	OnDestroy();
}

void Entity::SetPosition(float x, float y, float ratioX, float ratioY)
{
	sf::Vector2f size = std::visit([](auto& shape) -> sf::Vector2f {
		using T = std::decay_t<decltype(shape)>;

		if constexpr (std::is_same_v<T, sf::CircleShape>) {
			float size = shape.getRadius() * 2;
			return { size, size };
		}
		else
			return shape.getSize();
		}, mShape);

	x -= size.x * ratioX;
	y -= size.y * ratioY;

	std::visit([x, y](auto& shape) {shape.setPosition(x, y);}, mShape);
	mSprite.setPosition(x, y);

	//#TODO Optimise
	if (mTarget.isSet)
	{
		sf::Vector2f position = GetPosition(0.5f, 0.5f);
		mTarget.distance = Utils::GetDistance(position.x, position.y, mTarget.position.x, mTarget.position.y);
		GoToDirection(mTarget.position.x, mTarget.position.y);
		mTarget.isSet = true;
	}
}

sf::Vector2f Entity::GetPosition(float ratioX, float ratioY) const
{
	return std::visit([ratioX, ratioY, this](auto& shape) -> sf::Vector2f {

		using T = std::decay_t<decltype(shape)>;

		if constexpr (std::is_same_v<T, sf::CircleShape>) {
			float size = shape.getRadius() * 2;
			sf::Vector2f position = shape.getPosition();

			position.x += size * ratioX;
			position.y += size * ratioY;

			return position;
		}
		else {
			sf::Vector2f size = shape.getSize();
			sf::Vector2f position = shape.getPosition();

			position.x += size.x * ratioX;
			position.y += size.y * ratioY;

			return position;

		}
		}, mShape);
}

bool Entity::GoToDirection(int x, int y, float speed)
{
	sf::Vector2f position = GetPosition(0.5f, 0.5f);
	sf::Vector2f direction = sf::Vector2f(x - position.x, y - position.y);

	bool success = Utils::Normalize(direction);
	if (success == false)
		return false;

	SetDirection(direction.x, direction.y, speed);

	return true;
}

bool Entity::GoToPosition(int x, int y, float speed)
{
	if (GoToDirection(x, y, speed) == false)
		return false;

	sf::Vector2f position = GetPosition(0.5f, 0.5f);

	mTarget.position = { x, y };
	mTarget.distance = Utils::GetDistance(position.x, position.y, x, y);
	mTarget.isSet = true;

	return true;
}

void Entity::SetDirection(float x, float y, float speed)
{

	_velocity.SetDir({ x,y });
	mTarget.isSet = false;
	if (speed > 0)
		_velocity.SetSpeed(speed);
}

float Entity::GetRadius() const
{
	return std::visit([](auto& shape) -> float
		{
			using T = std::decay_t<decltype(shape)>;
			if constexpr (std::is_same_v < T, sf::CircleShape>)
				return shape.getRadius();
			else
				return 0.f;
		}, mShape);
}

void Entity::Update()
{
	if (mIsSatic)
		return;
	// render
	OnUpdate();
}

void Entity::FixedUpdate()
{
	if (mIsSatic)
		return;

	float dt = GetDeltaTime();
	float distance = dt * _velocity.GetSpeed();
	sf::Vector2f translation = _velocity.GetDir();
	std::visit([translation](auto& shape) {
		shape.move(translation);
		}, mShape);

	mSprite.setPosition(GetPosition());



	OnFixedUpdate();
}
Scene* Entity::GetScene() const
{
	return GameManager::Get()->GetScene();
}

float Entity::GetDeltaTime() const
{
	return GameManager::Get()->GetDeltaTime();
}