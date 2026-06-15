#include <iostream>
#include <cmath>
#include <iomanip>

int main() {
    double R1, R2;
    std::cin >> R1 >> R2;
    
    // Ensure R1 <= R2 for consistency (the formula is symmetric)
    if (R1 > R2) {
        std::swap(R1, R2);
    }
    
    // The volume of intersection of two perpendicular cylinders of radii R1 and R2
    // When R1 = R2 = R, volume = (16/3) * R^3
    // For general case, the formula is:
    // V = 8 * integral from 0 to R1 of sqrt(R1^2 - x^2) * sqrt(R2^2 - x^2) dx
    //
    // This integral can be expressed using elliptic integrals, but there's a known formula:
    // V = (8/3) * R1^3 * F(acos(R2/R1), (R1/R2)^2) when R1 <= R2? 
    // Actually, the standard formula is:
    // V = 8 * R1 * R2^2 * E(k) where k = sqrt(1 - (R1/R2)^2) and E is the complete elliptic integral of the second kind
    // But this is valid when R1 <= R2.
    //
    // Alternatively, we can use:
    // V = (8/3) * R1^3 * F(acos(R1/R2), (R2/R1)^2) ... not quite.
    //
    // The correct formula for the volume of intersection of two perpendicular cylinders
    // with radii R and r (R <= r) is:
    // V = 8 * r * R^2 * E(sqrt(1 - (R/r)^2))
    // where E(k) is the complete elliptic integral of the second kind.
    //
    // However, there's a more stable formula using the arithmetic-geometric mean or
    // we can use the integral representation and numerical integration for the constraints.
    //
    // Given the constraints (R1, R2 between 1 and 100) and the required precision (1e-4),
    // numerical integration is feasible.
    //
    // But let's check if we can use a known closed form.
    // Actually, the volume is:
    // V = 8 * ∫[0, min(R1,R2)] sqrt(R1^2 - x^2) * sqrt(R2^2 - x^2) dx
    //
    // We can use numerical integration with Simpson's rule or Gaussian quadrature.
    // Since the problem is small (only one test case, but we need precision 1e-4),
    // and time limit is 0.5 sec, even a simple Simpson with enough intervals should work.
    //
    // Alternatively, use the known formula with elliptic integrals.
    // The complete elliptic integral of the second kind E(k) can be computed via
    // the series: E(k) = (π/2) * sum_{n=0}^{∞} [(2n)! / (2^(2n) * (n!)^2)]^2 * k^(2n) / (1 - 2n)
    // But that's complicated.
    //
    // Actually, in C++11 and later, we have std::ellint_2 in <cmath> (C++17).
    // Let me check: yes, std::ellint_2(k) computes the complete elliptic integral of the second kind.
    //
    // Formula: For two perpendicular cylinders of radii a and b (a ≤ b),
    // V = 8 * b * a^2 * E(sqrt(1 - (a/b)^2))
    //
    // Let me verify with a = b = 1:
    // k = sqrt(1 - 1) = 0
    // E(0) = π/2
    // V = 8 * 1 * 1^2 * (π/2) = 4π ≈ 12.566... but the sample output is 5.3333.
    // So that's not correct.
    //
    // Correction: The standard "Steinmetz solid" for two equal cylinders of radius R
    // has volume V = (16/3) * R^3. For R=1, that's 16/3 ≈ 5.3333, which matches the sample.
    //
    // So what's the general formula?
    //
    // After checking references, the volume of intersection of two perpendicular cylinders
    // of radii a and b (a ≤ b) is:
    // V = 8 * a^3 * F( arcsin(b/a), 1 - (b/a)^2 ) ??? No.
    //
    // Actually, the correct formula is:
    // V = 8 * b * E(k) where k = sqrt(1 - (a/b)^2) and E is the complete elliptic integral of the second kind,
    // but that gives for a=b: k=0, E(0)=π/2, so V=8*b*(π/2)=4πb, which is wrong.
    //
    // Let me re-derive quickly.
    // The volume can be computed as 8 times the volume in the first octant.
    // In the first octant, for x from 0 to a (assuming a ≤ b), y from 0 to sqrt(a^2 - x^2),
    // and z from 0 to sqrt(b^2 - x^2) ??? Not exactly.
    //
    // Actually, for a fixed x, the cross-section is a rectangle with sides 2*sqrt(R1^2 - x^2) and 2*sqrt(R2^2 - x^2),
    // but only for |x| ≤ min(R1, R2). So the area at position x is 4 * sqrt(R1^2 - x^2) * sqrt(R2^2 - x^2).
    // Then the volume is ∫_{-min(R1,R2)}^{min(R1,R2)} 4 * sqrt(R1^2 - x^2) * sqrt(R2^2 - x^2) dx
    // = 8 * ∫_{0}^{min(R1,R2)} sqrt(R1^2 - x^2) * sqrt(R2^2 - x^2) dx.
    //
    // For R1 = R2 = R, this becomes:
    // 8 * ∫_{0}^{R} (R^2 - x^2) dx = 8 * [R^2*x - x^3/3]_{0}^{R} = 8*(R^3 - R^3/3) = 8*(2*R^3/3) = 16*R^3/3.
    // So that matches.
    //
    // Now, for the general case, we need to compute:
    // I = ∫_{0}^{a} sqrt(a^2 - x^2) * sqrt(b^2 - x^2) dx, where a = min(R1, R2), b = max(R1, R2).
    //
    // This integral can be expressed in terms of elliptic integrals.
    // Specifically:
    // I = (1/3) * [ (a^2 + b^2) * E(k) - (a^2 - b^2) * K(k) ] * a
    // where k = sqrt(1 - (a/b)^2), and K and E are complete elliptic integrals of the first and second kind.
    //
    // But wait, let me verify with a=b:
    // If a=b, then k=0.
    // E(0) = π/2, K(0) = π/2.
    // Then I = (1/3) * [2a^2 * (π/2) - 0] * a = (1/3)*a^2 * π * a = (π/3)*a^3.
    // Then volume = 8 * I = 8*(π/3)*a^3 = (8π/3)*a^3, but we know it should be 16/3 * a^3.
    // So that's not matching.
    //
    // Alternative formula:
    // According to standard references, the volume is:
    // V = 8 * b * E(k) * a^2 / b^2 ? No.
    //
    // Actually, the correct formula is:
    // V = 8 * a * b^2 * E(k) where k = sqrt(1 - (a/b)^2) for a ≤ b.
    // For a=b: k=0, E(0)=π/2, so V=8*a*a^2*(π/2)=4πa^3, which is not 16/3 a^3.
    //
    // I think I made a mistake in the cross-section.
    // Let me reconsider.
    // The equation of the first cylinder (along y-axis): x^2 + z^2 ≤ R1^2.
    // The equation of the second cylinder (along x-axis): y^2 + z^2 ≤ R2^2.
    // For a fixed z, the cross-section in the xy-plane is a rectangle: |x| ≤ sqrt(R1^2 - z^2), |y| ≤ sqrt(R2^2 - z^2).
    // So the area is 4 * sqrt(R1^2 - z^2) * sqrt(R2^2 - z^2).
    // Then volume = ∫_{z=-min(R1,R2)}^{min(R1,R2)} 4 * sqrt(R1^2 - z^2) * sqrt(R2^2 - z^2) dz
    // = 8 * ∫_{0}^{a} sqrt(R1^2 - z^2) * sqrt(R2^2 - z^2) dz, where a = min(R1, R2).
    //
    // Now, for R1=R2=R:
    // 8 * ∫_{0}^{R} (R^2 - z^2) dz = 8 * [R^2 z - z^3/3]_{0}^{R} = 8*(R^3 - R^3/3) = 16*R^3/3. Correct.
    //
    // Now, for the general case, we need to compute:
    // I = ∫_{0}^{a} sqrt(a^2 - z^2) * sqrt(b^2 - z^2) dz, with a = min(R1,R2), b = max(R1,R2).
    //
    // This integral is known and can be expressed as:
    // I = (1/3) * [ (a^2 + b^2) * E(k) - (a^2 - b^2) * K(k) ] * a, but let me check the parameters.
    // Actually, standard form: 
    // ∫_{0}^{a} sqrt((a^2 - z^2)(b^2 - z^2)) dz = (a/3) * [ (a^2 + b^2) * E(k) - (a^2 - b^2) * K(k) ] 
    // where k = sqrt(1 - (a/b)^2) [assuming b >= a].
    //
    // Let me test with a=b:
    // k = 0, E(0)=π/2, K(0)=π/2.
    // Then I = (a/3) * [2a^2 * (π/2) - 0] = (a/3) * a^2 * π = (π a^3)/3.
    // Then volume = 8 * I = 8π a^3 / 3, but we need 16a^3/3.
    // So 8π/3 ≈ 8.377, but 16/3 ≈ 5.333. Not matching.
    //
    // Wait, I see the mistake: when a=b, sqrt((a^2 - z^2)(b^2 - z^2)) = a^2 - z^2, and 
    // ∫_{0}^{a} (a^2 - z^2) dz = [a^2 z - z^3/3]_{0}^{a} = a^3 - a^3/3 = 2a^3/3.
    // So I should be 2a^3/3, but the formula gave π a^3 / 3. So the formula must be wrong.
    //
    // Actually, the elliptic integral formula is for a different normalization.
    // Let me look for a reliable source.
    // According to MathWorld (Weisstein, "Cylinder-Cylinder Intersection"), the volume is:
    // V = 8 * min(R1, R2)^3 * F( arcsin(max(R1,R2)/min(R1,R2)), (min(R1,R2)/max(R1,R2))^2 ) 
    // but that doesn't make sense because arcsin of a number >1 is undefined.
    //
    // Actually, MathWorld gives:
    // V = 8 * R^3 * F(π/2, k) where k = sqrt(1 - (r/R)^2) for r <= R, but F is the elliptic integral of the first kind?
    // No, for the Steinmetz solid of two cylinders of radii r and R (r <= R), the volume is:
    // V = 8 * r^2 * sqrt(R^2 - r^2) + 8 * r^3 * arcsin(r/R) + (8/3) * r^3 * (R/r)^2 * [E(k) - (1 - (r/R)^2) * K(k)] 
    // where k = sqrt(1 - (r/R)^2).
    //
    // Actually, a simpler approach: since the constraints are small (R1, R2 in [1,100]) and we only need 1e-4 precision,
    // we can use numerical integration.
    //
    // We'll integrate from 0 to a (a = min(R1,R2)) the function f(z) = sqrt(R1^2 - z^2) * sqrt(R2^2 - z^2).
    // Then multiply by 8.
    //
    // How many intervals? The function is smooth, so Simpson's rule with, say, 10000 intervals should be enough.
    // But 0.5 sec is plenty for 10000 intervals in C++.
    //
    // Alternatively, we can use the adaptive quadrature, but Simpson with fixed intervals is simple.
    //
    // Let's use Simpson's rule: 
    // ∫_{0}^{a} f(z) dz ≈ (h/3) * [f(z0) + 4*f(z1) + 2*f(z2) + 4*f(z3) + ... + f(zn)]
    // with n even.
    //
    // We'll take n = 100000 (to be safe for 1e-4 precision).
    //
    // But note: the problem says R1, R2 between 1 and 100, so a is at least 1, at most 100.
    // The function is well-behaved (no singularities in [0, a] since at z=a, one term becomes 0).
    //
    // Let me test with R1=R2=1: 
    // f(z) = 1 - z^2.
    // ∫_{0}^{1} (1 - z^2) dz = 2/3.
    // Volume = 8 * 2/3 = 16/3 ≈ 5.3333.
    //
    // So we'll do:
    // a = min(R1, R2)
    // b = max(R1, R2)
    // integral = 0
    // h = a / n
    // for i from 0 to n:
    //   z = i * h
    //   if i==0 or i==n: weight=1
    //   else if i%2==1: weight=4
    //   else: weight=2
    //   integral += weight * sqrt(R1^2 - z^2) * sqrt(R2^2 - z^2)
    // integral *= h/3
    // volume = 8 * integral
    //
    // However, to be efficient and precise, we can use n=10000 (which is 10001 points) and h = a/10000.
    //
    // Let's code that.

    double a = std::min(R1, R2);
    double b = std::max(R1, R2);
    
    // Number of intervals for Simpson's rule (must be even)
    int n = 100000; // 100000 is even and should give good precision
    double h = a / n;
    
    double integral = 0.0;
    for (int i = 0; i <= n; i++) {
        double z = i * h;
        double term1 = std::sqrt(R1 * R1 - z * z);
        double term2 = std::sqrt(R2 * R2 - z * z);
        double f = term1 * term2;
        
        if (i == 0 || i == n) {
            integral += f;
        } else if (i % 2 == 1) {
            integral += 4.0 * f;
        } else {
            integral += 2.0 * f;
        }
    }
    
    integral *= h / 3.0;
    double volume = 8.0 * integral;
    
    std::cout << std::fixed << std::setprecision(4) << volume << std::endl;
    
    return 0;
}
