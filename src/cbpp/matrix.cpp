#include "cbpp/matrix.h"

#include <math.h>
#include <iostream>

using namespace std;

namespace cbpp{
	
	void Matrix::_setmatrix(const Matrix* m2){
		for(uint16_t i = 0; i < size; i++){
			for(uint16_t j = 0; j < size; j++){
				matrix[i][j] = m2->matrix[i][j];
			}
		}
	}
	
	Matrix& Matrix::operator=(const Matrix* m2){
		if(m2 != nullptr){
			if(m2->allocated){
				if(allocated){
					if(size == m2->size){
						_setmatrix(m2);
					}else{
						_free();
						
						size = m2->size;
						_allocate();
						_setmatrix(m2);
					}
				}else{
					size = m2->size;
					_allocate();
					_setmatrix(m2);
				}
			}
		}
		
		return *this;
	}
	
	void Matrix::_allocate(){
		
		if(allocated){
			_free();
		}
		
		matrix = new float*[size];
		
		for(uint16_t i = 0; i < size; i++){
			matrix[i] = new float[size];
			
			for(uint16_t j = 0; j < size; j++){
				matrix[i][j] = 0.0f;
			}
		}
		
		allocated = true;
	}
	
	void Matrix::_free(){
		if(allocated){
			for(uint16_t i = 0; i < size; i++){
				if(matrix[i] != nullptr){
					delete[] matrix[i];
				}
			}
			
			delete[] matrix;
			matrix = nullptr;	
			
			allocated = false;
		}
	}
	
	Matrix::Matrix(uint16_t msize){
		allocated = false;
		
		if(msize == 0){
			size = 3;
		}else{
			size = msize;
		}
		
		//cerr<<"MATRIX "<<this<<" ALLOCATED\n";
		
		_allocate();
	}
	
	Matrix::Matrix(){ allocated = false; }
	
	Matrix::~Matrix(){
		//cerr<<"MATRIX "<<this<<" DEALLOCATED\n";
		_free();
	}
	
	float& Matrix::operator()(uint16_t x, uint16_t y){
		if(x-1 < size && y-1 < size){
			return matrix[y-1][x-1];
		}else{
			//бля
		}
	}
	
	float& Matrix::At(uint16_t x, uint16_t y){
		//cerr<<"SZ "<<size<<endl;
		if(x-1 < size && y-1 < size){
			return matrix[y-1][x-1];
		}else{
			//бля
		}
	}
	
	Matrix Matrix::GetMinor(uint16_t x, uint16_t y){
		Matrix out;
		
		if(size > 2){
			out = Matrix(size - 1);
			
			uint16_t tx = 0, ty = 0;
			
			for(uint16_t _y = 0; _y < size; _y++){
				tx = 0;
				
				if(_y != (y-1)){
					ty++;
					for(uint16_t _x = 0; _x < size; _x++){
						if(_x != (y-1)){
							tx++;
							
							out.matrix[ty-1][tx-1] = matrix[_y][_x];
						}
					}
				}
			}
			
		}else{
			out = Matrix(3);
		}
		
		return out;
	}
	
	float _det(Matrix m){
		float det = 0.0f;
		int degree = 1;
		
		uint16_t sz = m.size;
		if(sz > 2){
			
			for(uint16_t j = 0; j < m.size; j++){				
				det = det + degree*m.matrix[0][j]*_det(m.GetMinor(1,j+1));
				degree = -degree;
			}
			
			return det;
			
		}else{
			return m.At(1,1)*m.At(2,2) + m.At(2,1)*m.At(1,2);
		}
	}
	
	std::ostream& operator << (std::ostream &os, Matrix& mat){
		os<<std::endl;
		os<<"AMOGUS\n";
		
		if(mat.matrix != nullptr){
			for(uint16_t y = 0; y < mat.size; y++){
				if(mat.matrix[y] != nullptr){
					for(uint16_t x = 0; x < mat.size; x++){
						os<<mat.matrix[y][x]<<" ";
					}
					os<<std::endl;
				}
			}
		}else{
			os<<"Matrix is NULL\n";
		}
		
		return os;
	}
	
	float Matrix::Determinant(){
		return _det(*this);
	}
}