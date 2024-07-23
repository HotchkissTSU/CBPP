#include "cbpp/math.h"

namespace cbpp{	
	bool fEqual(float a, float b){
		return (std::abs(a-b) <= CBPP_MATH_EPSILON);
	}
	
	float LineSubstitute(mLine line, Vec2 pt){
		return line.a*pt.x + line.b*pt.y + line.c;
	}
	
	bool PointOnLine(mLine line, Vec2 pt){
		return fEqual(LineSubstitute(line,pt), 0.0f);
	}
	
	bool PointOnSegment(mSegment seg, Vec2 pt){
		Vec2 ap = pt - seg.start;
		Vec2 ab = seg.end - seg.start;
		
		return (ap.Length() <= ab.Length());
	}
	
	mLine GetLine(Vec2 p1, Vec2 p2){
		mLine out;
		
		Vec2 linevec = (p2 - p1).Norm();
		linevec.Set(-linevec.y, linevec.x); //вертим вектор на 90 градусов
		
		out.a = linevec.x; out.b = linevec.y; //A,B - координаты вектора нормали к прямой
		out.c = -out.a * p1.x - out.b * p1.y;
		
		out.p1 = p1; out.p2 = p2;
		out.normal = linevec;
		
		return out;
	}
	
	mSegment GetSegment(Vec2 p1, Vec2 p2){
		mSegment out;
		
		float seglen = (p2 - p1).Length();
		
		out.start = p1; out.end = p2;
		out.direction = (p2 - p1) / seglen;
		out.length = seglen;
		
		return out;
	}
	
	Intersection Intersect_LineLine(mLine l1, mLine l2){
		Intersection out;
		
		float m = l2.a, n = l2.b, k = l2.c;
		float a = l1.a, b = l1.b, c = l1.c;
		
		if( fEqual(l1.normal.VectorMul(l2.normal), 0.0f) ){ //прямые параллельны
			out.intersect = false;
			return out;
		}
		
		float y = (m*c - a*k) / (a*n - m*b);
		float x = (-b*y - c) / a;
		
		out.points.push_back(Vec2(x,y));
		out.intersect = true;
		
		return out;
	}
	
	Intersection Intersect_LineCircle(mLine line, mCircle circ){		
		//перегоняем прямую в систему отсчета, в которой начало координат - это центр круга
		//упрощаем уравнение круга, убирая оттуда сдвиги центра по осям
		line = GetLine(line.p1 - circ.pos, line.p2 - circ.pos);
		
		float a = line.a, b = line.b, c = line.c;
		float m = circ.radius;
		
		float d = a*a*( m*m * (b*b + a*a) - c*c );
		
		Intersection out;
		
		if(d < 0){ //решений нет, нет пересечения
			out.intersect = false;
			return out;
			
		}else if(d > 0){ //две точки пересечения
			float root = std::sqrt(d); //считаем этот кал только один раз
			float down = a*a + b*b;
		
			float y1 = (-b*c+root)/down;
			float x1 = -( (b*y1 + c)/a );
			
			float y2 = (-b*c-root)/down;
			float x2 = -( (b*y2 + c)/a );
			
			out.intersect = true;
			out.points.push_back(Vec2(x1,y1) + circ.pos);
			out.points.push_back(Vec2(x2,y2) + circ.pos); //возвращаем точки в исходную систему координат
			return out;
			
		}else{ //одна точка пересечения
			float y = -( (b*c)/(a*a + b*b) );
			float x = -( (b*y + c)/a );
			
			out.intersect = true;
			out.points.push_back(Vec2(x,y) + circ.pos);
			
			return out;
		}
	}
	
	Intersection Intersect_LineSegment(mLine line, mSegment seg){
		mLine seg_line = GetLine(seg.start, seg.end);
		Intersection lines_isec = Intersect_LineLine(line, seg_line);
		
		Intersection out;
		if(lines_isec.intersect){
			Vec2 isec_pt = lines_isec.points[0];
			
			bool pt_onseg = PointOnSegment(seg, isec_pt);
			
			out.intersect = pt_onseg;			
			
			out.points.push_back(isec_pt);
		}else{
			out.intersect = false;
		}
		
		return out;
	}
	
	Intersection Intersect_SegmentCircle(mSegment a, mCircle b){
		mLine seg_line = GetLine(a.start, a.end);
		
		Intersection isec = Intersect_LineCircle(seg_line, b), out;
		out.intersect = false;
		
		for(uint8_t i = 0; i < isec.points.size(); i++){
			Vec2 pt = isec.points[i];
			
			if(PointOnSegment(a, pt)){
				out.intersect = true;
				out.points.push_back(pt);
			}
		}
		
		return out;
	}
	
	Intersection Intersect_SegmentSegment(mSegment seg1, mSegment seg2){
		mLine l1 = GetLine(seg1.start, seg1.end), l2 = GetLine(seg2.start, seg2.end);
		Intersection lines_isec = Intersect_LineLine(l1, l2);
		
		Intersection out;
		
		if(lines_isec.intersect){
			Vec2 pt = lines_isec.points[0];
			
			out.intersect = PointOnSegment(seg1, pt) && PointOnSegment(seg2, pt);
			out.points.push_back(pt);
		}
		
		out.intersect = false;
		return out;
	}
	
	Intersection Intersect_CircleCircle(mCircle c1, mCircle c2){
		//Как и в поиске пересечения прямой и круга, перегоняем оба круга в новую систему отсчёта,
		//в которой один из кругов лежит в начале координат		
		c2.pos = c2.pos - c1.pos;
		
		Intersection out;
		
		float x0 = c2.pos.x, y0 = c2.pos.y;
		float m = c1.radius, k = c2.radius;
			
		if(fEqual(x0, 0.0f) && fEqual(y0, 0.0f) && fEqual(m, k)){
			out.intersect = true;
			out.points.push_back(c1.pos + Vec2(0.0f,-k));
			out.points.push_back(c1.pos + Vec2(0.0f, k)); //круги совпадают, выдаем две точки на диаметре
			
			return out;
		}
		
		if(fEqual(x0, 0.0f)){
			x0 = CBPP_MATH_EPSILON; //избегаем деления на ноль вот таким креативным способом
		}
		
		float p = -((k*k - m*m - x0*x0 - y0*y0)/2.0f);
		float summ = x0*x0 + y0*y0;
		
		float d_root = std::sqrt( m*m*summ - p*p );
		float kf = std::abs(2*x0);
		
		float d = kf*d_root;
		
		if(d > 0){
			float y1 = (p*y0 + x0*d_root)/summ;
			float y2 = (p*y0 - x0*d_root)/summ;
			
			float x1 = (p - y1*y0)/x0;
			float x2 = (p - y2*y0)/x0;
			
			out.intersect = true;
			out.points.push_back(Vec2(x1,y1) + c1.pos);
			out.points.push_back(Vec2(x2,y2) + c1.pos);
			return out;
			
		}else if(fEqual(d, 0.0f)){
			float y = (p*y0)/summ;
			float x = (p-y*y0)/x0;
			
			out.intersect = true;
			out.points.push_back(Vec2(x,y) + c1.pos);
			return out;
		}
		
		out.intersect = false;
		return out;
	}
	
	float Clamp(float x, float min, float max) {
		if(x > max){ return max; }
		if(x < min){ return min; }
		return max;
	}
	
	int Clamp(int x, int min, int max) {
		return (int)Clamp( (float)x, (float)min, (float)max );
	}
}