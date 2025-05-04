#include "RandomWalkerRunner.hpp"    

RandomWalkerRunner::RandomWalkerRunner(SceneManager* scene_manager, QObject* parent)
	: QObject(parent), scene_manager_(scene_manager) {}

void RandomWalkerRunner::start_segmentation()
{
	const auto result = compute_segmentation();
	if (!result.has_value()) {
		qWarning() << "[RandomWalker] Segmentation aborted: no image available.";
		return;
	}

	const auto& result_matrix = *result;

	const int zero_count = (result_matrix.array() == 0).count();
	const int one_count = (result_matrix.array() == 1).count();
	qDebug() << "[RandomWalker] Result matrix: zeros =" << zero_count << ", ones =" << one_count;

	const QImage rendered = render_segmentation_result(result_matrix);
	scene_manager_->set_segmentation_result(rendered);
}

std::optional<Eigen::Matrix<uint8_t, Eigen::Dynamic, Eigen::Dynamic>> RandomWalkerRunner::compute_segmentation() const
{
	const auto maybe_image = scene_manager_->image_data().maybe_matrix();
	if (!maybe_image.has_value()) return std::nullopt;

	const auto& image_matrix = *maybe_image;
	const auto background = scene_manager_->background_seeds();
	const auto object = scene_manager_->object_seeds();

	algorithm::RandomWalkerAlgorithm algorithm(image_matrix, background, object);
	return algorithm.run();
}

QImage RandomWalkerRunner::render_segmentation_result(const Eigen::Matrix<uint8_t, Eigen::Dynamic, Eigen::Dynamic>& result_matrix) const
{
	constexpr QColor kColorObject(0, 255, 0, 100);
	constexpr QColor kColorBackground(255, 0, 255, 100);

	QImage result(result_matrix.cols(), result_matrix.rows(), QImage::Format_ARGB32);
	for (size_t y = 0; y < result.height(); ++y)
	{
		for (size_t x = 0; x < result.width(); ++x)
		{
			const QColor color = result_matrix(y, x) == 1 ? kColorObject : kColorBackground;
			result.setPixelColor(x, y, color);
		}
	}
	return result;
}