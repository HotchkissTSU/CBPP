#ifndef CBPP_MATH_H
#define CBPP_MATH_H

#define CBPP_MATH_EPSILON 0.005f

#include "cbpp/vec2.h"
#include <vector>
#include <cmath>

namespace cbpp{	
	bool fEqual(float a, float b);
	
	float Clamp(float x, float min, float max);
	int Clamp(int x, int min, int max);
	
	extern "C" struct mLine {
		float a,b,c;
		Vec2 normal, p1, p2;
	};
	
	extern "C" struct mSegment {
		Vec2 start, end;
		Vec2 direction;
		float length;
	};
	
	extern "C" struct mCircle {
		float radius;
		Vec2 pos;
	};
	
	extern "C" struct Intersection {
		std::vector<Vec2> points;
		bool intersect = false;
	};
	
	mLine GetLine(Vec2 p1, Vec2 p2);
	mSegment GetSegment(Vec2 p1, Vec2 p2);
	
	Intersection Intersect_LineLine(mLine a, mLine b);
	Intersection Intersect_LineCircle(mLine a, mCircle circ);
	Intersection Intersect_LineSegment(mLine a, mSegment b);
	
	Intersection Intersect_SegmentSegment(mSegment a, mSegment b);
	Intersection Intersect_SegmentCircle(mSegment a, mCircle b);
	
	Intersection Intersect_CircleCircle(mCircle a, mCircle b);
	
	bool PointOnLine(mLine line, Vec2 pt);
	bool PointOnSegment(mSegment seg, Vec2 pt); //ВАЖНО - подразумевается, что точка уже лежит на прямой, образующей отрезок
	
	float LineSubstitute(mLine line, Vec2 pt);	//подстановка точки в уравнение Ax+By+C = 0
	float LinePointDistance(mLine line, Vec2 pt);
}

#endif