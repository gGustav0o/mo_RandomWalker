#include "RandomWalkerRunner.hpp"    

RandomWalkerRunner::RandomWalkerRunner(SceneManager* scene_manager, QObject* parent)
	: QObject(parent), scene_manager_(scene_manager) {}

void RandomWalkerRunner::start_segmentation()
{
	const auto image_matrix = scene_manager_->image_data().matrix();
	const auto background = scene_manager_->background_seeds();
	const auto object = scene_manager_->object_seeds();

	algorithm::RandomWalkerAlgorithm algorithm(image_matrix, background, object);
	const auto result_matrix = algorithm.run();

	/*QImage result(result_matrix.cols(), result_matrix.rows(), QImage::Format_Grayscale8);
	for (int y = 0; y < result.height(); ++y)
	{
		uchar* line = result.scanLine(y);
		for (int x = 0; x < result.width(); ++x)
			line[x] = result_matrix(y, x);
	}*/

	int zero_count = 0;
	int one_count = 0;
	for (int y = 0; y < result_matrix.rows(); ++y)
	{
		for (int x = 0; x < result_matrix.cols(); ++x)
		{
			if (result_matrix(y, x) == 0)
				++zero_count;
			else if (result_matrix(y, x) == 1)
				++one_count;
		}
	}
	qDebug() << "Result matrix: zeros =" << zero_count << ", ones =" << one_count;

	QImage result(result_matrix.cols(), result_matrix.rows(), QImage::Format_ARGB32);
	for (int y = 0; y < result.height(); ++y)
	{
		for (int x = 0; x < result.width(); ++x)
		{
			int alpha = 100;
			if (result_matrix(y, x) == 1)
				result.setPixelColor(x, y, QColor(0, 255, 0, alpha));
			else
				result.setPixelColor(x, y, QColor(255, 0, 255, alpha));
		}
	}

	scene_manager_->set_segmentation_result(result);
}
