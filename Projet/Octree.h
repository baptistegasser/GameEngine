#pragma once

#include "math/BoundingBox.h"

#include <algorithm>
#include <vector>

template <class T, int Capacity>
class Octree;

template <class T, int Capacity>
class Octree {
public:
	Octree(const BoundingBox& Boundary);
	~Octree() noexcept;
	bool Insert(const Point& Pos, const T& Element) noexcept;
	std::vector<T> RequestValues() const;
	std::vector<T> RequestValues(const BoundingVolume* Volume) const;

private:
	/// <summary>
	/// Class representing a point inside the octree, essentially a 3D point with some data attached
	/// </summary>
	struct OctreePoint : public Point { const T& Data; };

	BoundingBox Boundary;
	Octree<T, Capacity>* SubTrees;
	std::vector<OctreePoint> Points;
	bool Subdivided;

	void Subdivide() noexcept;
	bool Insert(const OctreePoint& Point) noexcept;
};

template <class T, int Capacity>
Octree<T, Capacity>::Octree(const BoundingBox& Boundary)
	: Boundary{ Boundary }
	, SubTrees{ nullptr }
	, Points{ }
	, Subdivided{ false }
{}

template <class T, int Capacity>
Octree<T, Capacity>::~Octree() noexcept
{
	if (Subdivided) {
		delete[] SubTrees;
	}
}

template <class T, int Capacity>
bool Octree<T, Capacity>::Insert(const Point& Pos, const T& Element) noexcept
{
	OctreePoint Point{ Pos };
	Point.Data = Element;
	return Insert(Point);
}

template <class T, int Capacity>
std::vector<T> Octree<T, Capacity>::RequestValues() const
{
	std::vector<T> Values;

	if (!Subdivided) {
		Values.reserve(Points.size());
		for (const auto& Point : Points) {
			Values.push_back(Point.Data);
		}
	}
	else {
		const auto GetSubTreesPoints = [&](const auto& Tree) {
			const auto Val = Tree.RequestValues();
			std::copy(Val.begin(), Val.end(), std::back_inserter(Values));
		};
		std::for_each_n(SubTrees, 8, GetSubTreesPoints);
	}

	return Values;
}

template <class T, int Capacity>
std::vector<T> Octree<T, Capacity>::RequestValues(const BoundingVolume* Volume) const
{
	std::vector<T> Values;

	if (!Subdivided) {
		Values.reserve(Points.size());
		for (const auto& Point : Points) {
			if (Volume->ContainPoint(Point))
				Values.push_back(Point.Data);
		}
	}
	else {
		const auto GetSubTreesPoints = [&](const auto& Tree) {
			const auto Val = Tree.RequestValues(Volume);
			std::copy(Val.begin(), Val.end(), std::back_inserter(Values));
		};
		std::for_each_n(SubTrees, 8, GetSubTreesPoints);
	}

	return Values;
}

template <class T, int Capacity>
void Octree<T, Capacity>::Subdivide() noexcept
{
	// Already subdivided
	if (SubTrees) return;

	const float HW  = Boundary.HalfWidth  / 2.f;
	const float HH  = Boundary.HalfHeight / 2.f;
	const float HD  = Boundary.HalfDepth  / 2.f;

	const Point Center = Boundary.Center;

	SubTrees = new Octree<T, Capacity>[8];

	SubTrees[0] = new Octree<T, Capacity>{ BoundingBox{ HW, HH, HD, Point{ Center.x+HW, Center.y+HH, Center.z+HD } } };
	SubTrees[1] = new Octree<T, Capacity>{ BoundingBox{ HW, HH, HD, Point{ Center.x+HW, Center.y+HH, Center.z-HD } } };
	SubTrees[2] = new Octree<T, Capacity>{ BoundingBox{ HW, HH, HD, Point{ Center.x+HW, Center.y-HH, Center.z+HD } } };
	SubTrees[3] = new Octree<T, Capacity>{ BoundingBox{ HW, HH, HD, Point{ Center.x+HW, Center.y-HH, Center.z-HD } } };
	SubTrees[4] = new Octree<T, Capacity>{ BoundingBox{ HW, HH, HD, Point{ Center.x-HW, Center.y+HH, Center.z+HD } } };
	SubTrees[5] = new Octree<T, Capacity>{ BoundingBox{ HW, HH, HD, Point{ Center.x-HW, Center.y+HH, Center.z-HD } } };
	SubTrees[6] = new Octree<T, Capacity>{ BoundingBox{ HW, HH, HD, Point{ Center.x-HW, Center.y-HH, Center.z+HD } } };
	SubTrees[7] = new Octree<T, Capacity>{ BoundingBox{ HW, HH, HD, Point{ Center.x-HW, Center.y-HH, Center.z-HD } } };

	for (const auto& Point : Points) {
		for (int i = 0; i < 8; ++i) {
			if (SubTrees[i].Insert(Point)) break;
		}
	}

	Subdivided = true;
}

template <class T, int Capacity>
bool Octree<T, Capacity>::Insert(const OctreePoint& Point) noexcept
{
	// Don't insert out of bound
	if (!Boundary.ContainPoint(Point)) return false;

	// If enough place insert
	if (Points.size() < Capacity && !Subdivided) {
		Points.push_back(Point);
		return true;
	}

	if (!Subdivided) Subdivide();

	for (int i = 0; i < 8; ++i) {
		if (SubTrees[i].Insert(Point)) return true;
	}

	return false; // Should not happen :)
}
