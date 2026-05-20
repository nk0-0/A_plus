#include <iostream>
#include <map>
#include <string>
#include <stdexcept>

class Expression {
public:
    virtual ~Expression() = default;
    virtual void print(std::ostream& os) const = 0;
    virtual int calculate(const std::map<std::string, int>& values) const = 0;
};

class Flyweight : public Expression {
public:
    void addReference() { ++refCount_; }
    void releaseReference() { 
        if (--refCount_ == 0 && !isImmortal()) 
            delete this; 
    }
    virtual bool isImmortal() const = 0;
    virtual ~Flyweight() = default;

protected:
    int refCount_ = 0;
};

class Constant : public Flyweight {
private:
    int value_;
    bool immortal_;

public:
    Constant(int value, bool immortal = false) : value_(value), immortal_(immortal) {}

    int getValue() const { return value_; }
    bool isImmortal() const override { return immortal_; }

    void print(std::ostream& os) const override { os << value_; }
    int calculate(const std::map<std::string, int>& values) const override { return value_; }
};

class Variable : public Flyweight {
private:
    std::string name_;

public:
    Variable(const std::string& name) : name_(name) {}

    const std::string& getName() const { return name_; }
    bool isImmortal() const override { return false; }

    void print(std::ostream& os) const override { os << name_; }
    int calculate(const std::map<std::string, int>& values) const override {
        auto it = values.find(name_);
        if (it == values.end())
            throw std::runtime_error("Variable not found: " + name_);
        return it->second;
    }
};

class ExpressionFactory;
ExpressionFactory& getFactory();

class BinaryOperator : public Expression {
protected:
    Expression* left_;
    Expression* right_;

public:
    BinaryOperator(Expression* left, Expression* right) : left_(left), right_(right) {}
    virtual ~BinaryOperator();
};

class ExpressionFactory {
private:
    std::map<int, Constant*> constants_;
    std::map<std::string, Variable*> variables_;

    ExpressionFactory() {
        for (int i = -5; i <= 256; ++i) {
            constants_[i] = new Constant(i, true);
        }
    }

    ~ExpressionFactory() {
        for (auto& p : constants_) delete p.second;
        for (auto& p : variables_) delete p.second;
    }

public:
    static ExpressionFactory& instance() {
        static ExpressionFactory factory;
        return factory;
    }

    Constant* createConstant(int value) {
        auto it = constants_.find(value);
        if (it != constants_.end()) {
            it->second->addReference();
            return it->second;
        }
        Constant* c = new Constant(value, false);
        c->addReference();
        constants_[value] = c;
        return c;
    }

    Variable* createVariable(const std::string& name) {
        auto it = variables_.find(name);
        if (it != variables_.end()) {
            it->second->addReference();
            return it->second;
        }
        Variable* v = new Variable(name);
        v->addReference();
        variables_[name] = v;
        return v;
    }

    void release(Expression* expr) {
        if (auto fw = dynamic_cast<Flyweight*>(expr)) {
            bool imm = fw->isImmortal();
            if (auto c = dynamic_cast<Constant*>(fw)) {
                int val = c->getValue();
                fw->releaseReference();
                if (!imm) constants_.erase(val);
            } 
            else if (auto v = dynamic_cast<Variable*>(fw)) {
                std::string name = v->getName();
                fw->releaseReference();
                if (!imm) variables_.erase(name);
            }
        }
    }

    ExpressionFactory(const ExpressionFactory&) = delete;
    ExpressionFactory& operator=(const ExpressionFactory&) = delete;
};

ExpressionFactory& getFactory() { return ExpressionFactory::instance(); }

BinaryOperator::~BinaryOperator() {
    getFactory().release(left_);
    getFactory().release(right_);
}

class Addition : public BinaryOperator {
public:
    Addition(Expression* left, Expression* right) : BinaryOperator(left, right) {}
    void print(std::ostream& os) const override {
        os << "("; left_->print(os); os << " + "; right_->print(os); os << ")";
    }
    int calculate(const std::map<std::string, int>& values) const override {
        return left_->calculate(values) + right_->calculate(values);
    }
};

class Subtraction : public BinaryOperator {
public:
    Subtraction(Expression* left, Expression* right) : BinaryOperator(left, right) {}
    void print(std::ostream& os) const override {
        os << "("; left_->print(os); os << " - "; right_->print(os); os << ")";
    }
    int calculate(const std::map<std::string, int>& values) const override {
        return left_->calculate(values) - right_->calculate(values);
    }
};

class Multiplication : public BinaryOperator {
public:
    Multiplication(Expression* left, Expression* right) : BinaryOperator(left, right) {}
    void print(std::ostream& os) const override {
        os << "("; left_->print(os); os << " * "; right_->print(os); os << ")";
    }
    int calculate(const std::map<std::string, int>& values) const override {
        return left_->calculate(values) * right_->calculate(values);
    }
};

class Division : public BinaryOperator {
public:
    Division(Expression* left, Expression* right) : BinaryOperator(left, right) {}
    void print(std::ostream& os) const override {
        os << "("; left_->print(os); os << " / "; right_->print(os); os << ")";
    }
    int calculate(const std::map<std::string, int>& values) const override {
        int denom = right_->calculate(values);
        if (denom == 0) throw std::runtime_error("Division by zero");
        return left_->calculate(values) / denom;
    }
};

int main() {
    ExpressionFactory& factory = ExpressionFactory::instance();

    Constant* c2 = factory.createConstant(2);
    Variable* vx = factory.createVariable("x");
    Addition* expr1 = new Addition(c2, vx);

    std::map<std::string, int> values{{"x", 3}};
    std::cout << "Expression1: ";
    expr1->print(std::cout);
    std::cout << " = " << expr1->calculate(values) << std::endl;
    std::cout << "Address of constant 2: " << c2 << std::endl;

    delete expr1; // освобождает c2 и vx

    // Повторное использование той же константы 2
    Constant* с2_reused = factory.createConstant(2);
    std::cout << "Address again: " << с2_reused << " (same object)\n";

    // Предопределённая константа 2 не удаляется
    factory.release(с2_reused);
    std::cout << "After release, constant 2 still exists:";
    Constant* c2_check = factory.createConstant(2);
    std::cout << "address = " << c2_check << " (same as before)\n";

    return 0;
}