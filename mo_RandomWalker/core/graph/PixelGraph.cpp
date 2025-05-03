#include "PixelGraph.hpp"

#include <Eigen/Sparse>
#include <cmath>

namespace graph
{

    namespace
    {
        constexpr double kBeta = 90.0; // чувствительность к градиентам

        enum class Direction {
            Right,
            Down,
            Left,
            Up
        };

        [[nodiscard]] constexpr std::pair<int, int> offset(Direction dir) noexcept
        {
            switch (dir) {
            case Direction::Right: return { 0, 1 };
            case Direction::Down:  return { 1, 0 };
            case Direction::Left:  return { 0, -1 };
            case Direction::Up:    return { -1, 0 };
            }
            return { 0, 0 }; // unreachable
        }

        constexpr std::array<Direction, 4> kDirections = {
            Direction::Right,
            Direction::Down,
            Direction::Left,
            Direction::Up
        };

        inline _pure_(bool is_inside(int y, int x, int height, int width))
        {
            return y >= 0 && y < height && x >= 0 && x < width;
        }

        inline _impure_(double compute_weight(uint8_t g1, uint8_t g2))
        {
            const double diff = static_cast<double>(g1) - static_cast<double>(g2);
            return std::exp(-kBeta * diff * diff);
        }

        _impure_(Eigen::SparseMatrix<double> build_laplacian(const Eigen::Matrix<uint8_t, Eigen::Dynamic, Eigen::Dynamic>& image))
        {
            const int height = static_cast<int>(image.rows());
            const int width = static_cast<int>(image.cols());
            const int n = width * height;

            auto index_at = [width](int row, int col) constexpr noexcept -> int {
                return row * width + col;
                };

            std::vector<Eigen::Triplet<double>> weight_triplets;
            weight_triplets.reserve(n * 4);

            Eigen::VectorXd degrees = Eigen::VectorXd::Zero(n);

            for (int y = 0; y < height; ++y)
            {
                for (int x = 0; x < width; ++x)
                {
                    const int i = index_at(y, x);
                    const uint8_t gi = image(y, x);

                    for (Direction dir : kDirections)
                    {
                        const auto [dy, dx] = offset(dir);
                        const int ny = y + dy;
                        const int nx = x + dx;
                        if (!is_inside(ny, nx, height, width))
                            continue;

                        const int j = index_at(ny, nx);
                        const uint8_t gj = image(ny, nx);

                        const double w = compute_weight(gi, gj);
                        weight_triplets.emplace_back(i, j, -w);
                        degrees[i] += w;
                    }
                }
            }

            for (int i = 0; i < n; ++i)
            {
                weight_triplets.emplace_back(i, i, degrees[i]);
            }

            Eigen::SparseMatrix<double> L(n, n);
            L.setFromTriplets(weight_triplets.begin(), weight_triplets.end());
            return L;
        }
    }

    PixelGraph::PixelGraph(const Eigen::Matrix<uint8_t, Eigen::Dynamic, Eigen::Dynamic>& image)
        : image_(image)
        , width_(static_cast<int>(image.cols()))
        , height_(static_cast<int>(image.rows()))
    {
    }

    int PixelGraph::index_at(int row, int col) const noexcept
    {
        return row * width_ + col;
    }

    std::pair<int, int> PixelGraph::coords_from_index(int index) const noexcept
    {
        return { index / width_, index % width_ };
    }

    Eigen::SparseMatrix<double> PixelGraph::laplacian() const
    {
		return build_laplacian(image_);
    }

} // namespace graph
