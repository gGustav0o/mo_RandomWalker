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
    SceneManager* scene_manager_ = nullptr;
};
