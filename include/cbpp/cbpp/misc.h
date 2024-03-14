#ifndef CBPP_MISC
#define CBPP_MISC

#include <time.h>
#include <cbpp/vec2.h>
#include <cmath>
#include <vector>

namespace cbpp{
	extern "C" float Clampf(float a, float mx, float mn);
	extern "C" bool VecInBox(Vec2 v, Vec2 p1, Vec2 p2);
	extern "C" Vec2 ClampVectorV(Vec2 v, Vec2 min, Vec2 max);
	
	extern "C" struct Lerp{
		Lerp(){}
		Lerp(float s, float e, float spd);
		
		void Tick();
		float Get();
		
		float cur_value, target, speed;
		time_t last_tick;
	};
	
	extern "C" struct Timer{
		Timer(){};
		Timer(float delay);
		
		void Tick();
		float Elapsed();
		float Left();
		bool Status();
		
		time_t start, end, curtime, last_tick, cur_value;
		float delay;
	};
	
	extern "C" struct Accumulator{
		Accumulator(){};
		
		void Tick();
		float Get();
		void SetSpeed(float aspd);
		
		time_t last_tick, curtime, tick_time;
		float accel_speed, cur_value;
	};
	
	extern "C" struct LineIntersectionResult{
		bool intersect = false;
		Vec2 intersect_pos;
	};
	
	extern "C" float* GetLineKoeffs(Vec2 pos1, Vec2 pos2);  //<- возвращает массив [k, b] с коэффицентами прямой (для y = kx + b)
	extern "C" float* GetLineKoeffs2(Vec2 pos1, Vec2 pos2); //<- аналогично, но [a,b,c] для Ax + By + C = 0
															//массивы из этих функций надо удалять самостоятельно
	
	extern "C" float GetLinePointOrientation(Vec2 lx, Vec2 ly, Vec2 p); //определяет, по какую сторону от прямой лежит точка
	
	extern "C" LineIntersectionResult GetLineIntersection(Vec2 l1_1, Vec2 l1_2, Vec2 l2_1, Vec2 l2_2); //считает пересечение отрезка с прямой
																									   //аргументы - сначала прямая, потом отрезок
																									   
	std::vector<std::wstring> SplitString(std::wstring str, wchar_t delim);
	std::vector<std::string> SplitString(std::string str, char delim);
}

#endif