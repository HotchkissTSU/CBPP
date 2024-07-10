#include "cbpp/matrix.h"

#include "cbpp/cb_alloc.h"
#include "cbpp/math.h"

#include <math.h>
#include <iostream>
#include <iomanip>

using namespace std;

namespace cbpp{
	Matrix::Matrix(Matrix& other){
		Set(other);
	}
	
	Matrix::Matrix(){
		Allocate(2,2);
	}
	
	Matrix::Matrix(uint16_t n){
		Allocate(n, n);
	}
	
	Matrix::Matrix(uint16_t sx, uint16_t sy){
		Allocate(sx, sy);
	}
	
	uint16_t Matrix::X(){ return sz_x; }
	uint16_t Matrix::Y(){ return sz_y; }
	
	float& Matrix::At(uint16_t x, uint16_t y){
		if( allocated && (x < sz_x) && (y < sz_y) ){
			return matrix[y][x];
		}else{
			return zeroflt;
		}
	}
	
	float& Matrix::operator()(uint16_t x, uint16_t y){
		return At(x, y);
	}
	
	void Matrix::Set(Matrix& other){
		if(!other.IsValid()){ return; }
		Allocate(other.X(), other.Y());
		if(other.matrix != nullptr && (&other != this)){
			for(uint16_t x = 0; x < sz_x; x++){
				for(uint16_t y = 0; y < sz_y; y++){
					At(x,y) = other.At(x,y);
				}
			}
		}
	}
	
	void Matrix::operator=(Matrix& other){	
		Set(other);
	}
	
	bool Matrix::operator==(Matrix& other){
		if(!IsValid() || !other.IsValid()){ return false; }
		if(sz_x != other.X() || sz_y != other.Y()){ return false; }
		
		for(uint16_t x = 0; x < sz_x; x++){
			for(uint16_t y = 0; y < sz_y; y++){
				if( !fEqual(At(x,y), other.At(x,y)) ){
					return false;
				}
			}
		}
		
		return true;
	}
	
	Matrix Matrix::operator+(Matrix& other){
		uint16_t sx = std::min(other.X(), sz_x); //перестраховка для дебила
		uint16_t sy = std::min(other.Y(), sz_y);
		
		Matrix out(sx, sy);
		
		if(!allocated && !other.allocated){ return out; }
		
		for(uint16_t x = 0; x < sx; x++){
			for(uint16_t y = 0; y < sy; y++){
				out.At(x,y) = At(x,y) + other.At(x,y);
			}
		}
		
		return out;
	}
	
	Matrix Matrix::operator-(Matrix& other){
		uint16_t sx = std::min(other.X(), sz_x);
		uint16_t sy = std::min(other.Y(), sz_y);
		
		Matrix out(sx, sy);
		
		if(!IsValid() && !other.IsValid()){ return out; }
		
		for(uint16_t x = 0; x < sx; x++){
			for(uint16_t y = 0; y < sy; y++){
				out.At(x,y) = At(x,y) - other.At(x,y);
			}
		}
		
		return out;
	}
	
	Matrix Matrix::operator*(float other){	
		uint16_t sx = sz_x, sy = sz_y;
		Matrix out(sx, sy);
		
		if(!allocated){ return out; }
		
		for(uint16_t x = 0; x < sx; x++){
			for(uint16_t y = 0; y < sy; y++){
				out.At(x,y) = At(x,y) * other;
			}
		}
		
		return out;
	}
	
	Matrix Matrix::operator*(int other){
		return operator*((float)other);
	}
	
	Matrix Matrix::operator*(Matrix& other){ //A = self, B = other
		if(!IsValid() && !other.IsValid()){ return Matrix(1); }
		if(sz_y != other.X()){ return Matrix(1); }
		
		uint16_t sx = sz_x, sy = other.Y();
		
		Matrix out(sx, sy);
		
		for(uint16_t x = 0; x < sx; x++){
			for(uint16_t y = 0; y < sy; y++){
				float sum = 0.0f;
				for(uint16_t i = 0; i < sz_x; i++){
					sum = sum + At(i,x)*other.At(y,i);
				}
				out.At(x,y) = sum;
			}
		}
		
		return out.GetTransposed();
	}
	
	Matrix Matrix::operator/(float other){
		if(!IsValid()){ return Matrix(1); }
		if(other == 0.0f){ other = 2e-23f; }
		
		Matrix out(sz_x, sz_y);
		for(uint16_t x = 0; x < sz_y; x++){
			for(uint16_t y = 0; y < sz_y; y++){
				out.At(x,y) = At(x,y)/other;
			}
		}
		
		return out;
	}
	
	Matrix Matrix::operator/(int other){
		return operator/((float)other);
	}
	
	bool Matrix::Square(){
		return (sz_x == sz_y);
	}
	
	bool Matrix::IsValid(){
		return allocated && (sz_x > 0) && (sz_y > 0) && (matrix != nullptr);
	}
	
	float Matrix::Det(){
		return _det(*this);
	}
	
	Matrix Matrix::Inv(){
		if(!Square()){ return Matrix(1); }
		
		Matrix out(sz_x, sz_y);
		Matrix tmp = GetTransposed();
		
		for(uint16_t x = 0; x < sz_x; x++){
			for(uint16_t y = 0; y < sz_y; y++){
				int8_t degree;
				
				if((x+y)%2 == 0){
					degree = 1;
				}else{
					degree = -1;
				}
				
				out.At(x,y) = degree*tmp.GetMinor(x,y).Det();
			}
		}
		
		float det = Det();
		
		if(det == 0){
			det = CBPP_MATH_EPSILON;
		}
		
		return out / det;
	}
	
	Matrix Matrix::GetTransposed(){
		Matrix out(sz_y, sz_x);
		
		for(uint16_t x = 0; x < sz_x; x++){
			for(uint16_t y = 0; y < sz_y; y++){
				out.At(y, x) = At(x, y);
			}
		}
		
		return out;
	}
	
	Matrix Matrix::GetMinor(uint16_t _x, uint16_t _y){
		Matrix out(sz_x-1, sz_y-1);
		
		if(sz_x > 1 && sz_y > 1 && allocated){		
			uint16_t dx = 0, dy = 0;
			
			for(uint16_t y = 0; y < sz_y; y++){
				dx = 0;
				
				if(y != _y){
					for(uint16_t x = 0; x < sz_x; x++){
						if(x != _x){
							out.matrix[dy][dx] = matrix[y][x];
							dx++;
						}
					}
					dy++;
				}
			}
		}
		
		return out;
	}
	
	void Matrix::Print(){
		if(allocated){
			
			std::cout << std::fixed << std::showpoint;
			std::cout << std::setprecision(3);
			
			for(uint16_t y = 0; y < sz_y; y++){
				for(uint16_t x = 0; x < sz_x; x++){
					cout<<std::setw(6)<<At(x, y)<<' ';
				}
				cout<<'\n';
			}
		}else{
			cout<<"Not allocated matrix\n";
		}
	}
	
	void Matrix::Allocate(uint16_t sx, uint16_t sy){
		if(allocated){
			Free();
		}
		
		if(sx <= 0 || sy <= 0){
			sx = 1;
			sy = 1;
		}
		
		matrix = new float*[sy];
		for(uint16_t y = 0; y < sy; y++){
			matrix[y] = new float[sx];
			
			for(uint16_t x = 0; x < sx; x++){
				matrix[y][x] = 0.0f;
			}
		}
		
		allocated = true;
		sz_x = sx;
		sz_y = sy;
	}
	
	void Matrix::Free(){
		if(allocated){
			for(uint16_t i = 0; i < sz_y; i++){
				delete[] matrix[i];
			}
			delete[] matrix;
			
			matrix = nullptr;
			sz_x = 0;
			sz_y = 0;
			
			allocated = false;
		}
	}
	
	Matrix::~Matrix(){
		Free();
	}
	
	float _det(Matrix& m){
		if(m.Square() && m.IsValid()){
			if(m.X() == 2){
				return ( m(0,0)*m(1,1) - m(1,0)*m(0,1) );
				
			}else if(m.X() == 1){
				return m(0,0);
				
			}else{
				float sum = 0;
				int degree = 1;
				for(uint16_t i = 0; i < m.X(); i++){
					Matrix minor = m.GetMinor(i,0);
					
					float mdet = _det( minor );
					
					sum = sum + degree*mdet*m.At(i,0);
					
					degree = -degree;
				}
				
				return sum;
			}
		}else{
			return 0.0f;
		}
	}
}

namespace cbpp { //Matrix2D
	Matrix2D::Matrix2D() {}
	
	Matrix2D::Matrix2D(Matrix2D& other) {
		At(0,0) = other.At(0,0);
		At(0,1) = other.At(0,1);
		At(1,0) = other.At(1,0);
		At(1,1) = other.At(1,1);
	}
	
	Matrix2D Matrix2D::operator+(Matrix2D& other) {
		Matrix2D out;
		
		out.At(0,0) = At(0,0) + other.At(0,0);
		out.At(1,0) = At(1,0) + other.At(1,0);
		out.At(0,0) = At(0,1) + other.At(0,1);
		out.At(0,0) = At(1,1) + other.At(1,1);
		
		return out;
	}
	
	Matrix2D Matrix2D::operator-(Matrix2D& other) {
		Matrix2D out;
		
		out.At(0,0) = At(0,0) - other.At(0,0);
		out.At(1,0) = At(1,0) - other.At(1,0);
		out.At(0,0) = At(0,1) - other.At(0,1);
		out.At(0,0) = At(1,1) - other.At(1,1);
		
		return out;
	}
	
	Matrix2D Matrix2D::operator*(float other) {
		Matrix2D out;
		
		out.At(0,0) = At(0,0) * other;
		out.At(1,0) = At(1,0) * other;
		out.At(0,0) = At(0,1) * other;
		out.At(0,0) = At(1,1) * other;
		
		return out;
	}
	
	Matrix2D Matrix2D::operator*(Matrix2D& other) {
		Matrix2D out;
		
		out.At(0,0) = At(0,0)*other.At(0,0) + At(1,0)*other.At(0,1);
		out.At(1,0) = At(0,0)*other.At(1,0) + At(1,0)*other.At(1,1);
		out.At(0,1) = At(0,1)*other.At(0,0) + At(1,1)*other.At(0,1);
		out.At(1,1) = At(0,1)*other.At(1,0) + At(1,1)*other.At(1,1);
		
		return out;
	}
	
	Matrix2D Matrix2D::operator/(float other) {
		if(fEqual(other, 0.0f)){
			other = CBPP_MATH_EPSILON;
		}
		
		Matrix2D out;
		
		out.At(0,0) = At(0,0) / other;
		out.At(1,0) = At(1,0) / other;
		out.At(0,0) = At(0,1) / other;
		out.At(0,0) = At(1,1) / other;
		
		return out;
	}
	
	void Matrix2D::operator=(Matrix2D& other) {
		if(this == &other){ return; }
		
		At(0,0) = other.At(0,0);
		At(1,0) = other.At(1,0);
		At(0,1) = other.At(0,1);
		At(1,1) = other.At(1,1);
	}
	
	bool Matrix2D::operator==(Matrix2D& other) {
		return (At(0,0) == other.At(0,0)) && (At(1,0) == other.At(1,0)) && (At(0,1) == other.At(0,1)) && (At(1,1) == other.At(1,1));
	}
	
	float& Matrix2D::operator()(uint8_t x, uint8_t y) {
		return At(x, y);
	}
	
	float& Matrix2D::At(uint8_t x, uint8_t y) {
		if(x > 1 || y > 1){ return zerov; }
		
		uint8_t index = y + y + x;
		return matrix[index];
	}
	
	float Matrix2D::Det() {
		return At(0,0)*At(1,1) + At(1,0)*At(0,1);
	}
}