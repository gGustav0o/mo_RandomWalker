#include "SceneManager.hpp"

SceneManager::SceneManager(QObject* parent)
    : QObject(parent)
{
}

void SceneManager::add_seed(const QPoint& point, SeedLabel label)
{
    switch (label) {
    case SeedLabel::Background:
        background_seeds_.push_back(point);
        break;
    case SeedLabel::Object:
        object_seeds_.push_back(point);
        break;
    }
    emit seeds_changed();
}

void SceneManager::clear_seeds()
{
    background_seeds_.clear();
    object_seeds_.clear();
    emit seeds_changed();
}

void SceneManager::on_image_changed(const QImage& new_image)
{
    image_data_.update_from(new_image);
    clear_seeds();
}

void SceneManager::add_rect_seed_area(const QRect& rect, SeedLabel label)
{
    for (int y = rect.top(); y <= rect.bottom(); ++y) {
        for (int x = rect.left(); x <= rect.right(); ++x) {
            add_seed(QPoint(x, y), label);
        }
    }
}

ImageData SceneManager::image_data() const noexcept
{
    return image_data_;
}

std::vector<QPoint> SceneManager::background_seeds() const noexcept
{
    return background_seeds_;
}

std::vector<QPoint> SceneManager::object_seeds() const noexcept
{
    return object_seeds_;
}

QImage SceneManager::segmentation_result() const noexcept
{
    qDebug() << "[C++] segmentation_result() called!";
	return segmentation_result_;
}


void SceneManager::set_segmentation_result(const QImage& result)
{
    segmentation_result_ = result;

    qDebug() << "result size:" << result.size();
    qDebug() << "format:" << result.format();
    qDebug() << "pixel at (0,0):" << result.pixelColor(0, 0);

    emit segmentationResultChanged();
}