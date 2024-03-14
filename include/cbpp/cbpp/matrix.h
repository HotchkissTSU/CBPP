#ifndef CBPP_MATRIXH
#define CBPP_MATRIXH

#include <cstdint>
#include <iostream>

namespace cbpp{
	extern "C" struct Matrix{
		Matrix(uint16_t size);
		Matrix();
		~Matrix();
		
		float& operator()(uint16_t x, uint16_t y);	
		Matrix& operator=(const Matrix* m2);
		
		float& At(uint16_t x, uint16_t y);
		
		void _allocate();
		void _free();
		void _setmatrix(const Matrix* src);
		
		float Determinant();
		Matrix GetMinor(uint16_t x, uint16_t y);
		
		float** matrix = nullptr;
		uint16_t size;
		
		bool allocated = false;
	};
	
	std::ostream& operator << (std::ostream &os, Matrix& mat);
	
	float _det(Matrix m);
}

#endif