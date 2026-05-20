#ifndef POINTCLOUD_H_
#define POINTCLOUD_H_

#include <string>
#include <stdexcept>
#include <vector>

class PointCloud {
public:
    void Init(std::string pc_type, size_t points_count) {
        pointcloud_type_ = std::move(pc_type);
        size_ = points_count;
        if (pointcloud_type_ == "XYZIR")
            points_ = std::vector<double>(size_ * 5);
        else if (pointcloud_type_ == "XYZIRDAT")
            points_ = std::vector<double>(size_ * 8);
        else
            throw std::runtime_error("unknown pointcloud type");
    }

    void AddPoint(const std::vector<double>& point, size_t ind) {
        size_t expected = pointSize();
        if (point.size() != expected)
            throw std::runtime_error("Point size mismatch");
        if (ind >= size_)
            throw std::out_of_range("Index out of range");
        for (size_t i = 0; i < point.size(); ++i)
            points_[expected * ind + i] = point[i];
    }

    size_t pointSize() const {
        return (pointcloud_type_ == "XYZIR") ? 5 : 8;
    }

    std::string pointcloud_type_;
    size_t size_ = 0;
    std::vector<double> points_;
};

inline void FillPointCloud(PointCloud* pc, size_t points_count, std::string point_type, const std::vector<double>& data) {
    size_t point_size = (point_type == "XYZIR") ? 5 : 8;
    pc->Init(std::move(point_type), points_count);
    for (size_t i = 0; i < points_count; ++i) {
        auto start = data.begin() + i * point_size;
        auto end = start + point_size;
        if (end > data.end()) throw std::runtime_error("Not enough data");
        pc->AddPoint(std::vector<double>(start, end), i);
    }
}

#endif