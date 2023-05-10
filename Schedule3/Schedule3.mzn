% Data
% Number of teams/stadions/tours
int: Tn; int: Sn; int: TourN = Tn - ((Tn mod 2 + 1) mod 2);
% Distance constant
float: R = 6371;
% Teams coordinates
array [1..Tn] of float: TLongitude; array [1..Tn] of float: TLatitude;
% Stadions coordinates
array [1..Sn] of float: SLongitude; array [1..Sn] of float: SLatitude;
% Stadions price-list
array[1..Sn, 1..2, 5..23] of float: SPriceList;

% functions
function var float: dist(float: lt1, float: lg1, float: lt2, float: lg2) =
  abs(acos(sin(lt1) * sin(lt2) + cos(lt1) * cos(lt2) * cos(lg1 - lg2)) * R);

% Schedule
array[1..TourN, 1..Tn, 1..Tn, 1..Sn, 1..2, 5..23] of var 0..1: Tab;

% Constraints
constraint forall(tr in 1..TourN, t1 in 1..Tn, t2 in 1..Tn, s in 1..Sn, d in 1..2, h in 5..23) (
  Tab[tr, t1, t2, s, d, h] = Tab[tr, t2, t1, s, d, h]
);
constraint forall(t1 in 1..Tn, t2 in 1..Tn where t1 != t2) (
  sum(tr in 1..TourN, s in 1..Sn, d in 1..2, h in 5..23)(Tab[tr, t1, t2, s, d, h]) = 1
);
constraint forall(tr in 1..TourN, t1 in 1..Tn) (
  sum(t2 in 1..Tn, s in 1..Sn, d in 1..2, h in 5..23)(Tab[tr, t1, t2, s, d, h]) <= 1
);
constraint forall(tr in 1..TourN, s in 1..Sn, d in 1..2, h in 5..23) (
  sum(t1 in 1..Tn, t2 in 1..Tn)(Tab[tr, t1, t2, s, d, h]) <= 2
);

% Importance of objectives (A - Comfort of team/ B - Renting cost)
var float: A = 0.2; var float: B = 1;
% Objective function
var float: F = sum(tr in 1..TourN, t1 in 1..Tn, t2 in 1..Tn, s in 1..Sn, d in 1..2, h in 5..23 where t1 < t2) (Tab[tr, t1, t2, s, d, h] * (dist(TLatitude[t1], TLongitude[t1], SLatitude[s], SLongitude[s]) + dist(TLatitude[t2], TLongitude[t2], SLatitude[s], SLongitude[s]))) * A
+ sum(tr in 1..TourN, t1 in 1..Tn, t2 in 1..Tn, s in 1..Sn, d in 1..2, h in 5..23 where t1 < t2) (Tab[tr, t1, t2, s, d, h] * SPriceList[s, d, h]) * B;

% Objective
solve minimize (F);

output ["Objective function = "] ++ [show(F)] ++ ["\n"] ++
  ["(Team1-Team2: [Tour] [Stadion] [day] [hour])\n"] ++
  [show(t1) ++ "-" ++ show(t2) ++ ": " ++
  show([tr | tr in 1.. TourN, s in 1..Sn, d in 1..2, h in 5..23 where Tab[tr, t1, t2, s, d, h] = 1]) ++ " " ++
  show([s | tr in 1.. TourN, s in 1..Sn, d in 1..2, h in 5..23 where Tab[tr, t1, t2, s, d, h] = 1]) ++ " " ++ 
  show([d | tr in 1.. TourN, s in 1..Sn, d in 1..2, h in 5..23 where Tab[tr, t1, t2, s, d, h] = 1]) ++ " " ++ 
  show([h | tr in 1.. TourN, s in 1..Sn, d in 1..2, h in 5..23 where Tab[tr, t1, t2, s, d, h] = 1]) ++ " " ++ 
  "\n" | t1 in 1..Tn, t2 in 1..Tn where t1 < t2]
