#include <iostream>
#include <string>
#include <deque>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <map>
#include <set>
#include <queue>
#include <fstream>
#include <stack>
#include <cmath>
#include <bitset>
#include <unordered_set>
#include <unordered_map>
#include <random>
#include <array>
using namespace std;
#define ll long long
#define ld long double
#define ull unsigned ll
#define forn(i, n) for (ll i = 1; i <= n; i++)
#define dforn(i, a, b) for (ll i = a; i <= b; i++)
#define rforn(i, n) for (ll i = n; i >= 1; i--)
#define rdforn(i, a, b) for (ll i = b; i >= a; i--)
ll mod = 998244353;
ld pi = acos(-1);
const ll N = 1e5;
const ll inf = 1e18 + 1;
const double eps = 1e-8;

ll n, m, gn, gm, total_penalty, big_penalty = 10000;
map<string, ll> teams_mp, stadions_mp;
vector<string> inv_teams, teams_schedule, inv_stadions, stadions_schedule;
map<pair<ll, ll>, ll> team_pairs, matches;
vector<pair<ll, ll>> team_pairs_inv, matches_inv;
vector<pair<ll, ll>> stadions_coord, teams_coord;
vector<vector<ll>> graph, stadions_assign;
vector<ll> total_assign;

void restore() {
    teams_mp.clear();
    inv_teams.clear();
    inv_teams.resize(1);
    teams_schedule.clear();
    teams_schedule.resize(1);
    teams_coord.clear();
    teams_coord.resize(1);
    stadions_mp.clear();
    inv_stadions.clear();
    inv_stadions.resize(1);
    stadions_schedule.clear();
    stadions_schedule.resize(1);
    stadions_coord.clear();
    stadions_coord.resize(1);
    team_pairs.clear();
    team_pairs_inv.clear();
    team_pairs_inv.resize(1);
    matches.clear();
    matches_inv.clear();
    matches_inv.resize(1);
    graph.clear();
    stadions_assign.clear();
    total_assign.clear();
    total_penalty = 0;
}

string schedule_intersection(string& s1, string& s2, string& s3) {
    string res;
    dforn(i, 0, 6) {
        res.push_back((s1[i] == '1' && s2[i] == '1' && s3[i] == '1') + '0');
    }
    return res;
}

ll dist(pair<ll, ll>& coord1, pair<ll, ll>& coord2) {
    return abs(coord1.first - coord2.first) + abs(coord1.second - coord2.second);
}

void build_graph() {
    forn(i, n - 1) {
        dforn(j, i + 1, n) {
            team_pairs[{i, j}] = ++gn;
            team_pairs_inv.push_back({ i, j });
        }
    }
    total_assign.resize(gn + 1);
    forn(i, n - 1) {
        forn(j, (n + 1) / 2) {
            matches[{i, j}] = ++gm;
            matches_inv.push_back({ i, j });
        }
    }
    graph.resize(gn + 1, vector<ll>(gm + 1, inf));
    stadions_assign.resize(gn + 1, vector<ll>(gm + 1, -1));
    forn(i, gn) {
        forn(j, gm) {
            ll team1 = team_pairs_inv[i].first, team2 = team_pairs_inv[i].second;
            ll tour = matches_inv[j].first, match = matches_inv[j].second;
            forn(k, m) {
                ll stadion = k;
                string schedule = schedule_intersection(teams_schedule[team1], teams_schedule[team2], stadions_schedule[stadion]);
                ll day = (tour - 1) * ((n + 1) / 2) + match;
                ll penalty = dist(teams_coord[team1], stadions_coord[stadion]) + dist(teams_coord[team2], stadions_coord[stadion]);
                if (schedule[(day - 1) % 7] == '0') {
                    penalty += big_penalty;
                }
                if (penalty < graph[i][j]) {
                    graph[i][j] = penalty;
                    stadions_assign[i][j] = stadion;
                }
            }
        }
    }
}

void vengr_algo() {
    vector<ll> u(gn + 1), v(gm + 1), p(gm + 1), way(gm + 1);
    forn(i, gn) {
        p[0] = i;
        ll j0 = 0;
        vector<ll> minv(gm + 1, inf);
        vector<bool> used(gm + 1);
        do {
            used[j0] = true;
            ll i0 = p[j0], delta = inf, j1;
            forn(j, gm) {
                if (used[j]) {
                    continue;
                }
                ll cur = graph[i0][j] - u[i0] - v[j];
                if (cur < minv[j]) {
                    minv[j] = cur;
                    way[j] = j0;
                }
                if (minv[j] < delta) {
                    delta = minv[j];
                    j1 = j;
                }
            }
            dforn(j, 0, gm) {
                if (used[j]) {
                    u[p[j]] += delta;
                    v[j] -= delta;
                }
                else {
                    minv[j] -= delta;
                }
            }
            j0 = j1;
        } while (p[j0]);
        do {
            ll j1 = way[j0];
            p[j0] = p[j1];
            j0 = j1;
        } while (j0);
    }
    forn(i, gm) {
        if (graph[p[i]][i] == inf) {
            continue;
        }
        total_assign[p[i]] = i;
        total_penalty += graph[p[i]][i];
    }
}

void solve() {
    restore();
    cout << "Enter the number of teams: ";
    cin >> n;
    cout << "Enter the information about teams (name, schedule, coordinates):\n";
    forn(i, n) {
        string name, schedule;
        ll x, y;
        cin >> name >> schedule >> x >> y;
        teams_mp[name] = teams_mp.size() + 1;
        inv_teams.push_back(name);
        teams_schedule.push_back(schedule);
        teams_coord.push_back({ x, y });
    }
    cout << '\n';
    cout << "Enter the number of stadions: ";
    cin >> m;
    cout << "Enter the information about stadions: (name, schedule, coordinates):\n";
    forn(i, m) {
        string name, schedule;
        ll x, y;
        cin >> name >> schedule >> x >> y;
        stadions_mp[name] = stadions_mp.size() + 1;
        inv_stadions.push_back(name);
        stadions_schedule.push_back(schedule);
        stadions_coord.push_back({ x, y });
    }
    cout << '\n';
    build_graph();
    vengr_algo();
    cout << "Optimal schedule's total penalty: " << total_penalty << '\n';
    cout << "Optimal schedule (team1 - team2: stadion, tour, match):\n";
    forn(i, gn) {
        ll team1 = team_pairs_inv[i].first, team2 = team_pairs_inv[i].second;
        ll tour = matches_inv[total_assign[i]].first, match = matches_inv[total_assign[i]].second;
        ll stadion = stadions_assign[i][total_assign[i]];
        cout << inv_teams[team1] << " - " << inv_teams[team2] << ": " << inv_stadions[stadion] << " " << tour << " " << match << '\n';
    }
}


int main()
{
    ios::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);
    ll t = 1;
    cout << "Enter the number of divisions: ";
    cin >> t;
    while (t--) {
        solve();
    }
    return 0;
}