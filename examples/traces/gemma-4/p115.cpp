#include <iostream>

using namespace std;

/**
 * Problem: 115. Calendar
 * Goal: Find the day of the week for any date in 2001.
 * Year 2001 is not a leap year.
 * Jan 1, 2001 was a Monday.
 * 
 * Days in each month for 2001:
 * Jan: 31, Feb: 28, Mar: 31, Apr: 30, May: 31, Jun: 30,
 * Jul: 31, Aug: 31, Sep: 30, Oct: 31, Nov: 30, Dec: 31
 * 
 * Day of week:
 * Monday: 1, Tuesday: 2, ..., Sunday: 7
 * 
 * Input: N M (day N, month M)
 * Output: Day of week (1-7) or "Impossible"
 */

int main() {
    // Optimize standard I/O for speed, although not strictly necessary for this problem.
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int N, M;
    // Read N (day) and M (month).
    if (!(cin >> N >> M)) {
        return 0;
    }

    // Number of days in each month for the year 2001 (non-leap year).
    // Index 0 is a placeholder to allow 1-based indexing for months.
    int daysInMonth[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    // First, verify if the provided month is within the standard range (1-12).
    if (M < 1 || M > 12) {
        cout << "Impossible" << endl;
        return 0;
    }

    // Second, verify if the provided day is valid for the given month.
    if (N < 1 || N > daysInMonth[M]) {
        cout << "Impossible" << endl;
        return 0;
    }

    // Calculate total days passed in the year up to the given date (N, M).
    // We start by summing the days in all months preceding the input month M.
    int totalDays = 0;
    for (int i = 1; i < M; ++i) {
        totalDays += daysInMonth[i];
    }
    // Add the days from the current month.
    totalDays += N;

    // Calculation for the day of the week:
    // We know Jan 1, 2001 was a Monday (day 1).
    // If totalDays = 1 (Jan 1st), then (1 - 1) % 7 + 1 = 1 (Monday).
    // If totalDays = 7 (Jan 7th), then (7 - 1) % 7 + 1 = 7 (Sunday).
    // If totalDays = 8 (Jan 8th), then (8 - 1) % 7 + 1 = 1 (Monday).
    // This formula correctly maps the day number to the 1-7 scale.
    int dayOfWeek = (totalDays - 1) % 7 + 1;

    // Output the result.
    cout << dayOfWeek << endl;

    return 0;
}

