#include <iostream>
#include <vector>
#include <array>
#include <random>

template <typename T, size_t N, size_t M>
class Matrix{
	//std::array<std::array<T, N>, M> matrix;
	std::vector<T> matrix;
	int rand_seed = 9;
public:
	Matrix() {
		std::random_device rd;
		std::mt19937 mersenne(rd());
		for (int i=0; i < N*M; i++)
		    matrix.push_back(mersenne() % 20 - 10);
	}
    Matrix(std::initializer_list<T> &&List){
        for (auto &el : List)
            matrix.push_back(el);
    }
	Matrix(float eps) {
		for (int i=0; i < N*M; i ++)
				matrix.push_back(eps);
	}
	T& get(int x, int y) {
		return matrix[x * N + y];
	}
	T get(int x, int y) const {
		return matrix[x * N + y];
	}
	Matrix<T, M, N> transposed() const{
		Matrix<T, M, N> res;
		for (int i = 0; i < N; i++)
			for (int k = 0; k < M; k++)
				res.matrix[k * M + i] = matrix[i* N + k];
		return res;
	}
	Matrix<T, N, M> operator<(float eps) {
		Matrix<T, N, M> res;
		for (auto i = 0; i < N; i++)
			for (auto k = 0; k < N; k++)
				if (this->matrix[i*N + k] < eps)
				{
					res.matrix[i* N + k] = 1;
				}
				else
				{
					res.matrix[i*N + k] = 0;
				}
		return res;
	}

    bool is_symmetric(float eps=1e-6) const{
        return all(abs(*this - this->transposed()) < eps);
    }
};

template <typename T, size_t N, size_t M>
Matrix<int, N, M> operator<(const Matrix<T, N, M> &a, const Matrix<T, N, M> &b)
{
	Matrix<int, N, M> res;
	for (auto i = 0; i < N; i++)
		for (auto k = 0; k < N; k++)
			if (a.get(i, k) < b.get(i, k))
			{
				res.get(i, k) = 0;
			}
			else
			{
				res.get(i, k) = 1;
			}
	return res;
}

template <typename T, size_t N, size_t M>
Matrix<T, N, M> operator-(const Matrix<T, N, M> &a, Matrix<T, N, M> &&b)
{
	Matrix<T, N, M> res;
	for (auto i = 0; i < N; i++)
		for (auto k = 0; k < N; k++)
			res.get(i, k) = a.get(i, k) - b.get(i, k);
	return res;
}

template <typename T, size_t N, size_t M>
bool operator==(const Matrix<T, N, M> &a, const Matrix<T, N, M> &b){
    for (auto i = 0; i < N; i++)
        for (auto k = 0; k < N; k++)
            if (a.get(i, k) != b.get(i, k))
                return false;
    return true;
}

template <typename T, size_t N, size_t M>
Matrix<T, N, M> where(const Matrix<T, N, M> &&s , const Matrix<T, N, M> &a, const Matrix<T, N, M> &b){
	Matrix<T, N, M> res;
	for (auto i = 0; i < N; i++)
		for (auto k = 0; k < N; k++)
			if (s.get(i, k) == 0)
			{
				res.get(i, k) = a.get(i, k);
			}
			else
			{
				res.get(i, k) = b.get(i, k);
			}
	return res;
}

template <typename T, size_t N, size_t M>
Matrix<T, N, M> abs(Matrix<T, N, M> &&s){
	Matrix<T, N, M> res;
	for (int i = 0; i < N; i++)
		for (int k = 0; k < M; k++)
			res.get(i, k) = abs(s.get(i, k));
	return res;
}

template <typename T, size_t N, size_t M>
bool all(Matrix<T, N, M> &&s) {
	for (int i = 0; i < N; i++)
		for (int k = 0; k < N; k++)
			if (s.get(i, k) == 0)
				return false;
	return true;
}