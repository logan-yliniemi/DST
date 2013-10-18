DST
===

Deep Sea Treasure --- Pareto Transformation

Optimizer-independent transformation of objectives to allow concave areas of the Pareto front to be found by linear combination of transformed objectives.

Tues Oct 15, 2013: 
=================

First optimizer used is a basic evolutionary algorithm (non-Pareto based). 30-step action chain, open loop; surfacing, OOB or sea floor leads to immediate episode end with no treasure reward.

Confirmed that population will converge to either one-step, 1-treasure policy or toward 124-treasure policy (median time steps vary stochastically, ballpark 22-25 [optimal 18]). Approximate trade-off value of BETA is 0.65 for switching between the two (non-normalized). As expected, no concave areas of the Pareto front attract convergence).

Fri Oct 18, 2013: 
=================

Calculated tradeoff BETA = 9/11. Observed tradeoff value was different due to the ease of finding the time-optimal policy (1 step) as opposed to the treasure-optimal policy (19 coordinated steps).

Integrated trigonometric transformation to find concave areas of the Pareto front. Converged to (-13,50).