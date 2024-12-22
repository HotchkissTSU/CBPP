#ifndef CBPP_MATRIXH
#define CBPP_MATRIXH

#include <cstdint>
#include <iostream>

namespace cbpp{
	extern "C" class Matrix{
		public:
			Matrix();
			Matrix(uint16_t n);
			Matrix(uint16_t sx, uint16_t sy);
			
			Matrix(Matrix& other);
			
			uint16_t X();
			uint16_t Y();
			
			bool operator==(Matrix& other);
			
			void operator=(Matrix& other);
			void Set(Matrix& other);
			
			Matrix operator+(Matrix& other);
			Matrix operator-(Matrix& other);
			
			Matrix operator*(float other);
			Matrix operator*(int other);
			Matrix operator*(Matrix& other);
			
			Matrix operator/(float other);
			Matrix operator/(int other);
			
			float& operator()(uint16_t x, uint16_t y);
			float& At(uint16_t x, uint16_t y);
			
			bool Square();
			bool IsValid();
			
			Matrix GetMinor(uint16_t x, uint16_t y);
			Matrix GetTransposed();
			
			float Det();
			Matrix Inv();
			
			void Allocate(uint16_t sx, uint16_t sy);
			void Free();
			
			void Print();
			
			~Matrix();
		
		private:
			uint16_t sz_x = 0, sz_y = 0;
			float** matrix = nullptr;
			bool allocated = false;
			
			float zeroflt = 0.0f;
	};
	
	class Matrix2D {
		public:
			Matrix2D();
			Matrix2D(Matrix2D& other);
			
			Matrix2D operator+(Matrix2D& other);
			Matrix2D operator-(Matrix2D& other);
			
			Matrix2D operator*(float other);
			Matrix2D operator*(Matrix2D& other);
			
			Matrix2D operator/(float other);
			
			void operator=(Matrix2D& other);
			bool operator==(Matrix2D& other);
			
			float& operator()(uint8_t x, uint8_t y);
			float& At(uint8_t x, uint8_t y);
			
			float Det();
			Matrix2D Inv();
			Matrix2D GetTransposed();
			
			void Print();
		
		private:
			float matrix[4];
			float zerov = 0.0f;
	};
	
	float _det(Matrix& m);
}

#endif