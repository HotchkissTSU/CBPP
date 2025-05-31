#ifndef CBPP_GEOMATH_H
#define CBPP_GEOMATH_H

/*
    Engine`s new math library
*/

#include <stddef.h>

#include "cbpp/vec2.h"

#define CBPP_MATH_EPSILON ((cbpp::float_t)(1e-5f))

#define CBPP_MATH_PI ((cbpp::float_t)(3.1415926535f))
#define CBPP_MATH_2PI ((cbpp::float_t)(2.0f) * CBPP_MATH_PI)

namespace cbpp {
    namespace math {
        bool Equal(float_t A, float_t B) noexcept;
        bool Equal(Vec2 A, Vec2 B) noexcept;

        //Degrees -> Radians
        float_t Rad(float_t fDeg) noexcept; 

        //Radians -> Degrees
        float_t Deg(float_t fRad) noexcept; 

        // A... circle! The round one!
        struct Circle {
            Circle() : fRadius(0.0f) {};
            Circle(Vec2 pos, float_t rad) : fRadius(rad), vPos(pos) {};

            float_t fRadius;
            Vec2 vPos;
        };

        // An infinite straight line
        struct Line {
            Line() : fA(0.0f), fB(0.0f), fC(0.0f) {};
            Line(Vec2 vPosA, Vec2 vPosB);
            Vec2 vDir, vPos1, vPos2;
            float_t fA, fB, fC; //Ax + By + C = 0

            //Normalize the A and B coeffs of the equasion
            void Normalize() noexcept;
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
            float_t fDistance = 0.0f;       //A distance between the primitives
            Vec2 vAvgCollidePos;            //Average intersection point
            char iCollPointsNum = 0;        //Count of the intersections (0,1 or 2)
            Vec2 aPoints[2];                //Intersection points
        };

        //Obtain a reference to the internal reusable line
        Line& LINE() noexcept;

        //Obtain a reference to the internal reusable circle
        Circle& CIRCLE() noexcept;

        /*
            If NULL is passed to these functions as pTarget, then the result
            is written in the internal buffer and can be obtained with GetLastIntersection().

            The data in this pointer is valid until any next Intersect() call.
            If bResult is false then you should not trust any other data in the structure.
        */

        bool Intersect(Intersection* pTarget, Line& A, Line& B);
        bool Intersect(Intersection* pTarget, Line& A, Circle& B);
        bool Intersect(Intersection* pTarget, Circle& A, Circle& B);

        Intersection* GetLastIntersection() noexcept;

        float_t Distance(Vec2 A, Vec2 B) noexcept;
        float_t Distance(Vec2 A, Line& B) noexcept;

        float_t SquareDistance(Vec2 A, Vec2 B) noexcept;

        Vec2 ClosestPoint(Vec2 A, Line& B) noexcept;
        Vec2 ClosestPoint(Line& A, Circle& B) noexcept;

        extern Intersection g_IsecBuffer;

        template<typename INT_T> bool IsPOT(INT_T iValue) noexcept {
            return (iValue > 0) && ((iValue & (iValue - 1)) == 0);
        }

        //Lock value between two borders
        template<typename NUM_T> NUM_T Clamp(NUM_T Value, NUM_T MinValue, NUM_T MaxValue) noexcept {
            const NUM_T Temp = Value < MinValue ? MinValue : Value;
            return Temp > MaxValue ? MaxValue : Temp;
        }

        //MIN <= X <= MAX
        template<typename NUM_T> NUM_T InRange(NUM_T Value, NUM_T MinValue, NUM_T MaxValue) noexcept {
            return (Value >= MinValue) && (Value <= MaxValue);
        }

        //MIN < X < MAX
        template<typename NUM_T> NUM_T InRangeStrict(NUM_T Value, NUM_T MinValue, NUM_T MaxValue) noexcept {
            return (Value > MinValue) && (Value < MaxValue);
        }

        bool InRange(Vec2 Value, Vec2 MinValue, Vec2 MaxValue) noexcept;
        bool InRangeStrict(Vec2 Value, Vec2 MinValue, Vec2 MaxValue) noexcept;

        Vec2 Clamp(Vec2 Value, Vec2 MinValue, Vec2 MaxValue) noexcept;

        size_t PlanarToLinear(size_t iSideLength, size_t iX, size_t iY) noexcept;
        void LinearToPlanar(size_t iSideLength, size_t iLinear, size_t& iX, size_t& iY) noexcept;
    }
}

#endif
