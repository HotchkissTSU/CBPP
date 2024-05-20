#include "cbpp/misc.h"
#include <iostream>
#include <iomanip>

namespace cbpp{
	uint32_t GetNumLength(int64_t val){
		uint32_t out = 0;
		
		while(val){
			val = val/10;
			out++;
		}
		
		return out;
	}
	
	std::string IntToString(int64_t val){
		uint32_t numlen = GetNumLength(val);
		char* buff = new char[numlen+1];
		memset(buff, 0, numlen+1);
		
		itoa(val, buff, 10);
		
		std::string out(buff);
		delete[] buff;
		
		return out;
	}
	
	int64_t StringToInt(std::string val){
		return atoi(val.c_str());
	}
	
	float Clampf(float a, float mx, float mn){
		if(a > mx){
			return mx;
		}else if(a < mn){
			return mn;
		}else{
			return a;
		}
	}
	
	bool VecInBox(Vec2 v, Vec2 p1, Vec2 p2){
		return (v.x >= p1.x && v.x <= p2.x && v.y >= p1.y && v.y <= p2.y);
	}
	
	Vec2 ClampVectorV(Vec2 v, Vec2 min, Vec2 max){
		return Vec2(Clampf(v.x, min.x, max.x), Clampf(v.y, min.y, max.y));
	}
	
	//linear interpolation
	
	Lerp::Lerp(float s, float t, float spd){
		cur_value = s;
		target = t;
		speed = spd;
	}
	
	float Lerp::Get(){ return cur_value; }
	
	void Lerp::Tick(){
		time_t curtime;
		time(&curtime);
		
		float tick_time = curtime - last_tick;
		
		time(&last_tick);
		
		float spd = speed*tick_time;
		
		cur_value = Clampf(target - cur_value, -spd, spd);
	}
	
	//timer
	Timer::Timer(float d){
		delay = std::abs(d);
		
		time(&curtime);
		start = curtime;
		end = curtime + delay;
	}
	
	void Timer::Tick(){
		time(&curtime);
	}
	
	float Timer::Elapsed(){ return curtime - start; }
	float Timer::Left(){ return end - curtime; }
	bool Timer::Status(){ return curtime >= end; }
	
	//accumulator
	
	void Accumulator::SetSpeed(float aspd){ accel_speed = aspd; }
	float Accumulator::Get(){ return cur_value; }
	void Accumulator::Tick(){
		time(&curtime);
		tick_time = curtime - last_tick;
		time(&last_tick);
		
		cur_value = cur_value + accel_speed*tick_time;
	}
	
	float* GetLineKoeffs(Vec2 p1, Vec2 p2){
		float* out = new float[3];
		
		Vec2 line_vec = p2 - p1;
		Vec2 line_normal = Vec2(-line_vec.y, line_vec.x); //A,B - координаты вектора нормали прямой, 
														//делаем (x,y) -> (-y,x) и поворачиваем на 90
		
		float A = line_normal.x;
		float B = line_normal.y;
		float C = -A*p1.x - B*p1.y;
		
		out[0] = A; out[1] = B; out[2] = C;
		
		return out;
	}
	
	float GetLinePointOrientation(Vec2 l1, Vec2 l2, Vec2 p){
		float* line = GetLineKoeffs(l1,l2);
		if(line == nullptr){ return 0; } //это чертовски неправильно, но движок обязан стоять как дом павлова, 
										//так что никаких выбрасываний исключений

		float A = line[0], B = line[1], C = line[2];
		
		delete[] line;

		return A*p.x + B*p.y + C;
	}
	
	LineIntersectionResult GetLineIntersection(Vec2 l1_1, Vec2 l1_2, Vec2 l2_1, Vec2 l2_2){
		LineIntersectionResult out;
		
		float D1 = GetLinePointOrientation(l1_1, l1_2, l2_1);
		float D2 = GetLinePointOrientation(l1_1, l1_2, l2_2);
		
		out.intersect = (D1 < 0 && D2 > 0) || (D1 > 0 && D2 < 0); 
		//подстановка дает разные знаки => разные полуплоскости => пересекаются
		
		if(out.intersect){ //отрезки пересекаются, считаем точку пересечения прямых, проведенных по этим отрезкам
			float* line1 = GetLineKoeffs(l1_1, l1_2);
			float* line2 = GetLineKoeffs(l2_1, l2_2);
			
			if(line1==nullptr || line2==nullptr){ return out; }
			
			float a1 = line1[0], b1 = line1[1], c1 = line1[2];
			float a2 = line2[0], b2 = line2[1], c2 = line2[2];
			
			delete[] line1;
			delete[] line2;
			
			float x = (c1 * b2 - c2 * b1) / (b1 * a2 - b2 * a1);
			float y = (c2 * a1 - c1 * a2) / (b1 * a2 - b2 * a1);
			
			out.intersect_pos = Vec2(x,y);
		}
		
		return out;
	}
	
	std::vector<std::wstring> SplitString(std::wstring str, wchar_t delim){
		std::wstring tmp;
		std::vector<std::wstring> out;
		
		
		for(std::size_t i = 0; i < str.size()+1; i++){
			if(i == str.size()){
				out.push_back(tmp);
				break;
			}

			if(str[i] != delim){
				tmp = tmp + str[i];
			}else{
				out.push_back(tmp);
				tmp.clear();
			}
		}
		
		return out;
	}
	
	std::vector<std::string> SplitString(std::string str, char delim){
		std::string tmp;
		std::vector<std::string> out;
		
		for(std::size_t i = 0; i < str.size()+1; i++){
			if(i == str.size()){
				out.push_back(tmp);
				break;
			}

			if(str[i] != delim){
				tmp = tmp + str[i];
			}else{
				out.push_back(tmp);
				tmp.clear();
			}
		}
		
		return out;
	}
}