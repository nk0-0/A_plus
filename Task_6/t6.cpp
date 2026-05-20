#include <iostream>
#include <string>
#include <vector>
#include <memory>

class ControlPoint {
protected:
    std::string name;
    double latitude;
    double longitude;
public:
    ControlPoint(std::string n, double lat, double lon) 
        : name(n), latitude(lat), longitude(lon) {}

    virtual ~ControlPoint() = default;

    std::string getName() const { return name; }
    double getLatitude() const { return latitude; }
    double getLongitude() const { return longitude; }
    
    virtual bool isObligatory() const = 0;
    virtual double getPenalty() const = 0;
};

class ObligatoryControlPoint : public ControlPoint {
public:
    ObligatoryControlPoint(std::string n, double lat, double lon) 
        : ControlPoint(n, lat, lon) {}

    bool isObligatory() const override { return true; }
    double getPenalty() const override { return 0.0; }
};

class OptionalControlPoint : public ControlPoint {
private:
    double penalty;
public:
    OptionalControlPoint(std::string n, double lat, double lon, double p) 
        : ControlPoint(n, lat, lon), penalty(p) {}
    
    bool isObligatory() const override { return false; }
    double getPenalty() const override { return penalty; }
};

class ControlPointProcessor {
public:
    virtual void processControlPoint(const ControlPoint& cp, int index) = 0;
    virtual ~ControlPointProcessor() = default;
};

class ControlPointBuilder : public ControlPointProcessor {
public:
    virtual void showResult() = 0;
    virtual ~ControlPointBuilder() = default;
};

class PrintProcessor : public ControlPointProcessor {
public:
    void processControlPoint(const ControlPoint& cp, int index) override {
        std::cout << "Number: " << index << std::endl;
        std::cout << "   Name: " << cp.getName() << std::endl;
        std::cout << "   Latitude: " << cp.getLatitude() 
                  << " Longitude: " << cp.getLongitude() << std::endl;
        if (!cp.isObligatory()) {
            std::cout << "   Penalty: " << cp.getPenalty() << " hours" << std::endl;
        } else {
            std::cout << "   Stage failure penalty" << std::endl;
        }
        std::cout << std::endl;
    }
};

class PenaltyCounter : public ControlPointBuilder {
private:
    double totalPenalty = 0.0;
public:
    void processControlPoint(const ControlPoint& cp, int index) override {
        if (!cp.isObligatory()) {
            totalPenalty += cp.getPenalty();
        }
    }

    void showResult() override {
        std::cout << "Total penalty: " << totalPenalty << " hours" << std::endl;
    }
};

class Manager {
private:
    std::vector<std::unique_ptr<ControlPoint>> cp_arr;
public:
    Manager() = default;

    void addControlPoint(std::unique_ptr<ControlPoint> cp) {
        cp_arr.push_back(std::move(cp));
    }

    void process(ControlPointProcessor& processor) {
        for (size_t i = 0; i < cp_arr.size(); ++i) {
            processor.processControlPoint(*cp_arr[i], i + 1);
        }
    }

    void process(ControlPointBuilder& builder) {
        for (size_t i = 0; i < cp_arr.size(); ++i) {
            builder.processControlPoint(*cp_arr[i], i + 1);
        }
        builder.showResult();
    }
};

int main() {
    Manager man;

    man.addControlPoint(std::make_unique<ObligatoryControlPoint>("Start", 12.5, 34.85));
    man.addControlPoint(std::make_unique<OptionalControlPoint>("Gorge", 23.45, 35.23, 3.0));
    man.addControlPoint(std::make_unique<OptionalControlPoint>("Mountain", 1.5, 0.85, 6.0));
    man.addControlPoint(std::make_unique<ObligatoryControlPoint>("Finish", 47.6, 19.7));

    PrintProcessor print;
    man.process(print);
    std::cout << std::endl;

    PenaltyCounter pen;
    std::cout << "Penalty:" << std::endl;
    man.process(pen);

    return 0;
}