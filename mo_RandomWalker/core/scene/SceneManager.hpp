#pragma once

#include <QObject>
#include <QPoint>
#include <QDebug>

#include <vector>

#include "../data/ImageData.hpp"
#include "../enums/SeedLabel.hpp"

class SceneManager : public QObject
{
	Q_OBJECT
	Q_PROPERTY(QImage segmentationResult READ segmentation_result NOTIFY segmentationResultChanged)

public:
	explicit SceneManager(QObject* parent = nullptr);

	Q_INVOKABLE void add_seed(const QPoint& point, SeedLabel label);
	Q_INVOKABLE void add_rect_seed_area(const QRect& rect, SeedLabel label);
	Q_INVOKABLE void clear_seeds();
	Q_INVOKABLE void on_image_changed(const QImage& new_image);

	[[nodiscard]] ImageData image_data() const noexcept;
	[[nodiscard]] std::vector<QPoint> background_seeds() const noexcept;
	[[nodiscard]] std::vector<QPoint> object_seeds() const noexcept;
	[[nodiscard]] QImage segmentation_result() const noexcept;
	void set_segmentation_result(const QImage& result);

signals:
	void seeds_changed();
	void segmentationResultChanged();

private:
	ImageData image_data_;
	std::vector<QPoint> background_seeds_;
	std::vector<QPoint> object_seeds_;
	QImage segmentation_result_;
};
