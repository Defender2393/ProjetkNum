#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <stdexcept>
#include "Vec3.cpp"

class Particle {
public:
    // --- reine Daten ---
    Vec3   x{0,0,0};   // Position [m]
    Vec3   U{0,0,0};   // Geschwindigkeit [m/s]
    double d{0.0};     // Durchmesser [m]

    Particle() = default;
    Particle(const Vec3& x0, const Vec3& U0, double d0)
        : x(x0), U(U0), d(d0) {}

    // ------------------------------------------------------------
    // IO-Format (einfach, whitespace-getrennt):
    //    x y z   Ux Uy Uz   d
    // Kommentare mit '#' werden ignoriert.
    // Beispielzeile:
    //    0.5 1.2 0.0   0.0 -0.2 0.0   0.0015
    // ------------------------------------------------------------

    static bool parseLine(const std::string& line, Particle& out) {
        std::string s = trimCopy(line);
        if (s.empty() || s[0]=='#') return false;
        std::istringstream is(s);
        Particle p;
        if (!(is >> p.x.x >> p.x.y >> p.x.z
                 >> p.U.x >> p.U.y >> p.U.z
                 >> p.d)) {
            return false;
        }
        out = p;
        return true;
    }

    std::string toLine() const {
        std::ostringstream os;
        os.setf(std::ios::fixed); os.precision(10);
        os << x.x << ' ' << x.y << ' ' << x.z << ' '
           << U.x << ' ' << U.y << ' ' << U.z << ' '
           << d;
        return os.str();
    }

    // ------------------------------------------------------------
    // load Datei
    // ------------------------------------------------------------

    // ------------------------------------------------------------
    // Rechnungen
    // ------------------------------------------------------------

private:
    static std::string trimCopy(const std::string& s) {
        size_t b = 0, e = s.size();
        while (b<e && std::isspace(static_cast<unsigned char>(s[b]))) ++b;
        while (e>b && std::isspace(static_cast<unsigned char>(s[e-1]))) --e;
        return s.substr(b, e-b);
    }
};
