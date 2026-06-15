#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <algorithm>

using namespace std;

const double EPS = 1e-8;

struct Vector3 {
    double x, y, z;
    
    Vector3() : x(0), y(0), z(0) {}
    Vector3(double x, double y, double z) : x(x), y(y), z(z) {}
    
    Vector3 operator+(const Vector3& v) const { return Vector3(x + v.x, y + v.y, z + v.z); }
    Vector3 operator-(const Vector3& v) const { return Vector3(x - v.x, y - v.y, z - v.z); }
    Vector3 operator*(double k) const { return Vector3(x * k, y * k, z * k); }
    double dot(const Vector3& v) const { return x * v.x + y * v.y + z * v.z; }
    double norm() const { return sqrt(x*x + y*y + z*z); }
    Vector3 normalized() const {
        double n = norm();
        if (n < EPS) return *this;
        return Vector3(x / n, y / n, z / n);
    }
};

// Sphere: center (x, y, z), radius r
struct Sphere {
    Vector3 center;
    double r;
    int id;
    
    Sphere(double x, double y, double z, double r, int id) 
        : center(x, y, z), r(r), id(id) {}
};

// Ray: point p and direction d
struct Ray {
    Vector3 p, d;
    
    Ray(Vector3 p, Vector3 d) : p(p), d(d.normalized()) {}
};

double solveQuadratic(double a, double b, double c) {
    // Returns smallest positive root of a*t^2 + b*t + c = 0
    double disc = b * b - 4 * a * c;
    if (disc < -EPS) return -1.0; // no real roots
    disc = max(0.0, disc);
    
    double sqrt_disc = sqrt(disc);
    double t1 = (-b - sqrt_disc) / (2 * a);
    double t2 = (-b + sqrt_disc) / (2 * a);
    
    // We want the smallest positive t
    if (t1 > EPS) return t1;
    if (t2 > EPS) return t2;
    return -1.0;
}

// Find intersection point between ray and sphere
double intersectRaySphere(const Ray& ray, const Sphere& sphere) {
    Vector3 oc = ray.p - sphere.center;
    double a = ray.d.dot(ray.d);
    double b = 2 * oc.dot(ray.d);
    double c = oc.norm() * oc.norm() - sphere.r * sphere.r;
    
    // Quadratic equation: a*t^2 + b*t + c = 0
    double t = solveQuadratic(a, b, c);
    
    // Check if the intersection is in forward direction
    if (t > EPS) {
        return t;
    }
    return -1.0;
}

// Calculate reflection direction given incident ray and normal at point of incidence
Vector3 reflect(const Vector3& incident, const Vector3& normal) {
    // R = I - 2*(I·N)*N
    return incident - normal * (2 * incident.dot(normal));
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n;
    cin >> n;
    
    vector<Sphere> spheres;
    for (int i = 0; i < n; ++i) {
        double x, y, z, r;
        cin >> x >> y >> z >> r;
        spheres.push_back(Sphere(x, y, z, r, i + 1));
    }
    
    // Read laser source point and direction point
    double sx, sy, sz, dx, dy, dz;
    cin >> sx >> sy >> sz >> dx >> dy >> dz;
    
    Vector3 start(sx, sy, sz);
    Vector3 dir(dx - sx, dy - sy, dz - sz);
    
    Ray ray(start, dir);
    
    vector<int> reflections;
    
    // Simulate up to 10 reflections (plus one extra to check if there are more)
    for (int iter = 0; iter <= 10; ++iter) {
        double min_t = -1.0;
        int hit_id = -1;
        
        // Find the closest sphere that the ray hits
        for (int i = 0; i < n; ++i) {
            double t = intersectRaySphere(ray, spheres[i]);
            if (t > EPS && (min_t < 0 || t < min_t)) {
                min_t = t;
                hit_id = spheres[i].id;
            }
        }
        
        if (hit_id == -1) {
            break; // No more reflections
        }
        
        if (reflections.size() < 10) {
            reflections.push_back(hit_id);
        }
        
        // If we've already recorded 10 hits, check if there's an 11th
        if (reflections.size() == 10) {
            // Try to find the next hit without updating the ray
            double min_t_next = -1.0;
            for (int i = 0; i < n; ++i) {
                double t = intersectRaySphere(ray, spheres[i]);
                if (t > EPS && (min_t_next < 0 || t < min_t_next)) {
                    min_t_next = t;
                }
            }
            
            if (min_t_next > EPS) {
                // There's at least one more reflection, so add "etc."
                cout << reflections[0];
                for (int i = 1; i < reflections.size(); ++i) {
                    cout << " " << reflections[i];
                }
                cout << " etc." << endl;
            } else {
                // No more reflections after this one
                for (int i = 0; i < reflections.size(); ++i) {
                    if (i > 0) cout << " ";
                    cout << reflections[i];
                }
                cout << endl;
            }
            return 0;
        }
        
        // Calculate the point of intersection
        Vector3 hit_point = ray.p + ray.d * min_t;
        
        // Calculate the normal at the point of incidence (from sphere center to hit point)
        Vector3 normal = (hit_point - spheres[hit_id-1].center).normalized();
        
        // Update the ray: start from hit_point in reflected direction
        Vector3 incident_dir = ray.d;
        Vector3 reflect_dir = reflect(incident_dir, normal);
        
        // Move the ray start slightly forward along reflection direction to avoid self-intersection
        Vector3 new_start = hit_point + reflect_dir * EPS;
        
        ray.p = new_start;
        ray.d = reflect_dir.normalized();
    }
    
    // Output all reflections (shouldn't reach here if we found more than 10)
    for (int i = 0; i < reflections.size(); ++i) {
        if (i > 0) cout << " ";
        cout << reflections[i];
    }
    cout << endl;
    
    return 0;
}
