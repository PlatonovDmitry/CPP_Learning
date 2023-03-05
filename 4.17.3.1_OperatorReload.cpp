#include <iostream>
#include <numeric>
#include <string>

using namespace std;

class Rational {
public:
    Rational() = default;

    Rational(int value)
        : numerator_(value)
        , denominator_(1)
    {
    }

    Rational(int numerator, int denominator)
        : numerator_(numerator)
        , denominator_(denominator)
    {
        Normalize();
    }

    int Numerator() const {
        return numerator_;
    }

    int Denominator() const {
        return denominator_;
    }

    Rational operator+=(Rational r){
        numerator_ = numerator_ * r.Denominator() + r.Numerator() * denominator_;
        denominator_ *= r.Denominator();    
        Normalize();

        return *this;
    }

    Rational operator-=(Rational r){
        numerator_ = numerator_ * r.Denominator() - r.Numerator() * denominator_;
        denominator_ *= r.Denominator();    
        Normalize();

        return *this;
    }

    Rational operator*=(Rational r){
        numerator_ *= r.Numerator();
        denominator_ *= r.Denominator();    
        Normalize();

        return *this;
    }

    Rational operator/=(Rational r){
        numerator_ *= r.Denominator();
        denominator_ *= r.Numerator();    
        Normalize();

        return *this;
    }

private:
    void Normalize() {
        if (denominator_ == 0) {
            throw invalid_argument("деление на ноль не допускается"s);
        }

        if (denominator_ < 0) {
            numerator_ = -numerator_;
            denominator_ = -denominator_;
        }
        int n = gcd(numerator_, denominator_);
        numerator_ /= n;
        denominator_ /= n;
    }

    int numerator_ = 0;
    int denominator_ = 1;
};

ostream& operator<<(ostream& output, Rational rational) {
    return output << rational.Numerator() << '/' << rational.Denominator();
}

istream& operator>>(istream& input, Rational& rational) {
    int numerator;
    int denominator;
    char slash;
    if ((input >> numerator) && (input >> slash) && (slash == '/') && (input >> denominator)) {
        rational = Rational{numerator, denominator};
    }
    return input;
}

Rational operator+(Rational left, Rational right) {
    return left += right;
}

Rational operator-(Rational left, Rational right) {
    return left -= right;
}

Rational operator*(Rational left, Rational right) {
    return left *= right;
}

Rational operator/(Rational left, Rational right) {
    return left /= right;
}

Rational operator+(Rational value) {
    return value;
}

Rational operator-(Rational value) {
    return {-value.Numerator(), value.Denominator()};
}

bool operator==(Rational left, Rational right) {
    return left.Numerator() == right.Numerator() 
    && left.Denominator() == right.Denominator();
}

bool operator!=(Rational left, Rational right) {
    return !(left == right) ;
}

bool operator>(Rational left, Rational right) {
    double left_result = left.Numerator() * 1.0 / left.Denominator();
    double right_result = right.Numerator() * 1.0 / right.Denominator();
    return left_result > right_result ;
}

bool operator<(Rational left, Rational right) {
    double left_result = left.Numerator() * 1.0 / left.Denominator();
    double right_result = right.Numerator() * 1.0 / right.Denominator();
    return left_result < right_result ;
}

bool operator>=(Rational left, Rational right) {
    return !(left < right);
}

bool operator<=(Rational left, Rational right) {
    return !(left > right);
}


