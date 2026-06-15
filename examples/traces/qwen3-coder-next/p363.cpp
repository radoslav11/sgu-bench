#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <climits>

using namespace std;

struct Person {
    int arrival;
    int serviceTime;
};

struct Event {
    int time;       // time when event happens (departure)
    int windowIdx;  // index of window where departure happens
    
    bool operator>(const Event& other) const {
        return time > other.time;
    }
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int N, M;
    cin >> N >> M;
    
    vector<Person> people(N);
    for (int i = 0; i < N; i++) {
        cin >> people[i].arrival >> people[i].serviceTime;
    }
    
    // min-heap for departure events (earliest time first)
    priority_queue<Event, vector<Event>, greater<Event>> departures;
    
    // For each window, store the time when the current last person in the queue will finish
    // If no one is in the queue, the window is free at time 0
    vector<int> windowFinishTime(M, 0); // time when the window becomes free
    
    // For each person, store (queue_number, departure_time)
    vector<pair<int, int>> result(N);
    
    for (int i = 0; i < N; i++) {
        int arrivalTime = people[i].arrival;
        
        // Process all departures that happen at or before arrivalTime
        while (!departures.empty() && departures.top().time <= arrivalTime) {
            Event e = departures.top();
            departures.pop();
            // Update the window's finish time to the departure time
            // (since the person just left, the window becomes free at this time)
            windowFinishTime[e.windowIdx] = e.time;
        }
        
        // Now find the best queue: minimum length, and among those, smallest window number
        // Queue length = number of people currently in the queue (not yet departed)
        // We can compute the length for each window:
        // We'll simulate by tracking how many people are in each queue.
        // But alternatively, we can compute queue length by counting how many people have been assigned to this window
        // and haven't departed yet. Since we process people in arrival order and handle departures before each arrival,
        // we can maintain queue lengths.
        
        // Instead of maintaining queue lengths separately, we can note:
        // When we assign a person to window j, the queue length for window j increases by 1.
        // When a departure happens at window j, queue length decreases by 1.
        // So let's maintain queue lengths.
        
        // However, we popped all departures that happen <= arrivalTime, so now
        // all remaining people in windows are those whose departure time > arrivalTime.
        // So we need to maintain current queue lengths.
        
        // Let's maintain a vector of current queue lengths (number of people in queue for each window).
        // We'll update this as we assign people and process departures.
        
        // But since we already processed departures up to arrivalTime, the current queue lengths
        // are the number of people assigned to each window minus the number of departures that have happened so far.
        // However, we don't track the assignments separately.
        
        // Let's restructure: maintain a vector `queueLength` for each window.
        // When a departure event occurs, we decrease the corresponding queue length.
        // When we assign a person to window j, we increase queue length by 1, and set the departure time.
        
        // Actually, we can do without explicit queue lengths if we recompute from the priority queue?
        // Instead, let's maintain an array `queueLength`.
        
        // But we haven't initialized `queueLength` yet. Let's do that.
        // Since we are inside the main loop, let's refactor.
        
        // Better approach: maintain `queueLength` and `nextFreeTime` for each window.
        // Actually, `nextFreeTime` is not enough because multiple people can be in the queue.
        // We need to know when the *entire queue* will be cleared for a new person to be served.
        // Actually, the new person will be served only after all people currently in the queue are served.
        // So the time when the new person starts service is max(arrivalTime, finishTime of the last person in the queue).
        // And the finish time for the new person = start time + service time.
        
        // So we need to know: for each window, when will the window be free for a new person to start service?
        // That is the maximum of (current finish time of the last person in the queue, arrivalTime of the new person).
        
        // We can store for each window: the time when the last person in the queue will finish service.
        // That's exactly `windowFinishTime[j]`.
        // Then, when a new person arrives at time `t`:
        //   start_service_time = max(t, windowFinishTime[j])
        //   finish_time = start_service_time + service_time
        //   and update windowFinishTime[j] = finish_time
        
        // However, the complication is that departures happen in between.
        // When a person departs, we need to update the window's finish time?
        // Actually, `windowFinishTime[j]` should represent the time when the last person currently in the queue will finish.
        // When a person departs at time `d`, then if that person was the last in the queue, then the queue length decreases by 1,
        // but the finish time for the new last person? Actually, we only store the finish time of the last person.
        // But if multiple people are in the queue, we don't track each person's finish time.
        
        // Alternative: use a priority queue for each window? That would be too heavy (N, M <= 100, so it's acceptable).
        // But the problem says N, M up to 100, so we can do O(N*M) per person, which is 100*100=10000, acceptable.
        
        // Actually, simpler: simulate events in time order.
        // Since N and M are small (<=100), we can do:
        //   - Maintain an array `queues` of M queues (each is a list of service times, or just finish times)
        //   - But we don't need the exact list, we need:
        //        * the length of each queue
        //        * and the finish time of the last person in the queue (to compute when a new person will finish)
        
        // However, when a person departs, the queue front leaves, so the new last person's finish time is the next one? 
        // But we don't store all finish times.
        
        // Better: for each window, maintain a queue of finish times for each person in the queue (in order).
        // Then:
        //   - When a person arrives at time t:
        //        * First, for each window, pop all departures that happened at time <= t (but we need to do event-based)
        //        * Instead, we can process all events (departures) that happen at or before t at the time of arrival.
        
        // Let's use an event-based simulation:
        //   We'll process people one by one in arrival order (which is given, and arrivals are non-decreasing).
        //   Before processing person i, we process all departures that occur at time <= people[i].arrival.
        
        // How to know when a departure occurs? We can store for each window a queue of departure times.
        // We maintain M queues (for windows 0 to M-1), each queue holds the departure times of people in that queue.
        // Then, when processing an arrival, for each window, if the front of the queue is <= arrivalTime, pop it (because the person leaves).
        // Then, the window's queue length is the size of the queue.
        
        // But we don't need the exact departure times for the simulation of assignment, only for updating the windows.
        // Actually, when assigning, we need to know the finish time of the new person.
        
        // Steps for person i (arrival = a, service = s):
        //   1. For each window j, while queue[j] is not empty and front of queue[j] <= a, pop it.
        //   2. Now, for each window j, the queue length = queue[j].size().
        //   3. Find the window with minimum queue length, and if tie, smallest j.
        //   4. Let start = max(a, (queue[j] is empty ? a : queue[j].back() )) 
        //      But wait: the last person in the queue finishes at queue[j].back()? 
        //      Actually, the queue[j] stores departure times. The last person's departure time is the last one we pushed.
        //      So if the queue is not empty, the new person will start service at queue[j].back() (because the window is busy until then) 
        //      but actually, no: the window becomes free when the last person departs, which is queue[j].back().
        //      However, the new person arrives at time a, so if a < queue[j].back(), they have to wait until queue[j].back().
        //      Then they are served for s time, so they depart at queue[j].back() + s.
        //      If the queue is empty, then they start immediately at a, depart at a + s.
        //
        //   5. Push the new departure time (start + s) to queue[j].
        //   6. Record the result: (j+1, start+s)
        //
        // However, note: when multiple people depart at the same time as the new arrival, the problem says:
        //   "When next person come in the moment when one or several people have just proceeded to their windows, he or she waits first until these people will leave their queues and then he or she chooses the queue"
        //   So we process departures *before* the new person chooses.
        //
        // This matches step 1: we remove all departures that happen at time <= a (including a).
        //
        // Implementation:
        //   We'll maintain a vector of queues (we can use vector<queue<int>> or vector<priority_queue>? 
        //   But we don't need priority queue for the windows, because the departures happen in order? 
        //   Actually, for each window, the departures happen in FIFO order (since the person at the front leaves first), 
        //   so the departure times for a window are increasing. So we can use a queue (FIFO) for each window.
        //
        // But note: the departure times for a window: 
        //   The first person: departs at a1 + s1.
        //   The second person: departs at (max(a2, a1+s1)) + s2, which is >= a1+s1.
        //   So yes, for a given window, departure times are non-decreasing. So a queue (FIFO) works.
        //
        // However, we don't need to store the departure times per window in a queue of integers? 
        // Actually, we can store the finish time (departure time) for each person in the window's queue.
        // And when a new person arrives, we pop from the front of the window's queue all events that are <= current arrival time.
        //
        // Steps:
        //   vector<queue<int>> windows(M);   // for each window, store the departure time of each person in the queue (in order)
        //   For each person i (in order of increasing arrival):
        //        // Clean up: for each window, pop all departures that are <= arrivalTime
        //        for (int j = 0; j < M; j++) {
        //            while (!windows[j].empty() && windows[j].front() <= people[i].arrival) {
        //                windows[j].pop();
        //            }
        //        }
        //        // Now find the best window
        //        int bestWindow = 0;
        //        int minSize = windows[0].size();
        //        for (int j = 1; j < M; j++) {
        //            if (windows[j].size() < minSize) {
        //                minSize = windows[j].size();
        //                bestWindow = j;
        //            }
        //        }
        //        // Now, compute departure time for the new person
        //        int startService;
        //        if (windows[bestWindow].empty()) {
        //            startService = people[i].arrival;
        //        } else {
        //            // the last person in the queue departs at windows[bestWindow].back()
        //            startService = windows[bestWindow].back();
        //        }
        //        int departTime = startService + people[i].serviceTime;
        //        windows[bestWindow].push(departTime);
        //        result[i] = {bestWindow+1, departTime};
        //
        // But note: the problem says "If there are several such queues than he or she chooses queue to the window with lowest number."
        // We break ties by smallest window index, and we start from 0 (which is window 1) so that's correct.
        //
        // However, the above cleaning step for each window is done for every person. 
        // Since N, M <= 100, and each person is pushed and popped at most once per window, worst-case O(N*M) = 10000, acceptable.
        //
        // But note: the cleaning step for a window might be done multiple times for the same window, but overall each person is popped once.
        // So total operations over all people: O(N) for departures (each departure popped once) and O(N*M) for cleaning (but actually, for each person, we check M windows, so O(N*M) = 100*100=10000).
        //
        // Let's test with the example:
        //   Input: 
        //        5 2
        //        1 2
        //        3 4
        //        4 1
        //        5 2
        //        6 1
        //
        //   Windows: 2 (indexed 0 and 1)
        //
        //   Person0: arrival=1, service=2
        //        Clean windows: both empty.
        //        bestWindow=0 (size=0)
        //        startService = 1, departTime=3.
        //        windows[0] = [3]
        //        result[0] = (1, 3)
        //
        //   Person1: arrival=3, service=4
        //        Clean window0: front=3 <=3 -> pop, so windows[0] becomes empty.
        //                   window1: empty.
        //        Now both windows empty -> choose window0.
        //        startService=3, departTime=7.
        //        windows[0] = [7]
        //        result[1] = (1,7)
        //
        //   Person2: arrival=4, service=1
        //        Clean: window0: 7>4 -> remains [7]; window1: empty.
        //        sizes: [1,0] -> window1 has size 0, so bestWindow=1.
        //        startService = 4 (since window1 empty), departTime=5.
        //        windows[1] = [5]
        //        result[2] = (2,5)
        //
        //   Person3: arrival=5, service=2
        //        Clean: window0: 7>5 -> remains [7]; window1: front=5<=5 -> pop, so becomes empty.
        //        Now windows: [7] and [] -> sizes [1,0] -> choose window1.
        //        startService=5, departTime=7.
        //        windows[1] = [7]
        //        result[3] = (2,7)
        //
        //   Person4: arrival=6, service=1
        //        Clean: window0: 7>6 -> remains [7]; window1: 7>6 -> remains [7].
        //        sizes: [1,1] -> choose window0 (smaller index).
        //        startService = max(6, 7) = 7, departTime=8.
        //        windows[0] = [7,8]
        //        result[4] = (1,8)
        //
        //   Output:
        //        1 3
        //        1 7
        //        2 5
        //        2 7
        //        1 8
        //
        //   Matches the sample output.
        //
        // So we'll implement with M queues (using std::queue).
        
        vector<queue<int>> windows(M);
        vector<pair<int, int>> ans;
        
        for (int idx = 0; idx < N; idx++) {
            int a = people[idx].arrival;
            int t = people[idx].serviceTime;
            
            // Clean up all windows: remove all departures that happen at or before time a
            for (int j = 0; j < M; j++) {
                while (!windows[j].empty() && windows[j].front() <= a) {
                    windows[j].pop();
                }
            }
            
            // Find the window with minimum queue size, and smallest index in case of tie
            int bestWin = 0;
            int minSize = windows[0].size();
            for (int j = 1; j < M; j++) {
                if (windows[j].size() < minSize) {
                    minSize = windows[j].size();
                    bestWin = j;
                }
            }
            
            // Compute departure time
            int departTime;
            if (windows[bestWin].empty()) {
                departTime = a + t;
            } else {
                departTime = windows[bestWin].back() + t;
            }
            
            windows[bestWin].push(departTime);
            ans.push_back({bestWin + 1, departTime});
        }
        
        // Output the answers
        for (auto& p : ans) {
            cout << p.first << " " << p.second << "\n";
        }
        
        return 0;
    }
}
