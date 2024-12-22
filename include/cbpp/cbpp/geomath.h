#ifndef CBPP_GEOMATH_H
#define CBPP_GEOMATH_H

/*
    Some geometry calculations
*/

#include <stddef.h>

#include "cbpp/vec2.h"

#define CBPP_MATH_EPSILON (float_t)(1e-5f)

#define CBPP_MATH_PI (float_t)(3.1415926535f)
#define CBPP_MATH_2PI (float_t)(2.0f) * CBPP_MATH_PI

/*
    We have an internal buffer to store the intersection points.
    This is NOT thread-safe, but the engine is single-threaded.
*/
#define CBPP_MATH_ISEC_POINTS_BUFFER_SIZE 16

namespace cbpp {
    namespace math {
        bool Equal(float_t A, float_t B) noexcept;
        bool Equal(Vec2 A, Vec2 B) noexcept;

        // A... circle! The round one!
        struct Circle {
            Circle(Vec2 pos, float_t rad) : fRadius(rad), vPos(pos) {};

            float_t fRadius;
            Vec2 vPos;
        };

        // An infinite straight line
        struct Line {
            Line(Vec2 vPosA, Vec2 vPosB);
            Vec2 vDir, vPos1, vPos2;
            float_t fA, fB, fC; //Ax + By + C = 0
        };

        // A line with a direction and a fixed start point
        struct Ray {
            Ray(Vec2 vStartPos, Vec2 vDirection) : vStart(vStartPos), vDir(vDirection), mpLine(vStartPos, vStartPos + vDirection*2) {};

            Vec2 vStart, vDir;
            Line mpLine;
        };

        // A line with fixed start and end points
        struct Segment {

        };

        struct Intersection {
            bool bResult = false;
            float_t fDistance = 0.0f; //A distance between the primitives

            Vec2 vAvgCollidePos;

            size_t iCollPointsNum = 0;
        };

        /*
            If NULL is passed to these functions as pTarget, then the result
            is written in an internal buffer and can be obtained with GetLastIntersection().

            The data in this pointer is valid until any next Intersect() call.
            The intersection points array is buffered too and is also valid until
            next calls. It can be obtained with GetIntersectionPoints().

            The return value is a "Intersects (in any way) or not" flag.
        */
        bool Intersect(Intersection* pTarget, Line& A, Line& B);
        bool Intersect(Intersection* pTarget, Line& A, Circle& B);
        bool Intersect(Intersection* pTarget, Circle& A, Circle& B);

        Vec2* GetIntersectionPoints() noexcept;
        Intersection* GetLastIntersection() noexcept;

        float_t Distance(Vec2 A, Vec2 B) noexcept;
        float_t Distance(Vec2 A, Line& B) noexcept;

        float_t SquareDistance(Vec2 A, Vec2 B) noexcept;

        Vec2 ClosestPoint(Vec2 A, Line& B) noexcept;
        Vec2 ClosestPoint(Line& A, Circle& B) noexcept;

        extern Intersection g_IsecBuffer;
        extern Vec2 g_IsecPtsBuffer[];

        template<typename NUM_T> NUM_T Clamp(NUM_T Value, NUM_T MinValue, NUM_T MaxValue) {
            if(Value > MaxValue) { return MaxValue; }
            if(Value < MinValue) { return MinValue; }
            return Value;
        }
    }
}

#endif