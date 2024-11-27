#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>

using namespace std;

class Polynomial;


class Term {
    friend class Polynomial;
    friend ostream& operator<<(ostream&, const Polynomial&);
    friend istream& operator>>(istream&, Polynomial&);
public:
    Term(float c = 0.0f, int e = 0) : coef(c), exp(e) {}
private:
    float coef;  
    int exp;     
};


class Polynomial {
public:
    Polynomial() {}

    Polynomial Add(const Polynomial& poly) const;
    Polynomial Mult(const Polynomial& poly) const;
    float Eval(float x) const;

    friend istream& operator>>(istream& is, Polynomial& poly);
    friend ostream& operator<<(ostream& os, const Polynomial& poly);

private:
    vector<Term> terms;  //項

    void sortTerms();
};


void Polynomial::sortTerms() {
    sort(terms.begin(), terms.end(), [](const Term& a, const Term& b) {
        return a.exp > b.exp;
        });
}

// 加法
Polynomial Polynomial::Add(const Polynomial& poly) const {
    Polynomial result;
    int i = 0, j = 0;

    while (i < terms.size() && j < poly.terms.size()) {
        if (terms[i].exp == poly.terms[j].exp) {
            float sumCoef = terms[i].coef + poly.terms[j].coef;
            if (sumCoef != 0) {
                result.terms.push_back(Term(sumCoef, terms[i].exp));
            }
            i++;
            j++;
        }
        else if (terms[i].exp > poly.terms[j].exp) {
            result.terms.push_back(terms[i++]);
        }
        else {
            result.terms.push_back(poly.terms[j++]);
        }
    }

    while (i < terms.size()) {
        result.terms.push_back(terms[i++]);
    }

    while (j < poly.terms.size()) {
        result.terms.push_back(poly.terms[j++]);
    }

    result.sortTerms();
    return result;
}

// 乘法
Polynomial Polynomial::Mult(const Polynomial& poly) const {
    Polynomial result;

    for (int i = 0; i < terms.size(); ++i) {
        for (int j = 0; j < poly.terms.size(); ++j) {
            float prodCoef = terms[i].coef * poly.terms[j].coef;
            int prodExp = terms[i].exp + poly.terms[j].exp;

            bool found = false;
            for (int k = 0; k < result.terms.size(); ++k) {
                if (result.terms[k].exp == prodExp) {
                    result.terms[k].coef += prodCoef;
                    found = true;
                    break;
                }
            }

            if (!found) {
                result.terms.push_back(Term(prodCoef, prodExp));
            }
        }
    }

    result.sortTerms();
    return result;
}

float Polynomial::Eval(float x) const {
    float result = 0;
    for (int i = 0; i < terms.size(); ++i) {
        result += terms[i].coef * pow(x, terms[i].exp);
    }
    return result;
}


istream& operator>>(istream& is, Polynomial& poly) {
    int n;
    cout << "有幾項:";
    is >> n;
    for (int i = 0; i < n; ++i) {
        float coef;
        int exp;
        cout << "輸入第 " << i + 1 << " 項的係數與指數: ";
        is >> coef >> exp;
        poly.terms.push_back(Term(coef, exp));
    }
    return is;
}


ostream& operator<<(ostream& os, const Polynomial& poly) {
    bool firstTerm = true;

    for (int i = 0; i < poly.terms.size(); ++i) {
        if (poly.terms[i].coef == 0) continue;

        if (!firstTerm && poly.terms[i].coef > 0) os << "+";
        os << poly.terms[i].coef << "x^" << poly.terms[i].exp << " ";

        firstTerm = false;
    }

    return os;
}

int main() {
    Polynomial p1, p2, sum, product;

    cout << "輸入第一個多項式:\n";
    cin >> p1;
    cout << "輸入第二個多項式:\n";
    cin >> p2;

    sum = p1.Add(p2);
    product = p1.Mult(p2);

    cout << "第一個多項式: " << p1 << endl;
    cout << "第二個多項式: " << p2 << endl;
    cout << "加法結果: " << sum << endl;
    cout << "乘法結果: " << product << endl;

    float x;
    cout << "輸入一個值帶入: ";
    cin >> x;
    cout << "P1(" << x << ") = " << p1.Eval(x) << endl;  
    cout << "P2(" << x << ") = " << p2.Eval(x) << endl;  
    return 0;
}
