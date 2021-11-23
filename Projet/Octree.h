#pragma once

#include "core/Actor.h"

#include "math/BoundingVolume.h"

#include <vector>
#include <memory>

struct Node;

constexpr unsigned int MAX_ACTORS_COUNT = 6;

struct Leaf {
	using ID = std::size_t;

	ID ActorID;
	Point Position;

	bool operator==(const Leaf& lhr) const noexcept { return ActorID == lhr.ActorID; };
};

struct Node {
	using ChildPtr = std::unique_ptr<Node>;

	Node(const BoundingBox& Boundary);

	bool Add(const Leaf& Leaf);
	bool Remove(const Leaf& Leaf);
	void Subdivide();

	bool Subdivided;
	BoundingBox Boundary;
	std::vector<Leaf> Leafs;
	std::vector<ChildPtr> Children;

private:
	bool AddToChildren(const Leaf& Leaf);
	bool RemoveFromChildren(const Leaf& Leaf);
};

class Octree {
public:
	using ActorPtr = Pitbull::Actor*;
	using ActorType = std::unique_ptr<Pitbull::Actor>;
	using ActorList = std::vector<ActorType>;
	using ActorPtrList = std::vector<ActorPtr>;

	Octree(const BoundingBox& Boundary);

	bool Add(ActorType Actor);
	bool Remove(const ActorPtr Actor);
	void Update();

	const ActorList& GetActors() const noexcept;
	ActorPtrList Find(const Point& Pos, float MaxDistance);
	ActorPtrList Find(const BoundingVolume Volume);

private:
	Node Root;
	ActorList Actors;
};
