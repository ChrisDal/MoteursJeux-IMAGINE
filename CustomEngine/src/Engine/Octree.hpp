#pragma once
#include <iostream>
#include <vector>
#include <cmath>
#include <cfloat>
#include <string>

struct Point3D
{
	float x;
	float y;
	float z;

	Point3D(float _x, float _y, float _z)
		: x(_x), y(_y), z(_z)
	{};

	Point3D()
		: x(0.0f), y(0.0f), z(0.0f)
	{}

	Point3D(const Point3D& p)
		: x(p.x), y(p.y), z(p.z)
	{}

	Point3D operator+(const Point3D& b)
	{
		Point3D res;
		res.x = this->x + b.x;
		res.y = this->y + b.y;
		res.z = this->z + b.z;

		return res;
	}

	Point3D operator-(const Point3D& b)
	{
		Point3D res;
		res.x = this->x - b.x;
		res.y = this->y - b.y;
		res.z = this->z - b.z;

		return res;
	}


	friend Point3D operator + (Point3D const& a, Point3D const& b)
	{
		return Point3D(a.x + b.x, a.y + b.y, a.z + b.z);

	}

	friend Point3D operator - (Point3D const& a, Point3D const& b)
	{
		return Point3D(a.x - b.x, a.y - b.y, a.z - b.z);
	}

	friend Point3D operator / (Point3D const& a, float k)
	{
		return Point3D(a.x / k, a.y / k, a.z / k);
	}

	template <typename T>
	friend Point3D operator * (Point3D const& a, T k)
	{
		return Point3D(a.x * (float)k, a.y * (float)k, a.z * (float)k);
	}

	friend std::ostream& operator<<(std::ostream& stream, const Point3D& p)
	{
		stream << '(' << p.x << ", " << p.y << ", " << p.z << ')';

		return stream;
	}
};



class Octree
{
private:

	std::vector<Octree*> m_children;
	Octree* m_parent = nullptr;
	Point3D* m_point = nullptr;  // at most 1 or 0 points 

	// bounding box of our cube 
	Point3D* limit_a = nullptr; // bottom near left
	Point3D* limit_b = nullptr; // top far right

	// Tag
	short int m_tag = -1;
	int m_depth = -1;
	std::string m_stag;

	bool m_dirty = false;


protected:

	void splitSpace(const Point3D& pa, const Point3D& pb);
	Point3D getMid(const Point3D& pa, const Point3D& pb);

	void setTagString();
	void setParent(Octree* parent);

public:

	static int depth;

	Octree(const Point3D& bottomnearleft, const Point3D& topfarright, short int tag = -1);
	Octree(float xa, float ya, float za, float xb, float yb, float zb, short int tag = -1);
	Octree(float x, float y, float z, short int tag = -1);
	Octree() { m_dirty = true; }

	~Octree();

	// find if a point is already in the Octree
	bool find(const Point3D& p);
	bool find(float xi, float yi, float zi);

	// find the closest point in octree
	Point3D* findclosest(const Point3D& p) const;
	Point3D* findclosestPoint(const Point3D& p, float& distance, Octree* minOct);
	Octree* findclosestOctree(const Point3D& p);

	// insert point in octree
	void insert(const Point3D& p);
	void insert(float xi, float yi, float zi);

	// Getters and setters
	void setMpoint(Point3D* p) { this->m_point = p; }
	Point3D* getMpoint() { return m_point; }

	std::vector<Octree*> getSiblings() const;

	short int getTag() { return m_tag; }
	std::string getStag() const { return m_stag; }

	int getDepth() const { return m_depth; }
	std::vector<Octree*> getChildren() const { return m_children; }

	bool isRegionEmpty() const;

	// get limits point A B 
	Point3D* getLimit(bool isA);
	bool insideBounds(const Point3D& p) const;

	// Calcul distance with 
	float getDistance(const Point3D& p) const;
	void getMinOctreeDist(std::vector<Octree*> siblings, float& distance,
		Octree* minOct, const Point3D& pos);

	void invalidate();
	void setDirty(bool isDirty) { m_dirty = isDirty; }
};


