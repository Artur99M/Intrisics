#include <type_traits>
#include <utility>

namespace {
    template <typename T, size_t nrows_, size_t ncols_>
    struct helping_class : Matrix<T, nrows_, ncols_> {

        helping_class(Matrix<T, nrows_, ncols_>) -> helping_class<T, nrows_, ncols_> {}
    };

}
template <typename T>
struct is_Matrix_specialization : std::true_type, decltype(helping_class(declval<T>())) {
};

