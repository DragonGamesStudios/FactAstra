#include "../../extlibs/glm/glm/glm.hpp"
#include "../../extlibs/glm/glm/gtc/type_ptr.hpp"

#include <memory>
#include <type_traits>

namespace sp::math
{
	template<typename _T, size_t _n>
	struct determinant_impl
	{
		static _T determinant(const glm::mat<_n, _n, _T>& _A)
		{
			return 0;
		}
	};

	template<typename _T>
	struct determinant_impl<_T, 2>
	{
		static _T determinant(const glm::mat2& _A)
		{
			// For
			// [ a b ]
			// [ c d ]
			// The equation is
			// ad - bc
			return _A[0][0] * _A[1][1] - _A[0][1] * _A[1][0];
		}
	};

	template<typename _T>
	struct determinant_impl<_T, 3>
	{
		static _T determinant(const glm::mat3& _A)
		{
			// For
			// [ a b c ]
			// [ d e f ]
			// [ g h i ]
			// The equation is
			// aei + bfg + cdh - ceg - bdi - afh
			return _A[0][0] * _A[1][1] * _A[2][2] + _A[0][1] * _A[1][2] * _A[2][0] + _A[0][2] * _A[1][0] * _A[2][1]
				- _A[0][2] * _A[1][1] * _A[2][0] - _A[0][1] * _A[1][0] * _A[2][2] - _A[0][0] * _A[1][2] * _A[2][1];
		}
	};

	template<typename _T, size_t _n>
	_T determinant(const glm::mat<_n, _n, _T>& _A)
	{
		return determinant_impl<_T, _n>::determinant(_A);
	}

	template<typename _T, size_t _n>
	glm::vec<_n, _T> cramer(const glm::mat<_n, _n, _T>& _A, const glm::vec<_n, _T>& _b)
	{
		glm::vec<_n, _T> x;
		for (size_t i = 0; i < _n; i++)
		{
			glm::mat<_n, _n, _T> Ai = _A;
			Ai[i] = _b;
			x[i] = determinant(Ai) / determinant(_A);
		}

		return x;
	}

	template<typename _T, size_t _n>
	bool intersecting_plane_line(const glm::vec<_n, _T>& _p0, const glm::vec<_n, _T>& _norm, const glm::vec<_n, _T>& _l0, const glm::vec<_n, _T>& _l)
	{
		return glm::dot(_l, _norm) && glm::dot(_p0 - _l0, _norm);
	}

	template<typename _T, size_t _n>
	glm::vec<_n, _T> intersect_plane_line(const glm::vec<_n, _T>& _p0, const glm::vec<_n, _T>& _norm, const glm::vec<_n, _T>& _l0, const glm::vec<_n, _T>& _l)
	{
		return _l0 + _l * (glm::dot(_p0 - _l0, _norm) / glm::dot(_l, _norm));
	}

	template<typename _T, size_t _n>
	glm::vec<_n, _T> intersect_line_line(const glm::vec<_n, _T>& _p0, const glm::vec<_n, _T>& _d0, const glm::vec<_n, _T>& _p1, const glm::vec<_n, _T>& _d1)
	{
		return _p0 + _d0 * cramer(glm::mat<2, _n, _T>(_d0, -_d1), _p1 - _p0).x;
	}

	template<typename _T>
	_T linear_interpolate(_T _v0, _T _v1, float_t _t)
	{
		return (1 - _t) * _v0 + _t * _v1;
	}

	template<typename _T, size_t _n>
	_T linear_interpolate_point(
		const glm::vec<_n, float_t>& _point,
		const glm::vec<_n, float_t>& _p0, _T _v0,
		const glm::vec<_n, float_t>& _p1, _T _v1
	)
	{
		return linear_interpolate(_v0, _v1, glm::length(_point - _p0) / glm::length(_p1 - _p0));
	}

	template<typename _T>
	_T triangular_interpolate(
		const glm::vec2& _point,
		const glm::vec2& _p0, _T _v0,
		const glm::vec2& _p1, _T _v1,
		const glm::vec2& _p2, _T _v2
	)
	{
		if (_point == _p0)
			return _v0;

		auto intersection = intersect_line_line(_p1, _p2 - _p1, _p0, _point - _p0);

		return linear_interpolate_point(
			_point,
			_p0, _v0,
			intersection, linear_interpolate_point(
				intersection,
				_p1, _v1,
				_p2, _v2
			)
		);
	}

	template<typename _T>
	_T bilinear_interpolate(
		const glm::vec2& _point,
		_T _topleft,
		_T _topright,
		_T _bottomright,
		_T _bottomleft,
		const glm::vec2& _topleftpt = { 0.0f, 0.0f },
		const glm::vec2& _bottomrightpt = { 1.0f, 1.0f }
	)
	{
		glm::vec<2, _T> v = glm::mat<2, 2, _T>(glm::vec<2, _T>(_topleft, _bottomleft), glm::vec<2, _T>(_topright, _bottomright))
			* glm::vec2(_bottomrightpt.y - _point.y, _point.y - _topleftpt.y);

		// Multiply v by
		// [  _bottomrightpt.x - _point.x    _point.x - _topleftpt.x ]

		_T mult = (_bottomrightpt.x - _point.x) * v.x + (_point.x - _topleftpt.x) * v.y;

		return (_T)(mult / ((_bottomrightpt.x - _topleftpt.x) * (_bottomrightpt.y - _topleftpt.y)));
	}

}