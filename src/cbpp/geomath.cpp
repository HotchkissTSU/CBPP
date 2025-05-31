#include "cbpp/geomath.h"

#include <math.h>

namespace cbpp::math {
    Intersection g_IsecBuffer;

    bool Equal(float_t A, float_t B) noexcept {
        return (float_t)fabs(A - B) < CBPP_MATH_EPSILON;
    }

    bool Equal(Vec2 A, Vec2 B) noexcept {
        float_t fDist = SquareDistance(A,B);
        return (fDist < CBPP_MATH_EPSILON*CBPP_MATH_EPSILON);
    }

    float_t Rad(float_t fDeg) noexcept {
        return (fDeg * CBPP_MATH_2PI) / 360.0f;
    }

    float_t Deg(float_t fRad) noexcept {
        return fRad * 180.0f / CBPP_MATH_PI;
    }

    float_t Distance(Vec2 A, Vec2 B) noexcept {
        return sqrt( (A.x*A.x) + (B.x*B.x) );
    }

    float_t Distance(Vec2 A, Line& B) noexcept {
        return (B.fA*A.x + B.fB*A.y + B.fC) / sqrt(B.fA*B.fA + B.fB*B.fB);
    }

    float_t SquareDistance(Vec2 A, Vec2 B) noexcept {
        return (A.x*A.x) + (B.x*B.x);
    }

    Vec2 ClosestPoint(Vec2 A, Line& B) noexcept {
        float_t fX0 = (B.fB * (B.fB*A.x - B.fA*A.y) - B.fA*B.fC) / (B.fA*B.fA + B.fB*B.fB);
        float_t fY0 = (B.fA * (B.fA*A.y - B.fB*A.x) - B.fB*B.fC) / (B.fA*B.fA + B.fB*B.fB);

        return Vec2(fX0, fY0);
    }

    Vec2 Clamp(Vec2 Value, Vec2 MinValue, Vec2 MaxValue) noexcept {
        return Vec2(Clamp(Value.x, MinValue.x, MaxValue.x),
                    Clamp(Value.y, MinValue.y, MaxValue.y));
    }

    bool InRange(Vec2 Value, Vec2 MinValue, Vec2 MaxValue) noexcept {
        return InRange(Value.x, MinValue.x, MaxValue.x) &&
               InRange(Value.y, MinValue.y, MaxValue.y);
    }
    
    bool InRangeStrict(Vec2 Value, Vec2 MinValue, Vec2 MaxValue) noexcept {
        return InRangeStrict(Value.x, MinValue.x, MaxValue.x) &&
               InRangeStrict(Value.y, MinValue.y, MaxValue.y);
    }

    size_t PlanarToLinear(size_t iSideLength, size_t iX, size_t iY) noexcept {
        return iSideLength * iY + iX;
    }
    
    void LinearToPlanar(size_t iSideLength, size_t iLinear, size_t& iX, size_t& iY) noexcept {
        iX = iLinear % iSideLength;
        iY = iLinear / iSideLength;
    }

    //Line
    Line::Line(Vec2 vp1, Vec2 vp2) {
        vDir = vp2 - vp1;
        vPos1 = vp1; vPos2 = vp2;

        Vec2 vLineNormal(-vDir.y, vDir.x); //(x,y) -> (-y,x) gives us a 90-degree rotation

        fA = vLineNormal.x;
        fB = vLineNormal.y;
        fC = vp1.y*vp2.x - vp1.x*vp2.y;
    }

    Line& LINE() noexcept {
        static Line s_mpLineBuffer;
        return s_mpLineBuffer;
    }

    Circle& CIRCLE() noexcept {
        static Circle s_mpCircleBuffer;
        return s_mpCircleBuffer;
    }
}

namespace cbpp::math {
    Intersection* GetLastIntersection() noexcept {
        return &g_IsecBuffer;
    }

    bool Intersect(Intersection* pTarget, Line& A, Line& B) {
        if(pTarget == NULL) {
            pTarget = &g_IsecBuffer;
        }

        float_t fCrossp = A.vDir.Cross(B.vDir);
        if(Equal(0.0f, fCrossp)) { //The lines are probably parallel
            pTarget->bResult = false;
            return false;
        }

        float_t fIsecY = (B.fA * A.fC - A.fA * B.fC) / (A.fA * B.fB - B.fA * A.fB);
        float_t fIsecX = -(A.fB*fIsecY + A.fC) / (A.fA);

        pTarget->bResult = true;
        pTarget->iCollPointsNum = 1;
        pTarget->aPoints[0] = Vec2(fIsecX, fIsecY);

        pTarget->fDistance = 0.0f;
        pTarget->vAvgCollidePos = pTarget->aPoints[0];

        return true;
    }

    bool Intersect(Intersection* pTarget, Line& A, Circle& B) {
        if(pTarget == NULL) {
            pTarget = &g_IsecBuffer;
        }

        Line mpLineLocal(A.vPos1 - B.vPos, A.vPos2 - B.vPos);

        float_t a = mpLineLocal.fA, b = mpLineLocal.fB, c = mpLineLocal.fC;
		float_t m = B.fRadius;
		
		float_t d = a*a*( m*m * (b*b + a*a) - c*c );
		
		if(d < 0){ //No solutions, so no intersection points
			pTarget->bResult = false;
            return false;
			
		}else if(d > 0){ //Two points
			float_t root = sqrt(d);
			float_t down = a*a + b*b;
		
			float_t y1 = (-b*c+root)/down;
			float_t x1 = -( (b*y1 + c)/a );
			
			float_t y2 = (-b*c-root)/down;
			float_t x2 = -( (b*y2 + c)/a );
			
			pTarget->bResult = true;
            pTarget->iCollPointsNum = 2;
            pTarget->fDistance = Distance(Vec2(), A);

            pTarget->aPoints[0] = Vec2(x1,y1) + B.vPos;
			pTarget->aPoints[1] = Vec2(x2,y2) + B.vPos;

            pTarget->vAvgCollidePos = (pTarget->aPoints[0] + pTarget->aPoints[1]) / 2;

			return true;
			
		}else{ //One point
			float_t y = -( (b*c)/(a*a + b*b) );
			float_t x = -( (b*y + c)/a );
			
			pTarget->bResult = true;
            pTarget->iCollPointsNum = 1;
            pTarget->fDistance = B.fRadius;

			pTarget->aPoints[0] = Vec2(x,y) + B.vPos;

            pTarget->vAvgCollidePos = pTarget->aPoints[0];
			
			return true;
		}
    }

    bool Intersect(Intersection* pTarget, Circle& A, Circle& B_) {
        if(pTarget == NULL) {
            pTarget = &g_IsecBuffer;
        }

        //We assume that the first circle lays at the coordinates origin to simplify the math
        Circle B(B_.vPos - A.vPos, B_.fRadius); 

        float_t x0 = B.vPos.x, y0 = B.vPos.y;
		float_t m = A.fRadius, k = B.fRadius;

		if(Equal(x0, 0.0f) && Equal(y0, 0.0f) && Equal(m, k)){
            pTarget->bResult = true;
            pTarget->fDistance = 0.0f;
            pTarget->iCollPointsNum = 2;
            pTarget->vAvgCollidePos = A.vPos;

            pTarget->aPoints[0] = A.vPos + Vec2(0.0f,-k);
            pTarget->aPoints[1] = A.vPos + Vec2(0.0f, k);
			
			return true;
		}
		
		if(Equal(x0, 0.0f)){
			x0 = CBPP_MATH_EPSILON;
		}
		
		float_t p = -((k*k - m*m - x0*x0 - y0*y0)/2.0f);
		float_t summ = x0*x0 + y0*y0;
		
		float_t d_root = sqrt( m*m*summ - p*p );
		float_t kf = fabs(2*x0);
		
		float_t d = kf*d_root;
		
		if(d > 0){
			float_t y1 = (p*y0 + x0*d_root)/summ;
			float_t y2 = (p*y0 - x0*d_root)/summ;
			
			float_t x1 = (p - y1*y0)/x0;
			float_t x2 = (p - y2*y0)/x0;

            pTarget->bResult = true;
            pTarget->fDistance = B.vPos.Length();
            pTarget->iCollPointsNum = 2;

            pTarget->aPoints[0] = Vec2(x1,y1);
            pTarget->aPoints[1] = Vec2(x2,y2);

            pTarget->vAvgCollidePos = (pTarget->aPoints[0] + pTarget->aPoints[1]) / 2;
            return true;
			
		}else if(Equal(d, 0.0f)){
			float_t y = (p*y0)/summ;
			float_t x = (p-y*y0)/x0;

            pTarget->bResult = true;
            pTarget->fDistance = B.fRadius;
            pTarget->iCollPointsNum = 1;

            pTarget->aPoints[0] = Vec2(x,y);

            pTarget->vAvgCollidePos = pTarget->aPoints[0];
			
			return true;
		}
		
		pTarget->bResult = false;
        return false;
    }
}
