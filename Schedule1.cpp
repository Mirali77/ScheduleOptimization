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
using namespace std;
#define ll long long
#define ld long double
#define ull unsigned ll
#define forn(i, n) for (ll i = 1; i <= n; i++)
#define dforn(i, a, b) for (ll i = a; i <= b; i++)
#define rforn(i, n) for (ll i = n; i >= 1; i--)
#define rdforn(i, a, b) for (ll i = b; i >= a; i--)
const ll mod = 1e9 + 7;
const ll N = 1e5;
const ll inf = 3e17;
const double eps = 1e-10;

string merge_schedules(string& sc1, string& sc2) {
	string res = sc1;
	dforn(i, 0, 6) {
		res[i] = min(sc1[i], sc2[i]);
	}
	return res;
}

vector<vector<ll>> create_graph(ll n, vector<string>& base, vector<vector<ll>>& timeslots) {
	ll sz1 = n * (n - 1) / 2, k = (n + 1) / 2, sz2 = k * (n - 1), sz = sz1 + sz2;
	vector<vector<ll>> res(n * n);
	dforn(i, 0, n - 2) {
		dforn(j, 0, k - 1) {
			res.push_back(vector<ll>());
			dforn(h, 0, n - 1) {
				dforn(p, h + 1, n - 1) {
					string scedule = merge_schedules(base[h], base[p]);
					if (scedule[timeslots[i][j] % 7] == '1') {
						res[h * n + p].push_back(n * n + i * k + j);
						res[n * n + i * k + j].push_back(h * n + p);
					}
				}
			}
		}
	}
	return res;
}

bool dfs(ll v, vector<bool>& used, vector<ll>& matching, vector<vector<ll>>& g) {
	if (used[v]) {
		return false;
	}
	used[v] = true;
	dforn(i, 0, g[v].size() - 1) {
		ll to = g[v][i];
		if (matching[to] == -1 || dfs(matching[to], used, matching, g)) {
			matching[to] = v;
			return true;
		}
	}
	return false;
}

vector<pair<ll, ll>> kunn(ll n, vector<vector<ll>>& g) {
	ll sz1 = n * n, k = (n + 1) / 2, sz2 = k * (n - 1), sz = sz1 + sz2;
	vector<ll> matching(sz, -1);
	dforn(i, 0, n * n - 1) {
		if (g[i].empty()) {
			continue;
		}
		vector<bool> used(sz);
		dfs(i, used, matching, g);
	}
	vector<pair<ll, ll>> res(sz2);
	dforn(i, 0, sz2 - 1) {
		ll cur = matching[n * n + i];
		if (cur == -1) {
			res[i] = { -1, -1 };
		}
		else {
			res[i] = { cur / n, cur % n };
		}
	}
	return res;
}

bool check(ll n, vector<pair<ll, ll>>& res) {
	ll k = (n + 1) / 2, sz2 = k * (n - 1), cnt = 0;
	dforn(i, 0, sz2 - 1) {
		cnt += res[i].first != -1;
	}
	return cnt == n * (n - 1) / 2;
}

void solve() {
	ll n;
	cout << "Enter the number of teams: ";
	cin >> n;
	cout << "Enter the information about teams (name, week schedule):" << '\n'; //Schedule - binary string with length 7 (1/0-free/not)
	ll sz1 = n * (n - 1) / 2, k = (n + 1) / 2, sz2 = k * (n - 1), sz = sz1 + sz2;
	vector<string> inv(n), base(n);
	vector<vector<ll>> timeslots(n - 1, vector<ll>(k));
	forn(i, n) {
		string name, schedule;
		cin >> name >> schedule;
		inv[i - 1] = name;
		base[i - 1] = schedule;
	}
	cout << '\n';
	dforn(i, 0, n - 2) {
		dforn(j, 0, k - 1) {
			timeslots[i][j] = i * k + j;
		}
	}
	vector<vector<ll>> g = create_graph(n, base, timeslots);
	vector<pair<ll, ll>> res = kunn(n, g);
	if (!check(n, res)) {
		cout << "It's impossible to make schedule with these constraints." << '\n' << '\n';
		return;
	}
	dforn(i, 0, n - 2) {
		cout << i + 1 << " tour: " << '\n';
		dforn(j, 0, k - 1) {
			if (res[i * k + j].first == -1) {
				cout << j + 1 << ") match: -" << '\n';
				continue;
			}
			cout << j + 1 << ") match: " << inv[res[i * k + j].first] << " - " << inv[res[i * k + j].second] << '\n';
		}
		cout << '\n';
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
	while (t) {
		t--;
		solve();
	}
	return 0;
}