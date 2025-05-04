#pragma once

#include "../scene/SceneManager.hpp"
#include "../algorithm/RandomWalkerAlgorithm.hpp"

#include <QObject>
#include <QImage>

class RandomWalkerRunner : public QObject
{
    Q_OBJECT
public:
    explicit RandomWalkerRunner(SceneManager* scene_manager, QObject* parent = nullptr);

public slots:
    void start_segmentation();

private:
    std::optional<Eigen::Matrix<uint8_t, Eigen::Dynamic, Eigen::Dynamic>> compute_segmentation() const;
    QImage render_segmentation_result(const Eigen::Matrix<uint8_t, Eigen::Dynamic, Eigen::Dynamic>& result_matrix) const;

    SceneManager* scene_manager_ = nullptr;
};
