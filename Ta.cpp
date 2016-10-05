#include <iostream>
#include <iomanip>  
#include <queue>
#include <cmath>

#define NSZ(U,L) std::abs((int)L - (int)U)

struct Square
{
	unsigned int p; /* Cube P */
	unsigned int q; /* Cube Q */

	Square(unsigned int p, unsigned int q) : p(p), q(q), cube_cumulative((unsigned int)pow(p, 3) + (unsigned int)pow(q, 3)) {}
	
	bool Ta(const Square& sq) const {
		return cube_cumulative == sq.cube_cumulative
			&& p != sq.p
			&& q != sq.q;
	}

	friend class SquareCompare;
	friend std::ostream& operator<<(std::ostream& os, const Square& sq) {
		return os << sq.p << "^3 + " << sq.q << "^3 = " << sq.cube_cumulative;
	}

private:
	unsigned int cube_cumulative;
};

class SquareCompare
{
public:
	bool operator()(const Square& lhs, const Square& rhs) {
		return lhs.cube_cumulative < rhs.cube_cumulative;
	}
};

int main() {
	unsigned int NU = 2001;
	unsigned __int64 found = 0, iter = 0;
	bool *in_p = nullptr, *in_q = nullptr;
	unsigned int lomin_x, lomin_y, lomin_z, lomin_t;

	std::cout << "Upper bound: ";
	std::cin >> NU;
	
	/* Sanity */
	if (NU > 2000) NU = 2000;
	if (!NU) NU = 1;

	in_p = new bool[NU];
	in_q = new bool[NU];

	/* Nullify */
	for (unsigned int i = 0; i < NU; ++i)
		in_p[i] = in_q[i] = false;

	std::priority_queue<Square, std::vector<Square>, SquareCompare> p_queue;

	/* Initialization vector */
	p_queue.push(Square(NU - 1, NU - 1));
	in_p[NU - 1] = true;
	in_q[NU - 1] = true;

	while (!p_queue.empty()) {
		Square sq = p_queue.top();
		p_queue.pop();
		
		in_p[sq.p] = false;
		in_q[sq.q] = false;

		++iter;

		/* Cube P typical down step 1 */
		if (sq.p > 0 && !in_p[sq.p - 1] && sq.p - 1 >= sq.q) {
			p_queue.push(Square(sq.p - 1, sq.q));
			in_p[sq.p - 1] = true;
			in_q[sq.q] = true;
		}

		/* Cube Q typical down step 1 */
		if (sq.q > 0 && !in_q[sq.q - 1] && sq.p >= sq.q - 1) {
			p_queue.push(Square(sq.p, sq.q - 1));
			in_p[sq.p] = true;
			in_q[sq.q - 1] = true;
		}

		/* Equals? */
		if (!p_queue.empty() && sq.Ta(p_queue.top())) {
			std::cout << sq << " = " << p_queue.top() << std::endl;
			lomin_x = sq.p;
			lomin_y = sq.q;
			lomin_z = p_queue.top().p;
			lomin_t = p_queue.top().q;
			found++;
		}
	}

	long double ratio = ((double)found / (double)iter) * 100.0;

	std::cout << std::endl;
	std::cout << "Convergence result: " << std::endl;
	std::cout << "  Stepsize: " << 1 << std::endl;
	std::cout << "  Iterations: " << iter << " at " << std::setprecision(2) << ratio << "%" << std::endl;
	std::cout << "  " << found << " positive integers with 2 cubes < " << NU << std::endl;
	std::cout << "  Local minimum x:" << lomin_x << ", y:" << lomin_y << ", z:" << lomin_z << ", t:" << lomin_t << std::endl;
	
	system("PAUSE");

	delete[] in_p;
	delete[] in_q;

	return 0;
}
