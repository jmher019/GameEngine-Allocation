#ifndef PUGGO_RESULT_HPP
#define PUGGO_RESULT_HPP

#include<functional>

using namespace std;

namespace puggo {
	template<typename Left, typename Right>
	class Result {
	private:
		Result(const Left& left): left(left), isError(false) {}
		Result(const Right& right): right(right), isError(true) {}
		
		Left left;
		Right right;
		bool isError = false;

	public:
		Result(const Result<Left, Right>& result): left(result.left), right(result.right), isError(result.isError) {}

		Result(Result<Left, Right>&& result): left(move(result.left)), right(move(result.right)), isError(move(result.isError)) {}

		Result<Left, Right>& operator=(const Result<Left, Right>& result) {
			this->left = result.left;
			this->right = result.right;
			this->isError = result.isError;

			return *this;
		}

		Result<Left, Right>& operator=(Result<Left, Right>&& result) {
			this->left = move(result.left);
			this->right = move(result.right);
			this->isError = move(result.isError);

			return *this;
		}

		Result<Left, Right>& map(function<void(Left)> okLambda) {
			if (!isError) {
				okLambda(left);
			}

			return *this;
		}

		Result<Left, Right>& mapError(function<void(Right)> errorLambda) {
			if (isError) {
				errorLambda(right);
			}

			return *this;
		}

		Result<Left, Right>& matchWith(function<void(Left)> okLambda, function<void(Right)> errorLambda) {
			if (isError) {
				okLambda(left);
			}
			else {
				errorLambda(right);
			}

			return *this;
		}

		static Result<Left, Right> ok(const Left& left) noexcept {
			return Result<Left, Right>(left);
		}
		
		static Result<Left, Right> error(const Right& right) noexcept {
			return Result<Left, Right>(right);
		}
	};
}

#endif // !PUGGO_RESULT_HPP
