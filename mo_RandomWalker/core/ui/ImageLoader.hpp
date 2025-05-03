#pragma once

#include <QObject>
#include <QImage>
#include <QString>

#include "../../MetaAnnotations.hpp"

class ImageProvider;

class ImageLoader : public QObject
{
    _no_copy_move_(ImageLoader)
    Q_OBJECT
    Q_PROPERTY(QImage image READ image NOTIFY image_changed)

public:
    explicit ImageLoader(QObject* parent = nullptr);

    Q_INVOKABLE [[nodiscard]] bool load_image(const QString& path) noexcept;
    Q_INVOKABLE void clear_image() noexcept;
    [[nodiscard]] QImage image() noexcept;
    void set_image_provider(_In_opt_ ImageProvider* provider) noexcept;

signals:
    void image_changed();

private:
    QImage image_ {};
    ImageProvider* provider_ = nullptr;
};